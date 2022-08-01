from flask import Blueprint, request, make_response, jsonify
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
from api.core.decorators import token_required

host = Blueprint("host", __name__)

#CREATE HOST
@host.route('/campaigns/hosts', methods=['POST'])
@token_required
def create_host(current_user):

    data = request.get_json()
    
    #Verify if json has all parameters 
    if data is None or 'campaign_id' not in data or 'type' not in data or 'campaign_target' not in data or 'fault_injector_target' not in data:
        return abort("Malformed request syntax!", 400)

    #Validate campaign id
    if not utils.validate_integer(data['campaign_id']) :
        return abort('Parameters must match the required type', 400)

    value = database.verify_campaign_belong_user_returnable(data['campaign_id'], current_user["id"])

    if not value:
        return abort("Campaign not reachable.", 422)

    #Verify type choosed
    if data['type'] != "localhost" and data['type'] != "remote":
        return abort('Type must be localhost or remote!', 422)

    if data['type'] == 'remote':
        if 'domain' not in data or 'username' not in data:
            return abort('Missing arguments for type remote!', 400)

        if not utils.validate_string(data['username']):
            return abort("Parameters can not be empty values and can not have special characters!", 422)
    
    #Insert in database
    idhost = database.create_host(data['campaign_id'],data['type'],data['domain'],data['username'])

    if not idhost:
        return abort('Host could not be created!', 422)
    
    #Verify targets to update campaign table
    if data['campaign_target']:
        campaign_target = idhost
    else:
        campaign_target = value[0]["campaigntarget"]

    if data['fault_injector_target']:
        fault_injector_target = idhost
    else:
        fault_injector_target = value[0]["faultinjectortarget"]

    #Update campaign host in database
    if not database.update_host_campaign(campaign_target, fault_injector_target, data['campaign_id']):
        return abort('Could not update campaign host!', 422)
    
    return make_response({"message": "Host created with success!"}, 202, {'Content-Type': 'application/json'})

#GET ALL HOSTS
@host.route('/campaigns/hosts/<campaign_id>', methods=['GET'])
@token_required
def get_hosts(current_user, campaign_id):

    if campaign_id is None:
        return abort("Malformed request syntax!", 400)

    hosts = database.get_hosts(campaign_id, current_user["id"])

    if hosts == False:
        return abort('Unexpected error!', 500)

    return make_response(jsonify(hosts), 200, {'Content-Type': 'application/json'})

#DELETE HOST
@host.route('/campaigns/hosts/<campaign_id>', methods=['DELETE'])
@token_required
def delete_host(current_user, campaign_id):

    #Verify and valiudate campaign id
    if campaign_id is None:
        return abort("Malformed request syntax!", 400)

    value = database.verify_campaign_belong_user_returnable(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable.", 422)

    data = request.get_json()

    #Verify data
    if data is None or 'host_id' not in data:
        return abort("Malformed request syntax!", 400)

    if not utils.validate_integer(data['host_id']):
        return abort('Parameters must match the required type', 400)

    #Validate data
    if value[0]['campaigntarget'] == data['host_id']: 
        campaign_target = None
    else:
        campaign_target = value[0]['campaigntarget']
    
    if value[0]['faultinjectortarget'] == data['host_id']: 
        fault_injector_target = None
    else:
        fault_injector_target = value[0]['faultinjectortarget']

    #Update hosts
    if not database.update_host_campaign(campaign_target, fault_injector_target, campaign_id):
        return abort('Could not update campaign host!', 500)
    
    #Delete host
    if not database.delete_host(campaign_id, data['host_id']):
        return abort('Could not delete host!', 500)

    return make_response({'message': 'Host deleted successfully!'}, 200, {'Content-Type': 'application/json'})