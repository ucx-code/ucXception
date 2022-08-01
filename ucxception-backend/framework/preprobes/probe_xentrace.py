import subprocess
import time
import signal
import logging
import os

from preprobes.base_probe import Base_Probe
from core import utils

logger = logging.getLogger(__name__)

class Probe_Xentrace(Base_Probe):
	"""Xentrace probe. Captures Xen event information.
	"""
	probe_thread = None
	target = None
	local_dir = None

	def run_probe(self, target, local_dir, file):
		logger.info("Running Xentrace probe, saving on %s" % file)
		self.local_dir = local_dir
		self.target = target
		self.probe_thread = utils.run_anywhere(target, "xentrace", file, None, None, True)
		self.probe_thread.remote_stdout = file

	def stop_probe(self):
		logger.info("Stopping Xentrace probe")
		utils.stop_anywhere(self.target, self.probe_thread, hasOutput=False)

	def extract_data(self):
		logger.info("Extracting data from Xentrace probe")
		local_file = os.path.join(self.local_dir, "%s_xentrace.bin" % self.target.ip)
		utils.get_anywhere(self.target, self.probe_thread.remote_stdout, local_file)


		

