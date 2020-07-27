import subprocess
from threading import Timer
import signal
#from pprint import pprint
import sys
import random
import time
import os
from probes.base_probe import Base_Probe
from core import utils
import logging

logger = logging.getLogger(__name__)

class Probe_Sar(Base_Probe):
	"""SAR probe. Obtains information about the entire system at regular intervals.
		Lowest possible resolution appears to be 1 second
	"""
	probe_thread = None
	target = None
	local_dir = None

	def run_probe(self, target, local_dir, BINARY_FILE, interval_str):
		logger.info("Running SAR probe, writing to %s with interval %s" % (BINARY_FILE, interval_str))

		self.local_dir = local_dir

		#probe_thread = subprocess.Popen(["sar" ,"-b" ,"-B" ,"-q" ,"-r" ,"-R" ,"-S" ,"-u"  interval_str], stdin=None, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		# Some systems dont have -R
		#with open(os.devnull, 'w') as devnull:
		#	self.probe_thread = subprocess.Popen(["sar" ,"-b" ,"-B" ,"-q" ,"-r" ,"-S" ,"-u", "-o", BINARY_FILE, interval_str], stdin=None, stdout=devnull, stderr=subprocess.PIPE)
		self.target = target
		cmd_args = " ".join(["-b" ,"-B" ,"-q" ,"-r" ,"-S" ,"-u", "-o", BINARY_FILE, interval_str])
		self.probe_thread = utils.run_anywhere(target, "sar", cmd_args, None, None, True)
		self.probe_thread.remote_stdout = self.probe_thread.local_stdout = BINARY_FILE


	def stop_probe(self):
		logger.info("Stopping SAR probe")
		utils.stop_anywhere(self.target, self.probe_thread, hasOutput=False)

	def extract_data(self):
		logger.info("Extracting data from SAR probe")
		local_file = os.path.join(self.local_dir, "%s_sar.bin" % utils.get_ip(self.target))
		utils.get_anywhere(self.target, self.probe_thread.remote_stdout, local_file)
		return local_file
