import core.utils as utils
import core.watchdog as watchdog
from core.eval_it import Eval_It

from core.ssh_config import *

from probes.probe_sar import Probe_Sar
from probes.probe_pidstat import Probe_Pidstat

from validator.ensure_injection import Ensure_Injection

from fi import hw_fi

from transformers.sar_to_csv import Sar_to_CSV
from transformers.save_output import Save_Output
from transformers.pidstat_to_csv import Pidstat_to_CSV
from transformers.save_output import Save_Output

from parsers.ucXception_fi_parser import *
from parsers.current_folder import Current_Folder
from parsers.app_returncode import App_Returncode
from parsers.app_output_md5 import App_Output_Md5

from conf.campaigns.base_campaign import Base_Campaign

import time
import subprocess
import random


class ExampleCampaign(Base_Campaign):

    # Parameters that should be defined by the user
    setup_script_path = "/home/frederico/Dropbox/INVESTIGACAO/DEMO_ucXception/ucXception/framework/workload_example.sh"
    workload_script_path = "/home/frederico/Dropbox/INVESTIGACAO/DEMO_ucXception/ucXception/framework/workload_example.sh"
    postworkload_script_path = "/home/frederico/Dropbox/INVESTIGACAO/DEMO_ucXception/ucXception/framework/workload_example.sh"
    pidgetter_script_path = "/home/frederico/Dropbox/INVESTIGACAO/DEMO_ucXception/ucXception/framework/getpid_example.sh"


    #target = remote_hosts["dolma-xen"]
    target = "localhost"

    # Variables that shouldn't be defined by the user
    p = None  # Holds the process of the workload
    fi_ret = None  # Holds the return value from the FI
    app_output = None  # Holds the result from the app
    app_returncode = None  # Holds the return code from the app

    experiment_data_path = "./results/"  # Folder where the "raw" results will be stored
    watchdog_dur = 30000  # time (in ms) that the watchdog will wait before killing the program

    remote_hosts = {
    }


    # Amount of runs to be performed
    campaigns = (
        ("examplecampaign_test", 2, (True,)),
    )

    # Probes launched before the workload (i.e., do not depend on a specific PID)
    pre_probes = (
    )

    # Probes that can only be launched after the workload has started
    post_probes = (
    )

    # Define the fault injector. None disables it
    fi = (hw_fi, target, "/home/frederico/Dropbox/INVESTIGACAO/DEMO_ucXception/ucXception/fi-tools/hw-faults/pinject_intel")

    # Parsers that use the available information and put it into table.csv
    parsers = (
    )

    # Validates whether everything went OK (failures here will make the run be disregarded)
    validators = (
        (Ensure_Injection()),
    )

    # Transforms raw data into processed data, usually will be kept in "results" folder and not in "table.csv"
    transformers = (
    )

    def __init__(self, input):
        """
        Call the Base Campaign initializer
        """
        super(ExampleCampaign, self).__init__(input)

    def run(self, run_id):
        super(ExampleCampaign, self).run(run_id)
        self.create_run_folder()
        self.pre_launch()
        self.launch()
        self.post_launch()
        self.launch_fi()
        self.peak_loop()
        self.post_finish()
        self.extract_data()
        self.run_transformers()
        self._exec_parsers()
        if self.validate_data():
            return self.row
        else:
            return None

    def pre_launch(self):
        self._start_clock()
        self._pre_launch_probes(self)
        if (self.setup_script_path != None and self.setup_script_path != ""):
            utils.run_anywhere(self.target, "/bin/bash", self.setup_script_path, False, False, False)
        self._stop_clock("prelaunch")

    def launch(self):
        self._start_clock()
        # Run the workload script and assign it to self.p
        self.p = utils.run_anywhere(self.target, "/bin/bash", self.workload_script_path, True, True, True)
        self._stop_clock("launch")

    def post_launch(self):
        self._start_clock()
        self._post_launch_probes(self)
        self._stop_clock("postlaunch")

    def launch_fi(self):
        self._start_clock()
        if self.fi is not None:
            (fi_f, fi_target, fi_path) = self.fi
            if fi_target == "localhost":
                selector = hw_fi.find_local_threads_by_pid
            else:
                selector = hw_fi.find_remote_threads_by_pid

            target_pid = -1
            # Get the PID of the process that we want to target
            if self.pidgetter_script_path != None and self.pidgetter_script_path != "":
                (out, err, rcode) = utils.run_anywhere(self.target, "/bin/bash", self.pidgetter_script_path, False, False, False)
                target_pid = int(random.choice(out.strip().split("\n")))
            else:
                target_pid = self.p.pid

            self.fi_ret = fi_f.launch_fi(fi_target, fi_path, (selector, fi_target, target_pid), (0, 5000))

        self._stop_clock("launchfi")

    def peak_loop(self):
        self._start_clock()
        watchdog.launch_watchdog(self.target, self.watchdog_dur, self.p, [])
        (self.app_output, self.app_error, self.app_returncode) = utils.communicate_anywhere(self.target, self.p)
        watchdog.stop_watchdog()
        self._stop_clock("peak")

    def post_finish(self):
        self._start_clock()
        self._stop_probes()
        if (self.postworkload_script_path != None and self.postworkload_script_path != ""):
            utils.run_anywhere(self.target, "/bin/bash", self.postworkload_script_path, False, False, False)
        self._stop_clock("postfinish")

    def extract_data(self):
        self._start_clock()
        self._extract_data()
        self._stop_clock("dataextract")

    def validate_data(self):
        self._start_clock()
        res = self._run_validators(self.row)
        self._stop_clock("datavalidate")
        return res

    def run_transformers(self):
        self._start_clock()
        self._run_transformers()
        self._stop_clock("transform")
