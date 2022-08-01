
from preprobes.base_probe import Base_Probe
from core import utils

import subprocess
import time
import signal
import logging
import os

logger = logging.getLogger(__name__)

class Probe_Tcpdump(Base_Probe):
	"""Tcpdump probe. Captures network packets from an interface, according to a rule.
	"""
	target = None
	probe_thread = None
	local_dir = None

	def run_probe(self, target, local_dir, if_name, outfile, rule=None):
		logger.info("Running Tcpdump probe, tapping on %s interface and writing to %s" % (if_name, outfile))

		# -K (not included right now) means dont verify checksums.
		# -B sets the buffer size in kilobytes, can be fine tuned to avoid loss packets
		# -p means no promiscuous mode. We dont need it usually as we just want to capture our traffic.
		if (rule == None):
			rule = ""
		cmd_args = " ".join(["-B", "62000", "-p", "-i", if_name, "-K", "-n", "-w", outfile,  rule])
		self.target = target
		self.local_dir = local_dir
		self.probe_thread = utils.run_anywhere(target, "tcpdump", cmd_args, None, None, True)
		self.probe_thread.remote_stdout = self.probe_thread.local_stdout = outfile

	def stop_probe(self):
		logger.info("Stopping Tcpdump probe")
		utils.stop_anywhere(self.target, self.probe_thread, hasOutput=False)

	def extract_data(self):
		logger.info("Extracting data from Tcpdump probe")
		local_file = os.path.join(self.local_dir, "%s_tcpdump.pcap" % self.target.ip)
		utils.get_anywhere(self.target, self.probe_thread.remote_stdout, local_file)
		return local_file

		

