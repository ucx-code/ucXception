from flask import Blueprint, request, make_response, current_app
from api.core.abort import abort
from framework.main import main
import api.database as database
from api.core.decorators import token_required
from multiprocessing import Process, Manager
import datetime, time, os

# Create a list with Manager to store the processes
worker_proce = False
manager = Manager()
lista = manager.list()

# Function to execute the processes
def worker():
    current_process = None
    while True:
            if len(lista) > 0 and (current_process is None or not current_process.is_alive()):
                current_process = Process(target=main, args=lista[0])
                current_process.start()
                current_process.join() # Wait for the process to finish
                lista.pop(0)
            time.sleep(2)

callframework = Blueprint("callframework", __name__)

#Create a process and execute main function from framework
@callframework.route('/execute', methods=['POST'])
@token_required
def call_framework_process(current_user):
    global worker_proce # Define the worker_proce as global
    
    data = request.get_json()

    if data is None or 'campaign_id' not in data:
        return abort("Malformed request syntax!", 400)

    value = database.verify_campaign_belong_user(data["campaign_id"], current_user["id"])

    if not value:
        return abort("Campaign not reachable",422)

    if current_app.config['FRAMEWORK_DATA']:

        campaign_id = data['campaign_id']

        campaign_database = database.get_campaign(campaign_id)

        if not campaign_database:
            return abort('Campaign with specific id does not exist!', 422)

        #Get from global framework data , campaign class , file and configuration aspects
        dicio_configuration = {}
        if current_app.config['FRAMEWORK_DATA']["campaigns"]:
            for campaign in current_app.config['FRAMEWORK_DATA']["campaigns"]:
                if campaign["data"]["campaign_name"] == campaign_database["type"]:
                    # Verify if class and file name exists, if not return error
                    if "campaign_class_name" in campaign["data"] and "campaign_class_name" in campaign["data"] and "campaign_class_name" in campaign["data"]:
                        dicio_configuration["campaign_class_name"] = campaign["data"]["campaign_class_name"]
                        dicio_configuration["campaign_file_name"] = campaign["data"]["campaign_file_name"]
                        if "configuration" in campaign["data"]:
                            dicio_configuration["campaign_configuration"] = campaign["data"]["configuration"]
                    else:
                        return abort('Could not verify campaign types!', 422)
        else:
            return abort("No data relative to components and/or campaigns!", 422)

        #Get all data associated with campaign
        executions = database.get_executions(campaign_id)
        campaign_target = database.get_campaign_target(campaign_id)
        fault_injector_target = database.get_campaign_fault_injector_target(campaign_id)
        files = database.get_files(campaign_id)

        if not executions or not campaign_target or not fault_injector_target or not files:
            return abort('Campaign must have executions and hosts!', 422)
        
        parameters = database.get_campaign_parameters(campaign_id)

        user_components = database.get_components(campaign_id)

        if user_components:
            for user_component in user_components:
                for component in current_app.config['FRAMEWORK_DATA'][user_component["component_type_name"]]:
                    
                    if component["data"]["component_name"] == user_component["component_type"]:
                        if ("component_file_name" and "component_class_name" ) in component["data"]:
                            user_component["component_file_name"] = component["data"]["component_file_name"]
                            user_component["component_class_name"] = component["data"]["component_class_name"]
                        else: 
                            return abort("Unepected error when reading component data!", 422)

                        if "constructor" in component["data"]:
                            user_component["constructor"] = component["data"]["constructor"]
                        
                        #Component target
                        component_target = database.get_component_host(user_component["idcomponent"])
                        if component_target:
                            user_component["target"] = component_target

                        #Component parameters
                        component_parameters = database.get_component_parameters(user_component["idcomponent"])
                        if component_parameters:
                            # Set type and accept fields
                            for db_parameter in component_parameters:
                                if "parameters" in component["data"] and db_parameter["name"] in component["data"]["parameters"]:
                                    db_parameter["type"] = component["data"]["parameters"][db_parameter["name"]]["type"]
                            
                            user_component["parameters"] = component_parameters

                        #Associated components
                        if "components_allowed" in component["data"]:
                            associated_component = database.get_component_associated_component(user_component["idcomponent"])
                            if associated_component:
                                user_component["associated_component"] = associated_component["components_idcomponent1"]


        if not database.update_begin_campaign_state(campaign_id,"executing",datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S")):
            return abort("Couldnt update state campaign!", 422)
        
        # Check the user option to use queue to manage the Campaigns
        # Without Manager
        if int(os.environ['USEQUEUE']) == 0:
            p = Process(target=main, args=(dicio_configuration, campaign_database, files, executions, campaign_target, fault_injector_target, parameters, user_components))
            p.start()
            return make_response({'message': f'Started execution for campaign {campaign_id}'}, 200, {'Content-Type': 'application/json'})
        # With Manager
        else:
            # Start the worker_proce as a separate process to manage the list with all the processes
            if worker_proce is False:
                worker_proce = Process(target=worker)
                worker_proce.start()

            # Add the new process to the list
            lista.append((dicio_configuration, campaign_database, files, executions, campaign_target, fault_injector_target, parameters, user_components))

            if len(lista) == 1:
                return make_response({'message': f'Started execution for campaign {campaign_id}'}, 200, {'Content-Type': 'application/json'})
            else:
                return make_response({'message': f'Added campaign {campaign_id} to the queue'}, 200, {'Content-Type': 'application/json'})
    else:
        return abort("No data relative to components and/or campaigns!", 422)