import pandas as pd
import subprocess
from threading import Timer
import signal
from pprint import pprint
import sys
import random
import time
import os
import subprocess

from probes.base_probe import Base_Probe
from core import utils
import logging

logger = logging.getLogger(__name__)

class Probe_Pidstat(Base_Probe):
	"""Pidstat probe. Captures information from a specific PID at regular intervals.
	"""
	probe_thread = None
	target = None
	local_dir = None

	def run_probe(self, target, local_dir, pid, interval_str, all_children=False):
		logger.info("Running Pidstat probe, tracing PID %d with interval %s" % (pid, interval_str))
		
		self.target = target
		self.local_dir = local_dir
		local_file = os.path.join(self.local_dir, "%s_pidstat.txt" % utils.get_ip(self.target))

		task_type = "CHILD" if all_children else "TASK"
		cmd_args = " ".join(["-h" ,"-d" ,"-l" ,"-r" ,"-s", "-w", "-t","-T" , task_type, "-u", "-w", "-p", str(pid), interval_str])
		self.probe_thread = utils.run_anywhere(target, "pidstat", cmd_args, local_file, subprocess.PIPE, True)


	def stop_probe(self):
		logger.info("Stopping Pidstat probe")
		utils.stop_anywhere(self.target, self.probe_thread, hasOutput=True)

	def extract_data(self):
		logger.info("Extracting data from Pidstat probe")
		#local_file = os.path.join(self.local_dir, "%s_pidstat.txt" % utils.get_ip(self.target))
		#utils.get_anywhere(self.target, self.probe_thread.remote_stdout, local_file)
		local_file = os.path.join(self.local_dir, "%s_pidstat.txt" % utils.get_ip(self.target))
		return local_file


	def convert_to_csv(data):
		out = []
		firstComment=True
		for line in data.split("\n"):
			line  = ' '.join(line.split())
			if (len(line) == 0):
				pass
			elif (line[0] == '#'):
				if  (firstComment == True):
					firstComment = False
					out.append(line[2:].replace(" ", ","))
			else:
				out.append(line.replace(" ", ","))
		return out[1:]

	def toPandas(csvData):

		with open('/tmp/uglyHack', 'w') as f:
	    		for item in csvData:
	        		print >> f, item

		df = pd.read_csv("/tmp/uglyHack")
		
		# Rename the columns
		new_cols = []
		for orig_col in df.columns:
			new_cols.append("pidstat_" + orig_col)
		df.columns = new_cols
		
		return df
				
