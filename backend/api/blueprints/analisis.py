from flask import Blueprint, request, make_response, jsonify, send_file
from api.core.abort import abort
import api.database as database
import api.core.parser as utils
from api.core.decorators import token_required
import os

analisis = Blueprint("analisis", __name__)
root_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

#STATISTICS
@analisis.route('/campaign/<campaign_id>/statistics', methods=['GET'])
@token_required
def get_campaign_statistics(current_user,campaign_id):

    campaign = database.get_basic_campaign(campaign_id, current_user["id"])
    
    if not campaign:
        return abort('The specified campaign cannot be accessed!', 422)

    if campaign["csvfilename"]:
        filepath = os.path.join(root_dir, "csvFiles", campaign["csvfilename"]+".csv")
        statistics = utils.statistics_parser(filepath)
    else:
        return abort('Could not retrieve csv filename!', 422)

    if statistics:
        response = { "campaign": campaign, "statistics" : statistics}
        return make_response(jsonify(response), 200, {'Content-Type': 'application/json'})
    else:
        response = { "campaign": campaign, "message" : "Could not retrieve statistics (file not found, no data, etc...)"}
        return make_response(jsonify(response), 422, {'Content-Type': 'application/json'})


#COLUMNS CSVFILE
@analisis.route('/campaign/<campaign_id>/columns', methods=['GET'])
@token_required
def get_csvfile_columns(current_user,campaign_id):

    campaign = database.get_basic_campaign(campaign_id, current_user["id"])
    
    if not campaign:
        return abort('The specified campaign cannot be accessed!', 422)

    if campaign["csvfilename"]:
        filepath = os.path.join(root_dir, "csvFiles", campaign["csvfilename"]+".csv")
        columns = utils.columns_parser(filepath)
    else:
        return abort('Could not retrieve csv filename!', 422)

    if columns:
        response = { "campaign": campaign, "columns" : columns}
        return make_response(jsonify(response), 200, {'Content-Type': 'application/json'})
    else:
        response = { "campaign": campaign, "message" : "Could not retrieve columns (file not found, no data, etc...)"}
        return make_response(jsonify(response), 422, {'Content-Type': 'application/json'})


#CHARTS
@analisis.route('/campaign/<campaign_id>/chart', methods=['POST'])
@token_required
def get_campaign_chart(current_user,campaign_id):

    data = request.get_json()

    #Verify if json has all parameters 
    if data is None or 'x_axis' not in data or 'y_axis' not in data:
        return abort("Malformed request syntax!", 400)

    campaign = database.get_basic_campaign(campaign_id, current_user["id"])
    
    if not campaign:
        return abort('The specified campaign cannot be accessed!', 422)

    if campaign["csvfilename"]:
        filepath = os.path.join(root_dir, "csvFiles", campaign["csvfilename"]+".csv")
        chartdata = utils.chart_parser(filepath, data["x_axis"], data["y_axis"])
    else:
        return abort('Could not retrieve csv filename!', 422)

    if chartdata:
        return make_response(jsonify(chartdata), 200, {'Content-Type': 'application/json'})
    
    return make_response({"message" : "Could not retrieve statistics (file not found, no data, etc...)"}, 422, {'Content-Type': 'application/json'})


#DOWNLOAD CSV DATA
@analisis.route("/campaign/<campaign_id>/download", methods = ['GET'])
@token_required
def download_csv(current_user, campaign_id):
    
    if campaign_id is None:
        return abort("Malformed request syntax!", 400)

    campaign = database.get_basic_campaign(campaign_id, current_user["id"])
    path_folder = os.path.join(root_dir, "csvFiles", campaign["csvfilename"]+".csv")

    if not os.path.exists(path_folder):
        return abort('Could not access file! (Campaign must be executed first)', 422)

    return send_file(path_folder,
                    mimetype='text/csv',
                    attachment_filename='data.csv',
                    as_attachment=True)