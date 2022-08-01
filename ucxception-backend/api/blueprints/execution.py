from flask import Blueprint, request, make_response, jsonify
from api.core.abort import abort
import api.database as database
import api.core.utils as utils
from api.core.decorators import token_required

execution = Blueprint("execution", __name__)

#CREATE EXECUTION
@execution.route('/campaigns/executions', methods=['POST'])
@token_required
def create_execution(current_user):

    data = request.get_json()
    
    #Verify if json has all parameters 
    if data is None or 'campaign_id' not in data or 'type' not in data or 'name' not in data or 'n_target_runs' not in data:
        return abort("Malformed request syntax!", 400)
    
    #Validate parameters according to is type
    if not utils.validate_integer(data['campaign_id']) or not isinstance(data['type'], bool) or not utils.validate_string(data['name']) or not utils.validate_integer(data['n_target_runs']):
        return abort('Parameters must match the required type', 400)

    #Insert in database
    if not database.create_execution(data['campaign_id'],data['name'],data['type'],data['n_target_runs']):
        return abort('Execution could not be created!', 422)
    
    return make_response({"message": "Execution created with success"}, 202, {'Content-Type': 'application/json'})

#GET ALL EXECUTIONS
@execution.route('/campaigns/executions/<campaign_id>', methods=['GET'])
@token_required
def get_execution(current_user, campaign_id):

    if campaign_id is None:
        return abort("Malformed request syntax!", 400)
    
    value = database.verify_campaign_belong_user(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable", 422)

    executions = database.get_executions(campaign_id)

    if executions == False:
        return abort('Unexpected error!', 500)

    return make_response(jsonify(executions), 200, {'Content-Type': 'application/json'})

#DELETE EXECUTION
@execution.route('/campaigns/executions/<campaign_id>', methods=['DELETE'])
@token_required
def delete_execution(current_user, campaign_id):

    if campaign_id is None:
        return abort("Malformed request syntax!", 400)

    value = database.verify_campaign_belong_user(campaign_id, current_user["id"])

    if not value:
        return abort("Campaign not reachable", 422)

    data = request.get_json()

    if data is None or 'execution_id' not in data:
        return abort("Malformed request syntax!", 400)

    if not utils.validate_integer(data['execution_id']):
        return abort('Execution id must be integer!', 500)

    if not database.delete_execution(campaign_id, data['execution_id']):
        return abort('Could not delete execution!', 500)

    return make_response({'message': 'Execution deleted successfully!'}, 200, {'Content-Type': 'application/json'})