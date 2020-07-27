from probes.base_probe import Base_Probe
from core import utils
import logging
import os

logger = logging.getLogger(__name__)


class Probe_Logs(Base_Probe):
    """Logs probe. Basically not a real probe, but will capture the specified logs during data extraction phase
    """
    known_logs = {
        "xen": ("/var/log/xen", ),
        "linux": ("/var/log/dmesg", "/var/log/messages", "/var/log/secure"),
        "openstack": ("/var/log", ),
    }

    target = None
    wanted = None
    exp_folder = None

    def run_probe(self, target, local_dir, wanted):
        self.target = target
        self.wanted = wanted
        self.exp_folder = local_dir

    def stop_probe(self):
        pass

    def extract_data(self):
        logger.info("Extracting data using the logs probe")
        logs_dir = os.path.join(self.exp_folder, "logs")
        utils.local_mkdir(logs_dir)
        for key in self.wanted:
            key_dir = os.path.join(logs_dir, key)
            utils.local_mkdir(key_dir)
            for path in self.known_logs[key]:
                local_file = os.path.join(key_dir, path.replace("/", "_"))
                utils.get_anywhere(self.target, path, local_file)