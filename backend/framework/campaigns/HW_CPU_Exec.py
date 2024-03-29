import framework.core.utils as utils
import framework.core.watchdog as watchdog
from framework.core.eval_it import Eval_It

from framework.core.ssh_config import *

from framework.preprobes.probe_sar import Probe_Sar
from framework.postprobes.probe_pidstat import Probe_Pidstat

from framework.validators.ensure_injection import Ensure_Injection

from framework.fi import hw_fi

from framework.transformers.sar_to_csv import Sar_to_CSV
from framework.transformers.save_output import Save_Output
from framework.transformers.pidstat_to_csv import Pidstat_to_CSV
from framework.transformers.save_output import Save_Output

from framework.parsers.ucXception_fi_parser import *
from framework.parsers.current_folder import Current_Folder
from framework.parsers.app_returncode import App_Returncode
from framework.parsers.app_output_md5 import App_Output_Md5

from framework.campaigns.base_campaign import Base_Campaign

import time
import subprocess


class HW_CPU_Exec(Base_Campaign):
    """ AI4EU conf
    """

    p = None  # Holds the process of the workload
    fi_ret = None  # Holds the return value from the FI
    app_output = None  # Holds the result from the app
    app_returncode = None  # Holds the return code from the app

    experiment_data_path = "./results/"  # Folder where the "raw" results will be stored
    app_path = "/mnt/c/Tese/ucXception-dev/framework/test/nthprime"  # Relative path of the python script that will be evaluated
    app_input = "20000"  # Input to be passed to the python script
    fi_min = 0
    fi_max = 0
    #app_path = "/mnt/c/Tese/ucXception-dev/framework/test/primes.py"
    #app_input = "1300000"
    watchdog_dur = 30000  # time (in ms) that the watchdog will wait before killing the program

    remote_hosts = {
        "dolma-xen"    : ssh("dolma-xen", "root"),
    }

    #target = remote_hosts["dolma-xen"]
    target = "localhost"

    # Amount of runs to be performed
    campaigns = (
        ("ai4eu_primetest_c", 3, (True,)),
        ("ai4eu_primetest_c_golden", 3, (False,)), # golden runs
    )

    # Probes launched before the workload (i.e., do not depend on a specific PID)
    pre_probes = (
        #(Probe_Sar(), "localhost", ("/tmp/probe_sar_out", "1")),
        #(Probe_Sar(), "localhost", ("/tmp/sar_out", "1")),
        #(Probe_Perf(), "localhost", ("/home/baskan/intelpcm/pcm", "0.1", "/tmp/",)),
    )

    # Probes that can only be launched after the workload has started
    post_probes = (
        #(Probe_Pidstat(), "localhost", (lambda locals: locals.p.pid, "1", False)),
    )

    # Define the fault injector. None disables it
    fi = (hw_fi, target, "/fi-tools/hw-faults/pinject_intel")

    # Parsers that use the available information and put it into table.csv
    parsers = (
        #(ucXception_fi_parser, ("self.fi_ret",)),
        #(Current_Folder, ("[]",)),
        #(App_Returncode, ("[self.app_returncode,]",)),
        #(App_Output_Md5, ("[self.app_output, 810674, '01a86739f39ebb1e2802c84ec4ce7080']",)),
    )

    # Validates whether everything went OK (failures here will make the run be disregarded)
    validators = (
        #(Ensure_Injection()),
    )

    # Transforms raw data into processed data, usually will be kept in "results" folder and not in "table.csv"
    transformers = (
        #(Sar_to_CSV(),     ("localhost", lambda p: p.gen_files[Probe_Sar().__class__])),
        #(Save_Output(),    ("localhost", lambda p: p.current_folder, lambda p: p.app_output)),
        #(Pidstat_to_CSV(), ("localhost", lambda p: p.gen_files[Probe_Pidstat().__class__])),
    )

    def __init__(self, input):
        """
        Call the Base Campaign initializer
        """
        super(HW_CPU_Exec, self).__init__(input)

    def run(self, run_id):
        super(HW_CPU_Exec, self).run(run_id)
        self.create_run_folder()
        self.pre_launch()
        self.launch()
        self.post_launch()
        self.launch_fi()
        self.peak_loop()
        self.post_finish()
        # self.restore_state()
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
        self._stop_clock("prelaunch")

    def launch(self):
        self._start_clock()
        #self.p = utils.run_anywhere(self.target, "python", self.app_path + " " + self.app_input, True, True, True)
        self.p = utils.run_anywhere(self.target, self.app_path, self.app_input, True, True, True)
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

            #self.fi_ret = fi_f.launch_fi(fi_target, fi_path, (selector, fi_target, self.p.pid), (500, 1500))
            self.fi_ret = fi_f.launch_fi(fi_target, fi_path, (selector, fi_target, self.p.pid), (self.fi_min, self.fi_max))

        self._stop_clock("launchfi")

    def peak_loop(self):
        self._start_clock()
        watchdog.launch_watchdog(self.target, self.watchdog_dur, self.p, [])
        #print("OLA")
        (self.app_output, self.app_error, self.app_returncode) = utils.communicate_anywhere(self.target, self.p)
        #print(self.app_output)
        print(self.app_error)
        watchdog.stop_watchdog()
        self._stop_clock("peak")

    def post_finish(self):
        self._start_clock()
        self._stop_probes()
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
