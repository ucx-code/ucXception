
from preprobes.base_probe import Base_Probe
from core import utils

import subprocess
import time
import signal
import logging
import os

logger = logging.getLogger(__name__)

class Probe_Intelpcm(Base_Probe):
	"""Intel PCM probe. Captures information from HW counters.
	"""
	target = None
	local_dir = None
	pcm_thread = None
	pcmmemory_thread = None
	pcmpower_thread = None

	#_pcm_path = "/home/baskan/intelpcm/pcm"

	def run_probe(self, target, local_dir, pcm_path, interval, outdir):
		logger.info("Running Intel PCM probe with interval %s and writing to %s" % (interval, outdir))

		self.target = target
		self.local_dir = local_dir

		outfile = outdir + "/tmp_pcm.csv"
		cmd_args = " ".join([interval, "/csv=" + outfile])
		self.pcm_thread = utils.run_anywhere(target, pcm_path + "/pcm.x", cmd_args, None, None, True)
		self.pcm_thread.remote_stdout = self.pcm_thread.local_stdout = outfile

		outfile = outdir + "/tmp_pcmmemory.csv"
		cmd_args = " ".join([interval, "/csv=" + outfile])
		self.pcmmemory_thread = utils.run_anywhere(target, pcm_path + "/pcm-memory.x", cmd_args, None, None, True)
		self.pcmmemory_thread.remote_stdout = self.pcmmemory_thread.local_stdout = outfile

		outfile = outdir + "/tmp_pcmpower.csv"
		cmd_args = " ".join([interval, "/csv=" + outfile])
		self.pcmpower_thread = utils.run_anywhere(target, pcm_path + "/pcm-power.x", cmd_args, None, None, True)
		self.pcmpower_thread.remote_stdout = self.pcmpower_thread.local_stdout = outfile

	def stop_probe(self):
		logger.info("Stopping Intel PCM probe")
		utils.stop_anywhere(self.target, self.pcm_thread, hasOutput=False)
		utils.stop_anywhere(self.target, self.pcmmemory_thread, hasOutput=False)
		utils.stop_anywhere(self.target, self.pcmpower_thread, hasOutput=False)

	def extract_data(self):
		logger.info("Extracting data from Intel PCM probe")
		local_file1 = os.path.join(self.local_dir, "%s_pcm.csv" % self.target.ip)
		local_file2 = os.path.join(self.local_dir, "%s_pcmmemory.csv" % self.target.ip)
		local_file3 = os.path.join(self.local_dir, "%s_pcmpower.csv" % self.target.ip)

		utils.get_anywhere(self.target, self.pcm_thread.remote_stdout, local_file1)
		utils.get_anywhere(self.target, self.pcmmemory_thread.remote_stdout, local_file2)
		utils.get_anywhere(self.target, self.pcmpower_thread.remote_stdout, local_file3)



		

