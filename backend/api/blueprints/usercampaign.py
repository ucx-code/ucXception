from flask import Blueprint, request, make_response, jsonify, current_app
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
from api.core.decorators import token_required
import pickle

user_campaign = Blueprint("user_campaign", __name__)

# #CREATE CAMPAIGN
@user_campaign.route('/campaigns', methods=['POST'])
@token_required
def create_campaign(current_user):

    if 'project_name' not in request.form or 'campaign_type' not in request.form: 
        return abort("Malformed request syntax!", 400)

    project_name = request.form.get('project_name')
    campaign_type = request.form.get('campaign_type')
    fipath = request.form.get('fipath')

    #Verify if project name doesnt have special characters
    if not utils.validate_string(project_name):
        return abort('Parameters must match the required type', 400)
    
    exists = False
    #Verify if campaign type exists
    for campaign in current_app.config['FRAMEWORK_DATA']["campaigns"]:
        #If campaign name is equals to the one choosed
        if campaign["data"]["campaign_name"] == campaign_type:
            exists = True

    if not exists:
        return abort("Campaign type does not exist!",422)

    id_campaign = database.create_campaign(project_name, campaign_type, fipath, current_user["id"])

    #Verify if campaign is created
    if not id_campaign:
        return abort("Could not create base campaign!", 500)

    #Campaign from global data
    for campaign in current_app.config['FRAMEWORK_DATA']["campaigns"]:
        #If campaign name is equals to the one choosed
        if campaign["data"]["campaign_name"] == campaign_type:
            #RESOLVE FILE ATTRIBUTION
            for key, value in campaign["data"]["configuration"].items():
                
                require = value["require"]

                if key in request.files and request.files[key]:
                    file_code = request.files[key]

                    # Verify and validate file name and extension
                    try:
                        (filename, extension) = file_code.filename.split('.')
                    except ValueError:
                        return abort("File must have name and/or extension.",422)

                    if not utils.validate_string(filename):
                        return abort('Parameters must match the required type', 400)
                    
                    if extension == "zip":
                        #ZIP file code in bytes
                        filecode_blob = pickle.dumps(file_code)

                        if not database.create_file(key, filecode_blob, 1, id_campaign):
                            return abort("Error saving file!", 500)
                    else:
                        return abort("File must be zip!",422)
                elif key in request.form and request.form.get(key):
                        path_blob = pickle.dumps(request.form.get(key))
                            
                        if not database.create_file(key, path_blob, 0, id_campaign):
                            return abort("Error saving path file!", 500)
                else:
                    if require:
                        return abort("Must upload a file or give a path to "+key+" field!", 422)


            #RESOLVE PARAMETERS ATTRIBUTION
            #Iterate parameters from global data
            if "parameters" in campaign["data"]:
                for parameter in campaign["data"]["parameters"]:
                    #If parameter not in request form
                    if parameter not in request.form:
                        #database.delete_campaign(id_campaign)
                        return abort("Malformed request syntax!", 400)
                    
                    value = utils.parameter_value_validation(request.form.get(parameter),campaign["data"]["parameters"][parameter]['type'])

                    if value == "error":
                        return abort("An unexpected error occurs when checking the parameters (empty parameters, out of bound, etc..).",422)

                    byte_data = pickle.dumps(value)
                    if not database.create_parameter(parameter,byte_data,id_campaign):
                        return abort("Error saving data!", 500)
    
    return make_response({"campaign-id": id_campaign, "message": "Campaign created with success"}, 202, {'Content-Type': 'application/json'})


#GET CAMPAINGS
@user_campaign.route('/campaigns', methods=['GET'])
@token_required
def get_campaign_user(current_user):

    args = request.args

    #Search bar text
    if args.get("searchbar") and not utils.validate_string(args.get("searchbar")):
        return abort('Parameters must match the required type', 400)

    #Cmapaign state
    if args.get("campaign_state") and not utils.validate_string(args.get("campaign_state")):
        return abort('Parameters must match the required type', 400)

    #Actual page
    if args.get("page") and not utils.validate_string_number(args.get("page")):
        return abort('Parameters must match the required type', 400)

    #Size of each page
    if args.get("page_size") and not utils.validate_string_number(args.get("page_size")):
        return abort('Parameters must match the required type', 400)
    
    #Gets campaigns filtered
    campaings = database.get_campaigns(True, current_user["id"], args.get("searchbar"), args.get("campaign_state"), args.get("page") , args.get("page_size"))
    #Total number of campaigns
    total = database.get_campaigns(False, current_user["id"], args.get("searchbar"), args.get("campaign_state"), args.get("page") , args.get("page_size"))

    if campaings == False or total == False:
        return abort('Unexpected error!', 500)

    response = make_response(jsonify(campaings), 200, {'Content-Type': 'application/json'})
    response.headers['X-Total-Count'] = total[0]["Total"]
    response.headers['Access-Control-Expose-Headers'] = 'X-Total-Count'

    return response