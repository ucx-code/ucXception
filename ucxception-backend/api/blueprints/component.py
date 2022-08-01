from flask import Blueprint, request, make_response, jsonify, current_app
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
from api.core.decorators import token_required
import pickle

component = Blueprint("component", __name__)
    
#CREATE COMPONENT
@component.route('/components', methods=['POST'])
@token_required
def create_components(current_user):
    
    data = request.form

    #Verify if is not none and if contains email and password
    if data is None or 'component_name' not in data or 'component_type' not in data or 'campaign_id' not in data or 'name' not in data:
        return abort("Malformed request syntax!", 400)

    id_campaign = data.get('campaign_id')

    if not database.verify_campaign_id(id_campaign):
        return abort('An unexpected validation error has occurred!', 400)

    component = database.get_component_type_id(data.get("component_type"))

    if not component:
        return abort('An unexpected verification error has occurred!', 400)

    idcomponent = database.create_component(data.get("name"),data.get("component_name"),component["idcomponenttype"],id_campaign)
    
    if not idcomponent:
        return abort('An unexpected error has occurred while creating component!', 422)

    if "associated_component" in data:
        if utils.validate_string_number(data.get("associated_component")):
            if not database.create_component_associated_component(idcomponent,int(data.get("associated_component"))):
                database.delete_component(idcomponent)
                return abort("Error creating associated component!",  422)
        else:
            database.delete_component(idcomponent)
            return abort("Error creating associated component!",  422)

    if current_app.config['FRAMEWORK_DATA']:
        #Component from global data
        for component in current_app.config['FRAMEWORK_DATA'][data["component_type"]]:
            #If campaign name is equals to the one choosed
            if component["data"]["component_name"] == data["component_name"]:
                value = None

                #Iterate parameters from global data
                if "parameters" in component["data"]:
                    for parameter in component["data"]["parameters"]:
                        #If parameter not in request form
                        if component["data"]["parameters"][parameter]["type"] == "file":
                            file_code = request.files[parameter]
                            if file_code.filename == '':
                                return abort("No file uploaded!", 400)
                            
                            if not utils.validate_string(file_code.filename.split('.')[0]):
                                return abort("File name can not be empty and can not have special characters!",422)
                        
                            file_extension = file_code.filename.split('.')[1]

                            if component["data"]["parameters"][parameter]["accept"].split('.')[1] == file_extension:
                                value = file_code
                            else:
                                return abort("Extension file must match specifications!", 422)
                        else:
                            if parameter not in data:
                                database.delete_component(idcomponent)
                                return abort("Missing parameter "+ parameter + "!", 422)

                            value = utils.parameter_value_validation(data.get(parameter),component["data"]["parameters"][parameter]['type'])

                        if value == "error" or value == None:
                            database.delete_component(idcomponent)
                            return abort("An unexpected error occurs when checking the parameters (empty parameters, out of bound, etc..)", 422)

                        byte_data = pickle.dumps(value)
                        if not database.create_component_parameter(parameter,byte_data,idcomponent):
                            database.delete_component(idcomponent)
                            return abort("Error inserting parameter in database",  422)

                if "target" in component["data"]:
                    if utils.validate_string_number(data.get("target")):
                        if not database.create_component_host(idcomponent,int(data.get("target"))):
                            database.delete_component(idcomponent)
                            return abort("Error creating host for component!",  422)
                    else:
                        database.delete_component(idcomponent)
                        return abort("Error creating host for component!",  422)
    else:
        return abort("No data relative to components and/or campaigns!", 422)

    return make_response({"message": "Component created with success"}, 200, {'Content-Type': 'application/json'})

#GET COMPONENT
@component.route('/components/user/<campaign_id>', methods=['GET'])
@token_required
def get_components_user(current_user,campaign_id):

    value = database.verify_campaign_belong_user(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable", 422)

    components = database.get_to_display_components(campaign_id)

    if components == False:
        return abort('Unexpected error occur while retrieving components!', 422)

    return make_response(jsonify(components), 200, {'Content-Type': 'application/json'})

#GET COMPONENT BASED ON COMPONENTS ALLOWED
@component.route('/components/<component_type>/<component_choice>/<campaign_id>', methods=['GET'])
@token_required
def get_components_allowed_user(current_user, component_type, component_choice, campaign_id):

    value = database.verify_campaign_belong_user(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable", 422)

    components = database.get_to_display_components(campaign_id)

    if components == False:
        return abort('Unexpected error occur while retrieving components!', 422)

    all_information = current_app.config['FRAMEWORK_DATA']
    all_component_imformation = all_information[component_type]

    retrieve_data = []
    help_break = False

    for component in all_component_imformation:
        if "component_name" in component["data"]:
            if component["data"]["component_name"] == component_choice:

                #Verify if component will need others compoents
                if "components_allowed" in component["data"] and "values" in component["data"]["components_allowed"] and len(component["data"]["components_allowed"]["values"]) > 0:
                    for value in component["data"]["components_allowed"]["values"]:
                        if "component_type" in value and "filename" in value:
                            if value["component_type"] in all_information:
                                for associated_component in all_information[value["component_type"]]:
                                    if value["filename"] == associated_component["data"]["component_file_name"]:
                                        for component_db in components:
                                            if component_db["type"] == associated_component["data"]["component_name"]:
                                                retrieve_data.append({ "label": component_db["name"], "value": component_db["idcomponent"] })
                                        break
                else:
                    return abort("An unexpected error has occurred in component " + component_choice, 422)
            
    if retrieve_data:
        return make_response(jsonify(retrieve_data), 200, {'Content-Type': 'application/json'})
    else:
        return abort("Make sure you have created the necessary components to use this probe.", 422)