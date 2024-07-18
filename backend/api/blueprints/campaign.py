from flask import Blueprint, make_response, jsonify, current_app, request
from api.core.abort import abort
from api.core.decorators import token_required
import api.core.parser as utils
import api.database as database
import pickle, os
from multiprocessing import Process
from framework.main import main

campaign = Blueprint("campaign", __name__)

#GET CAMPAIGN DATA
@campaign.route('/campaigns/information', methods=['GET'])
@token_required
def get_campaign_information(current_user):

    if(current_app.config['FRAMEWORK_DATA']):
        retrive_information = []

        #Get campaigns information from global variable
        campaign_imformation = current_app.config['FRAMEWORK_DATA']["campaigns"]

        for campaign in campaign_imformation:
            retrive_information.append(campaign["data"])

        if(retrive_information):
            return make_response(jsonify(retrive_information), 200, {'Content-Type': 'application/json'})
        else:
            return abort("No data relative to campaigns!", 422)
    else:
        return abort("No data relative to components and/or campaigns!", 422)
        

#GET DATA FROM ALL COMPONENTS FROM A GIVEN TYPE
@campaign.route('/components/<component_type>', methods=['GET'])
@token_required
def get_components_names(current_user, component_type):
    componentListName = []

    if(current_app.config['FRAMEWORK_DATA']):
        #Get campaigns information from global variable
        component_imformation = current_app.config['FRAMEWORK_DATA'][component_type]

        if not component_imformation:
            return abort("No data relative to specified component!", 422)

        for component in component_imformation:
            #Get components names from specific component_type
            if "component_name" in component["data"]:
                componentListName.append(component["data"]["component_name"])
        
        #Verify if dicio not empty
        if(componentListName):
            return make_response(jsonify(componentListName), 200, {'Content-Type': 'application/json'})
        else:
            return abort("Does not exist components to use!", 422)
    else:
        return abort("No data relative to components and/or campaigns!", 422)


#GET DATA FROM SPECIFIC COMPONENT
@campaign.route('/components/<component_type>/<component_choice>', methods=['GET'])
@token_required
def get_component_information(current_user, component_type, component_choice):
    if(current_app.config['FRAMEWORK_DATA']):
        
        #Get campaigns information from global variable
        all_component_imformation = current_app.config['FRAMEWORK_DATA'][component_type]

        if not all_component_imformation:
            return abort("No data relative to specified component!", 422)

        for component in all_component_imformation:
            if "component_name" in component["data"]:
                if component["data"]["component_name"] == component_choice:
                    
                    component_aux = component["data"].copy()

                    if "components_allowed" in component["data"]:
                        component_aux["components_allowed"] = {"require" : True}

                    return make_response(jsonify(component_aux), 200, {'Content-Type': 'application/json'})

        return abort("No data is available with the specifications provided!", 422)
    else:
        return abort("No data relative to components and/or campaigns!", 422)


# Delete Campaign
@campaign.route('/campaigns/<campaign_id>', methods=['DELETE'])
@token_required
def delete_campaign(current_user,campaign_id):

    #Verify and validate campaign id
    if campaign_id is None:
        return abort("Malformed request syntax!", 400)

    value = database.verify_campaign_belong_user_returnable(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable.", 422)
    
    #Delete Campaign
    if not database.delete_campaign(campaign_id):
        return abort('Could not delete campaign!', 500)

    return make_response({'message': 'Campaign deleted successfully!'}, 200, {'Content-Type': 'application/json'})


# Get and Copy Information about a single Campaign
@campaign.route('/campaigns/<campaign_id>', methods=['GET'])
@token_required
def get_campaign(current_user, campaign_id):
    
    #Verify and validate campaign id
    if campaign_id is None:
        return abort("Malformed request syntax!", 400)

    value = database.verify_campaign_belong_user_returnable(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable.", 422)

    campaign_info = database.copy_campaign(campaign_id,current_user)

    campaign_info['message'] = f"Campaign with ID: {campaign_id} and NAME: '{campaign_info['Project_Name']}' Copied Successfully!"

    if campaign_info:
        return make_response(jsonify(campaign_info), 200, {'Content-Type': 'application/json'})
    else:
        return abort("No data is available with the specifications provided!", 422)
    

@campaign.route('/campaign/setup/suggestion_run', methods=['POST'])
@token_required
def suggestion_run(current_user):
    
    # Check if "file" is in the request
    if 'file' not in request.files:
        return abort("No file selected !", 422)
    
    # Check if "app_input" is in the request
    if not request.form['app_input']:
        return abort("app_input is empty !", 422)
    
    app_input = request.form['app_input']             # Get app_input
    campaign_type = request.form['campaign_type']     # Get campaign type
    file_contents = request.files['file'].read()      # Get file contents
    
    files = [{"name": "app_path", "data": file_contents, "savedonstorage": 1}]
    executions = [{'idexecution': 'Simulation', 'name': 'Golden', 'hasfault': 0, 'ntargetruns': 5}]
    campaign_target = {'type': 'localhost', 'domain': None, 'username': None}
    fault_injector_target = {'type': 'localhost', 'domain': None, 'username': None}
    user_components = []
    parameters = [{'name': 'app_input', 'data': pickle.dumps(app_input)}, 
                  {'name': 'watchdog_dur', 'data': pickle.dumps(50000)}, 
                  {'name': 'fi_min', 'data': pickle.dumps(0)},              
                  {'name': 'fi_max', 'data': pickle.dumps(0)}]              
    dicio_configuration = {}
    if current_app.config['FRAMEWORK_DATA']["campaigns"]:
        for campaign in current_app.config['FRAMEWORK_DATA']["campaigns"]:
            if campaign["data"]["campaign_name"] == campaign_type:
                if "campaign_class_name" in campaign["data"] and "campaign_class_name" in campaign["data"] and "campaign_class_name" in campaign["data"]:
                    dicio_configuration["campaign_class_name"] = campaign["data"]["campaign_class_name"]
                    dicio_configuration["campaign_file_name"] = campaign["data"]["campaign_file_name"]
                    if "configuration" in campaign["data"]:
                        dicio_configuration["campaign_configuration"] = campaign["data"]["configuration"]
                else:
                    return abort('Could not verify campaign types!', 422)
    else:
        return abort("No data relative to components and/or campaigns!", 422)

    id = database.create_campaign("simulation_run",campaign_type,'',current_user["id"])

    campaign_database = {'idcampaign': id,
                        'type': campaign_type, 
                        'name': 'simulation_run', 
                        'state': 'waiting', 
                        'fipath': '', 
                        'csvfilename': f'{id}_simulation_run', 
                        'startdate': None, 
                        'enddate': None, 
                        'user_iduser': current_user["id"]}
    
    p = Process(target=main, args=(dicio_configuration, campaign_database, files, executions, campaign_target, fault_injector_target, parameters, user_components))
    p.start()
    p.join()
    
    campaign = database.get_basic_campaign(id, current_user["id"])
    
    if campaign["csvfilename"]:
        filepath = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "csv-files", campaign["csvfilename"]+".csv")
        statistics = utils.statistics_parser(filepath)
    print("sTATISTICS ----------------------------------------", statistics)
    if statistics:
        watchdog_dur = int(statistics['golden_run_total_duration'] * 2.5 * 1000)    # All Converted to milliseconds
        fi_min = int(statistics['golden_run_min_peak_duration'] * 0.2 * 1000)       
        fi_max = int(statistics['golden_run_max_peak_duration'] * 0.8 * 1000)       
        return make_response(jsonify({"message": "Simulation Run - Values Calculated [Reloading Page]", "watchdog_dur": watchdog_dur, "fi_min": fi_min, "fi_max": fi_max, "app_input": app_input}), 200)
    else:
        return abort("Simulation Run - Ended with Error", 400)
    

# Get and Copy Information about a single Campaign
@campaign.route('/campaigns/archive/<campaign_id>', methods=['PUT'])
@token_required
def archive_campaign(current_user, campaign_id):
        
        #Verify and validate campaign id
        if campaign_id is None:
            return abort("Malformed request syntax!", 400)
    
        value = database.verify_campaign_belong_user_returnable(campaign_id, current_user["id"])
        
        if not value:
            return abort("Campaign not reachable.", 422)
    
        info = database.archive_campaign(campaign_id, value[0]['archived'])
        
        if info and value[0]['archived'] == 0:
            return make_response(jsonify({"message": f"Campaign with ID: {campaign_id} and NAME: '{value[0]['name']}' Archived Successfully!"}), 200, {'Content-Type': 'application/json'})
        elif info and value[0]['archived'] == 1:
            return make_response(jsonify({"message": f"Campaign with ID: {campaign_id} and NAME: '{value[0]['name']}' Unarchived Successfully!"}), 200, {'Content-Type': 'application/json'})
        else:
            return abort("Failed to archive the campaign.", 422)