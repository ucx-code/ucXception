from flask import Flask, request
from flask_restful import Api, Resource, reqparse
import time
import pandas as pd
import csv

class ExecutionModel():
	active=False
	start_time=None
	n_runs=None
	n_campaigns=None
	current_run=None
	current_campaign=None
	end_time=None
	last_update=None

 

class Execution(Resource):
	currentExec= None
	
	def updateStamp(self):
		Execution.currentExec.last_update = time.time()

	# The campaign manager has been launched and is posed to perform one or more FI campaigns.
	def post(self):
		self.updateStamp()
		# We are the new execution
		Execution.currentExec.active = True
		Execution.currentExec.start_time = request.form.get('start_time')
		Execution.currentExec.n_campaigns = request.form.get('n_campaigns')
		Execution.currentExec.n_runs = request.form.get('n_runs')
		
		addRowToData(Execution.currentExec)
		return '', 200

	# The campaign manager has finished running.
	def delete(self):
		self.updateStamp()
		Execution.currentExec.end_time=time.time()
		Execution.currentExec.active=False
		Execution.currentExec = ExecutionModel()
		updateRowInData(Execution.currentExec)
		return '', 200

	# A FI has been completed and the results are sent here
	def put(self):
		print Execution.currentExec.start_time
		self.updateStamp()
		Execution.currentExec.current_run = request.form.get('current_run')
		Execution.currentExec.current_campaign = request.form.get('current_campaign')
		updateRowInData(Execution.currentExec)
		return '', 200

	def get(self):

		data = {'active' : Execution.currentExec.active,
			'start_time' : Execution.currentExec.start_time,
			'n_runs' : Execution.currentExec.n_runs,
			'n_campaigns' : Execution.currentExec.n_campaigns,
			'current_run': Execution.currentExec.current_run,
			'current_campaign' : Execution.currentExec.current_campaign,
			'end_time' : Execution.currentExec.end_time,
			'last_update' : Execution.currentExec.last_update
			}
		return data

data = None

columns = ["active", "n_runs", "start_time", "n_campaigns", "current_run", "current_campaign", "end_time", "last_update"]
def loadExecutions(file_path):
	try:
		table = pd.read_csv(file_path, header=0, na_filter=False)
	except Exception as e:
		print "Failed to read CSV file with exception", e
		table = pd.DataFrame(columns=columns)
	return table

def saveExecutions(file_path, table):
	table.to_csv(file_path, quotechar='"', quoting=csv.QUOTE_NONNUMERIC, index=False, columns=columns)

def updateRowInData(currentExec):
	global data
	# Remove the old row
	data = data[data["start_time"] != currentExec.start_time]
	# Add it again but with the most current values
	addRowToData(currentExec)

def addRowToData(currentExec):
	global data, csv_path

	row = {'active' : currentExec.active,
	       'n_runs'  : currentExec.n_runs,
	       'n_campaigns' : currentExec.n_campaigns, 
	       'current_run' : currentExec.current_run,
	       'current_campaign' : currentExec.current_campaign,
	       'end_time' : currentExec.end_time,
	       'last_update' : currentExec.last_update,
	       'start_time' : currentExec.start_time
		}
	data = data.append(row, ignore_index=True)
	saveExecutions(csv_path, data)

def findActiveExecution(df):
	activeRow = df['active'] == True
	if (len(activeRow) != 1):
		return ExecutionModel()
	else:
		e = ExecutionModel()
		e.active = True
		e.n_runs = df[activeRow]["n_runs"]
		e.n_campaigns = df[activeRow]["n_campaigns"]
		e.current_run = df[activeRow]["current_run"]
		e.current_campaign = df[activeRow]["current_campaign"]
		e.end_time = df[activeRow]["end_time"]
		e.last_update = df[activeRow]["last_update"]
		return e

# Load old executions
csv_path = "./executions_data.csv"
data = loadExecutions(csv_path)

Execution.currentExec = findActiveExecution(data)
print Execution.currentExec

app = Flask(__name__)
api = Api(app)

api.add_resource(Execution, "/exec/current")
app.run(debug = True)