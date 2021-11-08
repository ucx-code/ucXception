import numpy as np
import subprocess
import utils

logger = logging.getLogger(__name__)

class responsiveness_check_parser:
	def parse(self, ssh_config):

		if utils.is_local(ssh_config):
			logger.warn("Responsiveness_Check_Parser does not support checking in localhost")
			return {}
		else:
			login_info = ssh_config.username + "@" + ssh_config.ip 
			row = {}

			p = subprocess.Popen(["/usr/bin/ssh", login_info, "echo 1"], stdin=None, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False)
			row["responsive"] = (p.wait() == 0)

			return row
