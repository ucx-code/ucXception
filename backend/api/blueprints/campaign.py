from flask import Blueprint, make_response, jsonify, current_app
from api.core.abort import abort
from api.core.decorators import token_required
import api.database as database

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