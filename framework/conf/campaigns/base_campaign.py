import core.utils as utils
import core.watchdog as watchdog
from core.eval_it import Eval_It
from core.ssh_config import *

from probes import probe_sar
from probes import probe_pidstat

from parsers.ucXception_fi_parser import *

import time
import os

class Base_Campaign(object):
	"""Base class for every campaign. User-made campaigns should inherit from here
	Defines internal helper functions and variables
	"""

	row = None  # Dict with key-value pairs that will be saved to CSV
	gen_files = None  # Dict of files that have been generated during this run

	time_start = None
	current_folder = None
	fi_enabled = None

	def __init__(self, input):
		self.row = {}
		self.gen_files = {}
		if input is not None:
			self.fi_enabled = input[0]

	def _eval_lambda_funcs(self, input, locals):
		"""Evaluates lambda functions in user-defined parameters.
		Allows the user to dynamically access experiment run variables	
		"""
		output = []
		for i in input:
			a = i
			if (type(i) == type(lambda: None)):
				a = i(self)
			output.append(a)
		return tuple(output)

	def _eval_targets(self, target, dir, input):
                if (not isinstance(target, SSH_Config) and utils.is_local(target) == False):
			return (self.remote_hosts[target], dir,) + input
		return (target, dir,) + input

	def _pre_launch_probes(self, locals):
		"""Spawns the probes that should run before the workload is launched.
		"""
		for probe_info in self.pre_probes:
			(probe_module, probe_target, probe_in) = probe_info
			#probe_in = self._eval_lambda_funcs(probe_in, locals) # No need for this yet
			probe_module.run_probe(*self._eval_targets(probe_target, self.current_folder, probe_in))

	def _post_launch_probes(self, locals):
		"""Spawns the probes that must run after the workload is launched.
		Usually because they depend on the PID of the workload process to work.
		"""
		for probe_info in self.post_probes:
			(probe_module, probe_target, probe_in) = probe_info
			probe_in = self._eval_lambda_funcs(probe_in, locals)
			probe_module.run_probe(*self._eval_targets(probe_target, self.current_folder, probe_in))

	def _stop_probes(self):
		"""Stop all the probes
		"""
		for probe_info in (self.pre_probes + self.post_probes):
			(probe_module, probe_target,  probe_in) = probe_info
			probe_module.stop_probe()

	def _exec_parsers(self):
		"""Exec all the parsers to extract the information that will be saved to the CSV
		"""
		for parser in self.parsers:
			(parser_name, parser_in) = parser

			parser_in_exec = [self.current_folder, ]
			for x in self._eval_lambda_funcs(parser_in, self):
				parser_in_exec.extend(eval(x))
				
			print("!!!!! " + str(parser_in_exec))
			try:
				parser_in_exec = parser_in_exec
				print("======= " + str(parser_name) + " - " + str(parser_in_exec)  )
				self.row.update(parser_name().parse(*parser_in_exec))
			except TypeError as te:
				if self.fi_enabled is False:
					pass # Ignore it
				else:
					raise te


	def _extract_data(self):
		for probe_info in (self.pre_probes + self.post_probes):
			(probe_module, probe_target,  probe_in) = probe_info
			data_list = probe_module.extract_data()
			if data_list is not None:
				if probe_module.__class__ not in self.gen_files:
					self.gen_files[probe_module.__class__] = [data_list,]
				else:
					self.gen_files[probe_module.__class__] = self.gen_files[probe_module.__class__] + [data_list,]
			#print self.gen_files

	def _run_validators(self, row):
		if self.fi_enabled is True:
			for validator in self.validators:
				if not validator.run(row):
					return False
		return True

	def _run_transformers(self):
		for transformer_info in self.transformers:
			(transformer_module, transformer_in) = transformer_info
			transformer_in = self._eval_lambda_funcs(transformer_in, self)
			data_list = transformer_module.run(*transformer_in)
			if data_list is not None:
				if transformer_module.__class__ not in self.gen_files:
					self.gen_files[transformer_module.__class__] = data_list,
				else:
					self.gen_files[transformer_module.__class__] = self.gen_files[transformer_module.__class__] + data_list

	def create_run_folder(self):
		# Always try to create base folder
		utils.local_mkdir(self.experiment_data_path)

		now = time.time()
		folder_name = "run_%s" % time.strftime("%y_%m_%d_%T", time.localtime(now))

		# There is a chance that the folder already exists (2 runs in the same second), so we append an id
		for i in range(1, 100):
			new_name = folder_name  + "_%d" % i
			if not os.path.isdir(new_name):
				break
		assert(i < 99)	# Something went very badly

		self.current_folder = os.path.join(self.experiment_data_path, folder_name)
		utils.local_mkdir(self.current_folder)

	def _start_clock(self):
		assert(self.time_start is None)
		self.time_start = time.time()

	def _stop_clock(self, name):
		now = time.time()
		self.row[name + "_start"] = self.time_start
		self.row[name + "_stop"] = now
		self.row[name + "_dur"] = now - self.time_start
		self.time_start = None

	def run(self, run_id):
		# Add support for easily disabling FI and flagging it in the CSV
		if self.fi_enabled is False:
			self.fi = None
			self.row["fi_enabled"] = "False"
		else:
			self.row["fi_enabled"] = "True"

