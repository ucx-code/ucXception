import os, json, re, csv
COMPONENS_DIRECTORY = ["campaigns","parsers","preprobes","postprobes","transformers","validators"]

#Return loaded data
def pre_load_data():
    returned_data = {}
    directory = "framework"

    for folder in COMPONENS_DIRECTORY:
        path_folder = os.path.join(directory, folder)
        returned_data[folder] = return_list_files(path_folder)

    return returned_data

#Return a list of json files
def return_list_files(directory):

    listObj = []
    #Get path for framework
    current_dir = os.path.abspath(os.path.dirname(__file__))
    root_dir = os.path.dirname(os.path.dirname(current_dir))
    mypath = os.path.join(root_dir, directory)
    
    #Get .json and .py files
    files_json = [f for f in os.listdir(mypath) if os.path.isfile(os.path.join(mypath, f)) if f.endswith(".json")]
    files_py = [f for f in os.listdir(mypath) if os.path.isfile(os.path.join(mypath, f)) if f.endswith(".py")]
    
    #Check if the .json has its .py counterpart
    for json_file in files_json:
        #Remove extension and  verify if exist in py list
        if any(os.path.splitext(json_file)[0] in os.path.splitext(py_file)[0] for py_file in files_py):

            path = os.path.join(mypath, json_file)
            data = read_json(path)
            #If does not occour error, function return data
            if data and ("campaign_name" in data and "campaign_file_name" in data and "campaign_class_name" in data) or ("component_name" in data and "component_file_name" in data and "component_class_name" in data):
                error = False
                for object in listObj:
                    if "campaign_name" in object["data"]:
                        if object["data"]["campaign_name"] == data["campaign_name"]:
                            print("Campaign already with that name!")
                            error = True
                    elif "component_name" in object["data"]:
                        if object["data"]["component_name"] == data["component_name"]:
                            print("Component already with that name!")
                            error = True
                
                if not error:
                    listObj.append({ "filename": path, "data": data})

    return listObj

#Read json files
def read_json(path):
    try:
        f = open(path)
        data = json.load(f)
        f.close()
    except:
        print("Error opening file")
        return {}

    return data

#Email message
def message_body_constructor(url, reset_token):
    message = " Dear, User \n To reset your password click on the following link: \n " + url + reset_token + " \n If you have not requested a password reset simply ignore this message. \n Sincerely \n ucXception Support Team"

    return message

#Create folder
def create_folder(path):
    try:
        os.mkdir(path)
    except FileExistsError:
        return False

    return True



#Validate inputs
def validate_email(email):
    #Email regex
    regex = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b'

    if re.fullmatch(regex,email):
        return True
    else:
        return False

def validate_password(password):
    #Email regex
    regex = r'^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)[a-zA-Z\d]{8,}$'

    if re.fullmatch(regex,password):
        return True
    else:
        return False

def validate_string(string):
    if(bool(re.match('^[a-zA-Z0-9_/:-]*$',string))==False):
        return False
    return True

def validate_string_number(string):
    if(bool(re.match('^[0-9]*$',string))==False):
        return False
    return True

def validate_integer(value):
    if isinstance(value, int):
        return True
    return False

def parameter_value_validation(parameter_value, type):
    if type == "integer":
        return int(parameter_value)
    elif type == "string":
        return parameter_value
    elif type == "boolean":
        if parameter_value.lower() == 'true':
            return True
        return False
    elif type == "multiple":
        if parameter_value:
            return parameter_value.split(",")
        return "error"
    else:
        return "error"
