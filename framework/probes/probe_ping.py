from probes.base_probe import Base_Probe
from core import utils
import logging
import os
import time
logger = logging.getLogger(__name__)


class Probe_Ping(Base_Probe):
    """ Basic probe that constantly pings a target PC. But pings cant be trusted ...
    """
    probe_thread = None
    source = None
    target = None
    local_dir = None
    remote_dir = None

    def run_probe(self, source, local_dir, target, rate="0.1"):

        logger.info("Running Ping probe. Source %s to target %s" % (source, target))
        self.source = source
        self.target = target
        self.local_dir = local_dir
        self.remote_dir = "/tmp/ping_out%s.txt" % time.time()
        #cmd_args = "-D -A -n -O %s" % target.ip
        cmd_args = " ".join(["-D", "-n", "-i", rate,  target.ip])
        self.probe_thread = utils.run_anywhere(source, "ping", cmd_args, self.remote_dir, None, True)

    def stop_probe(self):
        logger.info("Stopping Ping probe")
        utils.stop_anywhere(self.source, self.probe_thread, hasOutput=True)

    def extract_data(self):
        logger.info("Extracting data from Ping probe")
        local_file = os.path.join(self.local_dir, "%s_ping.txt" % self.target.ip)
        utils.get_anywhere(self.source, self.remote_dir, local_file)
        return local_file