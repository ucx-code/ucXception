# Local framework imports
from framework.conf.current import *
import framework.core.io as io
from framework.parsers.info_parser import *
from framework.core.ssh_config import *
from framework.transformers.sar_to_csv import Sar_to_CSV
import api.database as database

# General imports
import time, signal, logging,sys, os, pickle, imp, zipfile, re, shutil, datetime

logger = logging.getLogger(__name__)

#Validate file name
def validate_file(regex, filename):
	if(bool(re.match(regex,filename))==False):
		return False
	return True


#Extract zip file
def read_extract_zip(campaign_path, files, configuration, class_):
	#Create folder to store zip and files
	try:
		os.mkdir(campaign_path)
	except FileExistsError:
		logger.warning("Folder with that name already created")

	#Name for zip file
	for upload_file in files:
		if upload_file["name"] in configuration and hasattr(class_, upload_file["name"]):
			if upload_file["savedonstorage"]:
				#root_dir/framework/test/campaignid_campaignname/campaignid_campaignname.zip
				zip = os.path.join(campaign_path, upload_file["name"]) +".zip"
				
				#Read data to zip file
				with open(zip, "wb") as output_file:
					output_file.write(upload_file["data"])
			
				#Extract content inside zip file
				try:
					with zipfile.ZipFile(zip, 'r') as zip_ref:
						for file in zip_ref.filelist:
							name = file.filename
							perm = ((file.external_attr >> 16) & 0o777)
							if name.endswith("/"):
								outfolder = os.path.join(campaign_path, name)
								os.mkdir(outfolder, perm)
								os.chmod(outfolder, 0o777)
							else:
								outfile = os.path.join(campaign_path, name)
								if "regex" in configuration[upload_file["name"]]:
									if(validate_file(configuration[upload_file["name"]]["regex"], name)):
										setattr(class_, upload_file["name"], str(outfile))
							
								fh = os.open(outfile, os.O_CREAT | os.O_WRONLY, perm)
								os.write(fh, zip_ref.read(name))
								os.close(fh)
								os.chmod(outfile, 0o777)

				except zipfile.BadZipFile as error:
					logger.error(error)
			else:
				filepath_str = pickle.loads(upload_file["data"])
				setattr(class_, upload_file["name"], str(filepath_str))
						

#Set target
def set_target(class_, host):
	if hasattr(class_, "target"):
		if host["type"] == "remote":
			setattr(class_, "target", ssh(host["domain"], host["username"]))
		else:
			setattr(class_, "target", "localhost")	


#Set parameters
def set_parameters(class_, parameters):
	for parameter in parameters:
		data = pickle.loads(parameter['data'])

		if hasattr(class_, parameter['name']):
			setattr(class_, parameter['name'], data)


#Set executions
def set_executions(class_, executions):
	campaigns = []
	for execution in executions:
		hasfault = False
		if execution["hasfault"] == 1:
			hasfault = True

		aux = (execution["name"], execution["ntargetruns"], (hasfault,))
		campaigns.append(aux)

	if hasattr(class_, "campaigns") and len(campaigns) != 0:
		setattr(class_, "campaigns", tuple(campaigns))


#Set components
def set_component_probes(class_, component, component_type, component_class, target):
	component_aux = (component_class(), target,)
	parameter_aux = ()
	if "parameters" in component and component["parameters"] and len(component["parameters"]) > 0 and "constructor" in component:
		for element in component["constructor"]:
			for parameter in component["parameters"]:
				if element == parameter["name"]:
					data = pickle.loads(parameter["data"])
					parameter_aux = parameter_aux + (data,)
		component_aux = component_aux + (parameter_aux,)

	value = getattr(class_, component_type)
	value = value + (component_aux,)
	setattr(class_, component_type, value)
	return parameter_aux

def set_component_parsers(class_, component, component_type, component_class):
	component_aux = (component_class,)
	parameter_aux = ()
	#Verify if it has constructor string
	if "constructor" in component and component["constructor"]:
		constructor_string = component["constructor"]

		#Verify if component has parameters to replace in string
		if "parameters" in component and component["parameters"] and len(component["parameters"]) > 0:
			for parameter in component["parameters"]:
				if parameter["name"] in constructor_string:
					data = pickle.loads(parameter["data"])
					#Replace in string
					constructor_string = constructor_string.replace(parameter["name"], str(data))
		
		#Put string in a tuple
		parameter_aux = (constructor_string,)

		#Final component, add tuple from constructor to final component
		l = list(component_aux)
		l.append(parameter_aux)
		component_aux = tuple(l)

	#Set in component class object
	value = getattr(class_, component_type)
	value = value + (component_aux,)
	setattr(class_, component_type, value)

def set_component_validators(class_, component, component_type, component_class):
	component_aux = (component_class())

	value = getattr(class_, component_type)
	value = value + (component_aux,)
	setattr(class_, component_type, value)

def set_component_transformers(class_, component, component_type, component_class, target, components_objects):
	for objects in components_objects:
		if objects["idcomponent"] == component["associated_component"]:
			component_aux = (component_class(), (target, lambda p: p.gen_files[(objects["component_object"].__class__,target,objects["component_key"])]))
			value = getattr(class_, "transformers")
			value = value + (component_aux,)
			setattr(class_, component_type, value)
			break

def set_components(class_, components, basedir_framework):
	#Help to save obejects created
	components_objects = []
	component_key = None
	
	print(components)

	# Iterate components created by user
	for component in components:
		# Get path for class file -> root_dir/framework/component/component.py
		path_component_file = os.path.join(basedir_framework, component["component_type_name"] , component["component_file_name"])

		#Create component class
		module = imp.load_source(component["component_class_name"], path_component_file)
		component_class = getattr(module, component["component_class_name"])

		#Set target
		target = None
		if "target" in component:
			if component["target"]["type"] == "remote":
				target = ssh(component["target"]["domain"], component["target"]["username"])
			else:
				target = "localhost"
		
		#Component type is the variable in class for each component
		component_type = component["component_type_name"]
		if component_type == "preprobes":
			component_type = "pre_probes"
			component_key = set_component_probes(class_, component, component_type, component_class, target)
		elif component_type == "postprobes":
			component_type = "post_probes"
			set_component_probes(class_, component, component_type, component_class, target)
		elif component_type == "parsers":
			set_component_parsers(class_, component, component_type, component_class)
		elif component_type == "validators":
			set_component_validators(class_, component, component_type, component_class)
		elif component_type == "transformers":
			set_component_transformers(class_, component, component_type, component_class, target, components_objects)

		#Add object to dicionary
		if component_type not in components_objects:
			components_objects.append({"component_object" : component_class(), "idcomponent": component["idcomponent"], "component_key": component_key})
			component_key = None
	

#Main
def main(dicio_configuration, campaign, files, executions, campaign_target, fault_injector_target, parameters, components):
	logging.basicConfig(level=logging.DEBUG)

	# Root Dir
	root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
	# Get path for file -> root_dir/framework
	basedir_framework = os.path.abspath(os.path.dirname(__file__))
	# Get path for class file -> root_dir/framework/campaigns/AI4EU.py
	path_campaign_file = os.path.join(basedir_framework, "campaigns" , dicio_configuration["campaign_file_name"])

	#CREATE CLASS
	module = imp.load_source(dicio_configuration["campaign_class_name"], path_campaign_file)
	class_ = getattr(module, dicio_configuration["campaign_class_name"])

	#EXTRACT FILE FROM ZIP AND SAVE IT
	# root_dir/framework/test/campaignid_campaignname
	campaign_path = os.path.join(basedir_framework, "test" , campaign["csvfilename"])

	if 'campaign_configuration' in dicio_configuration:
		read_extract_zip(campaign_path, files, dicio_configuration['campaign_configuration'], class_)

	#SET PARAMETERS
	if len(parameters) != 0:
		set_parameters(class_, parameters)

	#SET TARGET
	if campaign_target:
		set_target(class_, campaign_target)
	else:
		logger.error("No target defined.")
		return 0

	#SET FI VARIABLE (TARGET , INJECTOR PATH)
	if hasattr(class_, "fi"):
		fi_list = list(getattr(class_, "fi"))

		if fault_injector_target["type"] == "remote":
			fi_list[1] = ssh(fault_injector_target["domain"], fault_injector_target["username"])
		else:
			fi_list[1] = "localhost"

		if campaign["fipath"] == "" or campaign["fipath"] == None:
			value = root_dir + fi_list[2]
			fi_list[2] = value
		else:
			fi_list[2] = campaign["fipath"]

		fi_list = tuple(fi_list)
		# Set value
		setattr(class_, "fi", fi_list)

	# #SET EXECUTIONS
	if len(executions) != 0:
		set_executions(class_,executions)
	else:
		logger.error("No executions defined.")
		return 0

	#SET COMPONENTS
	if len(components) != 0:
		set_components(class_, components, basedir_framework)
	else:
		logger.info("No components defined.")

	#Plans	
	main_csv_path = os.path.join(root_dir, "csv-files" , campaign["csvfilename"]+".csv")
	plans = ((class_),)

	#Execute faul injection
	try:
		execute_framework(plans, main_csv_path, (), campaign["idcampaign"])

		shutil.rmtree(campaign_path) 

		#Update campaign after end everything
		if not database.update_end_campaign_state(campaign["idcampaign"],"ended",datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S")):
			logger.error("Could not update campaign state!")
	except Exception as e:
		logger.error(e)
		shutil.rmtree(campaign_path) 
  
		if not database.update_end_campaign_state(campaign["idcampaign"],"ended with error",datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S")):
			logger.error("Could not update campaign state!")

#Execute framework
def execute_framework(plans, main_csv_path, predefined_cols, idcampaign):
	def signal_handler(signal, frame):
		#print "Saving table to CSV"
		io.save_csv(main_csv_path, table)

		# Telling the REST service that we are terminating
		# if (rest_enabled): req = requests.delete(endpoint)
		sys.exit(0)

	# ====== main() ============
	#assert(os.getuid() == 0)

	logger.info("Starting ucXception framework")

	# Lets load the previous results
	table = io.read_or_create_table(main_csv_path, predefined_cols, False)

	# When we receive a Ctrl+C lets save our results before existing
	signal.signal(signal.SIGINT, signal_handler)

	for plan in plans:

		for campaign in plan.campaigns:
			(campaign_name, n_runs, input) = campaign
			

			for run in range(1, n_runs+1):
				plan_obj = plan(input)
				logger.info("Doing run %d out of %d wrt. cmp. %s of plan %s" % (run, n_runs, campaign_name, plan))

				start_time = time.time() # In version >= 3.3 there are better options for high performance counters
				logger.debug("start_time: %f" % start_time)

				row = plan_obj.run(run) # Execute the steps of this campaign

				end_time = time.time() # In version >= 3.3 there are better options for high performance counters

				if row is not None:
					row.update(info_parser().parse(None, campaign_name, run, start_time, end_time))
					table = io.add_row(table, row)
				else:
					logger.warning("main: we didnt include this run because something failed before...")
				logger.debug("end_time: %f" % end_time)
				
				if not database.update_execution_current_run(idcampaign, campaign_name, run, input[0]):
					logger.error("Could not update execution current run!")
				
				del plan_obj
       
		io.save_csv(main_csv_path, table) # Save between campaigns at least, just in case something fails badly
	io.save_csv(main_csv_path, table)
 
