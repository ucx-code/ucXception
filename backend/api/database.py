import sqlite3
from api.script_tables import return_script
import os
import errno
import pickle
import base64

DATABASE_NAME = "ucxception.db"
DIRECTORY = "database" 
COMPONENTS_DIRECTORY = ["campaigns","parsers","preprobes","postprobes","transformers","validators"]

#Get database connection
def get_db():
    try:
        basedir = os.path.abspath(os.path.dirname(__file__))
        path = os.path.join(basedir, DIRECTORY, DATABASE_NAME)
        conn = sqlite3.connect(path)
    except sqlite3.Error:
        print("Error in connection")
    return conn

#Create database
def create_tables():
    #Create folder database
    try:
        #Get absolute path and create new path with new directory
        basedir = os.path.abspath(os.path.dirname(__file__))
        path = os.path.join(basedir, DIRECTORY)
        #Create folder
        os.mkdir(path)
        print("Folder {} created with sucess".format(DIRECTORY))
    except OSError as e:
        #Verify if already exists
        if e.errno != errno.EEXIST:
            raise 
        else:
            print("Folder {} already exists".format(path))

    try:
        conn = get_db()
        cursor = conn.cursor()
        tables = return_script()
        for table in tables:
            cursor.execute(table)
        print("Tables creation success")
    except:
        print("Tables creation failed")
    finally:
        conn.close()

#Retrieve a dicionary
def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d

#GENERIC FUNCTIONS
def generic_access(statement, parameters_list):
    try:
        conn = get_db()
        cursor = conn.cursor()
        cursor.execute(statement, parameters_list)
        conn.commit()
    except:
        conn.rollback()
        return False
    finally:
        conn.close()
    return True

def retrieve_generic_access(statement, parameters_list, isfetchAll):
    try:
        conn = get_db()
        conn.row_factory = dict_factory
        cursor = conn.cursor()
        cursor.execute(statement, parameters_list)
        conn.commit()
    except:
        conn.rollback()
        return False
    finally:
        if isfetchAll:
            result = cursor.fetchall()
        else:
            result = cursor.fetchone()
        conn.close()
    return result


# Validation and verification
def verify_email(email):
    statement = "SELECT public_id, email FROM user WHERE email = ?;"
    parameters_list = [email]
    return retrieve_generic_access(statement, parameters_list, False)

def verify_campaign_belong_user(campaign_id, user_id):
    statement = "SELECT 1 FROM campaign  WHERE idcampaign = ? AND user_iduser = ?;"
    parameters_list = [campaign_id, user_id]
    return generic_access(statement, parameters_list)

def verify_campaign_belong_user_returnable(campaign_id, user_id):
    statement = "SELECT idcampaign, type, name, csvfilename, campaigntarget, faultinjectortarget FROM campaign WHERE idcampaign = ? AND user_iduser = ?;"
    parameters_list = [campaign_id, user_id]
    return retrieve_generic_access(statement, parameters_list, True)

#USER
def create_user(public_id, username, email, hashed_password):
    statement = "INSERT INTO user(public_id, username, email, password) VALUES (?, ?, ?, ?);"
    parameters_list = [public_id, username, email, hashed_password]
    return generic_access(statement, parameters_list)

def get_user_from_email(email):
    statement = "SELECT public_id, password, email, username FROM user WHERE email = ?;"
    parameters_list = [email]
    return retrieve_generic_access(statement, parameters_list, False)

def get_user_from_public_id(public_id):
    statement = "SELECT iduser, public_id, email, username FROM user WHERE public_id = ?;"
    parameters_list = [public_id]
    return retrieve_generic_access(statement, parameters_list, False)



#RESET PASSWORD
def create_reset_password(expiration_time, hashed_token, user_publicid):
    statement = "INSERT INTO reset_password(expiration_time, hashed_token, user_iduser) VALUES (?, ?, ?);"
    parameters_list = [expiration_time, hashed_token, user_publicid]
    return generic_access(statement, parameters_list)

def verify_reset_token(hashed_token):
    statement = "SELECT user_iduser FROM reset_password WHERE hashed_token = ?;"
    parameters_list = [hashed_token]
    return retrieve_generic_access(statement, parameters_list, False)

def change_password(user_id, hased_password):
    statement = "UPDATE user SET password = ? WHERE public_id = ?;"
    parameters_list = [hased_password, user_id]
    return generic_access(statement, parameters_list)



#CAMPAIGNS
def create_campaign(project_name, campaign_type, fipath, iduser):
    try:
        conn = get_db()
        cursor = conn.cursor()
        
        statement = "SELECT idcampaign FROM campaign WHERE idcampaign=(SELECT max(idcampaign) FROM campaign);"
        cursor.execute(statement)
        result = cursor.fetchone()
        if result == None:
            csvfilename = "1_"+project_name
        else:
            csvfilename = str(result[0]+1)+"_"+project_name

        statement = "INSERT INTO campaign(name,type,state,fipath,csvfilename,user_iduser) VALUES(?,?,?,?,?,?);"
        cursor.execute(statement, [project_name,campaign_type,"waiting",fipath,csvfilename,iduser])
        conn.commit()
    except:
        conn.rollback()
        return False
    finally:
        idcampaign = cursor.lastrowid
        conn.close()
    return idcampaign

#Gets Total number of campaigns
def get_total(iduser,searchbar):
    parameters_list = [iduser]
    statement = "SELECT COUNT(DISTINCT idcampaign) AS Total FROM campaign WHERE user_iduser = ? AND state != 'waiting' "
    if searchbar:
        statement += "AND (name LIKE ? OR startdate LIKE ?)"
        parameters_list.append("%"+searchbar+"%")
        parameters_list.append("%"+searchbar+"%")
    return retrieve_generic_access(statement, parameters_list, True)

#Gets Campaigns Filtered and Gets SearchBar
def get_campaigns(iduser, searchbar, currentpage, numberelements, toggleValue):
    
    parameters_list = [iduser]
    statement = "SELECT idcampaign FROM campaign WHERE user_iduser = ? AND state != 'waiting' "
    
    # SearchBar
    if searchbar:
        statement += "AND (name LIKE ? OR startdate LIKE ?) "
        parameters_list.append("%"+searchbar+"%")
        parameters_list.append("%"+searchbar+"%")

    # Sorting Depending on the toggleValue (no "if" for toggleValue == '0' because it is the default sort)
    if toggleValue == '1': statement += " ORDER BY idcampaign DESC"
    elif toggleValue == '2': statement += " ORDER BY UPPER(name)"
    elif toggleValue == '3': statement += " ORDER BY UPPER(name) DESC"

    # We get all campaigns that match the search criteria and sort them if necessary
    all_campaigns = retrieve_generic_access(statement, parameters_list, True)

    # Pagination
    if currentpage and numberelements:
        start = (int(currentpage) - 1) * int(numberelements)
        end = start + int(numberelements)
        paginated_campaigns = all_campaigns[start:end]
    else:
        paginated_campaigns = all_campaigns

    ids = [campaign['idcampaign'] for campaign in paginated_campaigns]
    statement = "SELECT campaign.idcampaign, campaign.type, campaign.name AS campaignName, campaign.state, campaign.startdate, execution.name AS executionName, execution.hasfault, execution.ntargetruns, execution.ncurrentruns FROM campaign JOIN execution ON campaign.idcampaign = execution.campaign_idcampaign WHERE campaign.user_iduser = ? AND campaign.state != 'waiting' AND "
    if len(ids) != 1:
        statement += f"campaign.idcampaign IN {tuple(ids)}"
    else:
        statement += f"campaign.idcampaign = {ids[0]}"

    # Apply sorting to the final query as well so that the current page is also sorted
    if toggleValue == '1': statement += " ORDER BY campaign.idcampaign DESC"            # Newest to oldest
    elif toggleValue == '2': statement += " ORDER BY UPPER(campaign.name)"              # A - Z
    elif toggleValue == '3': statement += " ORDER BY UPPER(campaign.name) DESC"         # Z - A
    parameters_list = [iduser]
    
    return retrieve_generic_access(statement, parameters_list, True)

def get_campaign(idcampaign):
    statement =  "SELECT * FROM campaign WHERE idcampaign = ?;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, False)

def get_basic_campaign(idcampaign, userid):
    statement =  "SELECT type, name,  state, csvfilename, startdate, enddate FROM campaign WHERE idcampaign = ? AND user_iduser = ?;"
    parameters_list = [idcampaign, userid]
    return retrieve_generic_access(statement, parameters_list, False)

def get_campaign_target(idcampaign):
    statement =  "SELECT host.type, host.domain, host.username FROM campaign, host WHERE campaign.idcampaign = ? AND host.idhost = campaign.campaigntarget;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, False)

def get_campaign_fault_injector_target(idcampaign):
    statement =  "SELECT host.type, host.domain, host.username FROM campaign, host WHERE campaign.idcampaign = ? AND host.idhost = campaign.faultinjectortarget;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, False)

def verify_campaign_id(idcampaign):
    statement =  "SELECT campaign.name FROM campaign WHERE idcampaign = ?;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, False)

def update_host_campaign(campaigntarget,faultinjectortarget,idcampaign):
    statement = "UPDATE campaign SET campaigntarget = ?, faultinjectortarget = ? WHERE idcampaign = ?;"
    parameters_list = [campaigntarget,faultinjectortarget,idcampaign]
    return generic_access(statement, parameters_list)

def delete_campaign(idcampaign):
    statement = "DELETE FROM campaign WHERE idcampaign = ?;"
    parameters_list = [idcampaign]
    return generic_access(statement, parameters_list)

def update_begin_campaign_state(idcampaign, state, datetime):
    statement = "UPDATE campaign SET state = ?, startdate = ? WHERE idcampaign = ?;"
    parameters_list = [state, datetime, idcampaign]
    return generic_access(statement, parameters_list)

def update_end_campaign_state(idcampaign, state, datetime):
    statement = "UPDATE campaign SET state = ?, enddate = ? WHERE idcampaign = ?;"
    parameters_list = [state, datetime, idcampaign]
    return generic_access(statement, parameters_list)

#FILES
def create_file(name, data, savedonstorage, idcampaign):
    statement = "INSERT INTO file(name, data, savedonstorage, campaign_idcampaign) VALUES (?,?,?,?);"
    parameters_list = [name, data, savedonstorage, idcampaign]
    return generic_access(statement, parameters_list)

def get_files(idcampaign):
    statement =  "SELECT name, data, savedonstorage FROM file WHERE campaign_idcampaign = ? AND name='app_path';"
    parameters_list = [idcampaign]
    temp = retrieve_generic_access(statement, parameters_list, True)
    if (temp is not None):
        return temp
    else:
        statement =  "SELECT name, data, savedonstorage FROM file WHERE campaign_idcampaign = ?;"
        parameters_list = [idcampaign]
        return retrieve_generic_access(statement, parameters_list, True)



#CAMPAIGN PARAMETERS
def create_parameter(name,blob,idcampaign):
    statement = "INSERT INTO campaign_parameter(name,data,campaign_idcampaign) VALUES (?,?,?);"
    parameters_list = [name,blob,idcampaign]
    return generic_access(statement, parameters_list)

def get_campaign_parameters(idcampaign):
    statement = "SELECT name, data FROM campaign_parameter WHERE campaign_idcampaign = ?;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, True)

#COMPONENT PARAMETERS
def create_component_parameter(name,blob,idcomponent):
    statement = "INSERT INTO component_parameter(name,data,components_idcomponent) VALUES (?,?,?);"
    parameters_list = [name,blob,idcomponent]
    return generic_access(statement, parameters_list)

def get_component_parameters(idcomponent):
    statement = "SELECT name, data FROM component_parameter WHERE components_idcomponent = ?;"
    parameters_list = [idcomponent]
    return retrieve_generic_access(statement, parameters_list, True)




#COMPONENTS TYPES
def create_component_types():
    for type in COMPONENTS_DIRECTORY:
        statement = "INSERT INTO component_type(name) VALUES (?);"
        parameters_list = [type]
        generic_access(statement, parameters_list)

def get_component_type_id(type):
    statement = "SELECT idcomponenttype FROM component_type WHERE name = ?;"
    parameters_list = [type]
    return retrieve_generic_access(statement, parameters_list, False)
    
    



#COMPONENTS
def create_component(name,type,idcomponenttype,idcampaign):
    try:
        conn = get_db()
        cursor = conn.cursor()
        statement = "INSERT INTO components(name,type,component_type_idcomponenttype,campaign_idcampaign) VALUES (?,?,?,?);"
        cursor.execute(statement, [name,type,idcomponenttype,idcampaign]) 
        conn.commit()
    except:
        conn.rollback()
        return False
    finally:
        idcomponent = cursor.lastrowid
        conn.close()
    return idcomponent

def get_to_display_components(idcampaign):
    statement = "SELECT idcomponent, name, type FROM components WHERE campaign_idcampaign = ?;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, True)

def get_components(idcampaign):
    statement = "SELECT components.idcomponent AS idcomponent, components.name AS component_name, components.type AS component_type, component_type.name AS component_type_name FROM components, component_type WHERE campaign_idcampaign = ? AND idcomponenttype = component_type_idcomponenttype;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, True)

def delete_component(idcomponent):
    statement = "DELETE FROM components WHERE idcomponent = ?;"
    parameters_list = [idcomponent]
    return generic_access(statement, parameters_list)

def create_component_associated_component(idcomponent, idcomponent_associated):
    statement = "INSERT INTO components_components (components_idcomponent, components_idcomponent1) VALUES (?,?);"
    parameters_list = [idcomponent,idcomponent_associated]
    return generic_access(statement, parameters_list)

def get_component_associated_component(idcomponent):
    statement = "SELECT components_idcomponent, components_idcomponent1 FROM components_components WHERE components_idcomponent = ?;"
    parameters_list = [idcomponent]
    return retrieve_generic_access(statement, parameters_list, False)



#EXECUTIONS
def create_execution(idcampaign,name,type,nTargetRuns):
    statement = "INSERT INTO execution (name, hasfault, ntargetruns, ncurrentruns, campaign_idcampaign) VALUES (?,?,?,?,?);"
    parameters_list = [name,type,nTargetRuns,0,idcampaign]
    return generic_access(statement, parameters_list)

def get_executions(idcampaign):
    statement =  "SELECT idexecution, name, hasfault, ntargetruns FROM execution WHERE campaign_idcampaign = ?;"
    parameters_list = [idcampaign]
    return retrieve_generic_access(statement, parameters_list, True)

def delete_execution(idcampaign,idexecution):
    statement = "DELETE FROM execution WHERE campaign_idcampaign = ? AND idexecution = ?;"
    parameters_list = [idcampaign,idexecution]
    return generic_access(statement, parameters_list)

def update_execution_current_run(campaignid, name, ncurrentruns, input):
    statement = "UPDATE execution SET ncurrentruns = ? WHERE campaign_idcampaign = ? AND name = ? AND hasfault = ?;"
    parameters_list = [ncurrentruns, campaignid, name, input]
    return generic_access(statement, parameters_list)




#HOSTS
def create_host(idcampaign,type,domain,username):
    try:
        conn = get_db()
        cursor = conn.cursor()
        statement = "INSERT INTO host (type, domain, username, campaign_idcampaign) VALUES (?,?,?,?);"
        cursor.execute(statement,  [type,domain,username,idcampaign]) 
        conn.commit()
    except:
        conn.rollback()
        return False
    finally:
        idhost = cursor.lastrowid
        conn.close()
    return idhost

def get_hosts(idcampaign, user_id):
    statement =  "SELECT campaign.campaigntarget AS campaign_target, campaign.faultinjectortarget AS campaign_fault_injector_target,  host.idhost, host.type, host.domain, host.username FROM campaign, host WHERE campaign.idcampaign = ? AND campaign.user_iduser = ? AND host.campaign_idcampaign = campaign.idcampaign;"
    parameters_list = [idcampaign, user_id]
    return retrieve_generic_access(statement, parameters_list, True)

def delete_host(idcampaign,idhost):
    statement = "DELETE FROM host WHERE campaign_idcampaign = ? AND idhost = ?;"
    parameters_list = [idcampaign,idhost]
    return generic_access(statement, parameters_list)

def create_component_host (idcomponent, idhost):
    statement = "INSERT INTO components_host(components_idcomponent, host_idhost) VALUES (?,?);"
    parameters_list = [idcomponent,idhost]
    return generic_access(statement, parameters_list)

def get_component_host(idcomponent):
    statement = "SELECT host.type, host.domain, host.username FROM components_host, host WHERE components_host.components_idcomponent = ? AND host.idhost = components_host.host_idhost;"
    parameters_list = [idcomponent]
    return retrieve_generic_access(statement, parameters_list, False)

# Function that will copy all information of a single campaign 
def COPY_CAMPAIGN(idcampaign,userid):
    campaign_info = get_campaign(idcampaign)
    executions = get_executions(idcampaign)
    parametros = get_campaign_parameters(idcampaign)
    hosts = get_hosts(idcampaign,userid["id"])
    campaign_data = {
        'Campaign_Type': campaign_info['type'],
        'Project_Name': campaign_info['name'],
        'Fault_Injector_Path': campaign_info['fipath'],
        'app_input': pickle.loads(parametros[0]['data']),
        'fi_max': pickle.loads(parametros[3]['data']),
        'fi_min': pickle.loads(parametros[2]['data']),
        'watchdog_dur': pickle.loads(parametros[1]['data']),
        'Executions': executions,
        'Hosts': hosts,
        'id': int(idcampaign)
    }
    return campaign_data

def database_main():
    create_tables()
    create_component_types()