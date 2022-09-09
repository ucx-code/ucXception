import framework.core.utils as utils
import framework.core.watchdog as watchdog
from framework.core.eval_it import Eval_It

from framework.core.ssh_config import *

from framework.validators.ensure_injection import Ensure_Injection

from framework.fi import sw_fi

from framework.transformers.save_output import Save_Output

from framework.parsers.ucXception_SW_parser import *
from framework.parsers.current_folder import Current_Folder
from framework.parsers.app_returncode import App_Returncode
from framework.parsers.app_output_md5 import App_Output_Md5

from framework.campaigns.base_campaign import Base_Campaign

import os

class SW_Faults_Example(Base_Campaign):
    p = None  # Holds the process of the workload
    app_output = None  # Holds the result from the app
    app_returncode = None  # Holds the return code from the app

    experiment_data_path = "./results/"  # Folder where the "raw" results will be stored
    source_path = "/mnt/c/Tese/ucXception-dev/framework/test/nthprime.c"  # Path of the source file that will be used for SW FI
    #source_path = "/root/test/nthprime.c"  # Path of the source file that will be used for SW FI

    app_path = "/mnt/c/Tese/ucXception-dev/framework/test/nthprime"
    #app_path = "/root/test/nthprime"
    app_input = "20000"  # Input to be passed to the program
    watchdog_dur = 30000  # time (in ms) that the watchdog will wait before killing the program

    recompile_cmd = "/usr/bin/gcc"
    recompile_args = [source_path,"-std=c99",  "-o", app_path]

    patch_files = [] # List of patch files to be applied to the source code. Filled by the SW FI tool
    patch_file = None # Current patch file being injected (changes throughout the campaign execution)
    keep_patches = False # Flags whether do keep or delete the generated patch files after the campaign ends

    # Amount of runs to be performed
    campaigns = (
        #"prime", Eval_It("len(self.patch_files)"), (True,)),
        ("sw-faults", 6, (True,)),
    )
    
    remote_hosts = {
        "dolma-xen"    : ssh("dolma-xen", "root"),
    }
    target = "localhost" # Target system for fault injection

     # Probes launched before the workload (i.e., do not depend on a specific PID)
    pre_probes = (
    )

    # Probes that can only be launched after the workload has started
    post_probes = (
    )

    # Define the fault injector. None disables it
    fi = (sw_fi, "localhost", "/fi-tools/sw-faults/JAR/Injetor.jar")

    # Parsers that use the available information and put it into table.csv
    parsers = (
        (Current_Folder, ("[]",)),
        (App_Returncode, ("[self.app_returncode,]",)),
        (ucXception_SW_Parser, ("[self.patch_files, self.patch_file]",)),
    )

    # Validates whether everything went OK (failures here will make the run be disregarded)
    validators = (
    )

    # Transforms raw data into processed data, usually will be kept in "results" folder and not in "table.csv"
    transformers = (
        #(Save_Output(),    ("localhost", lambda p: p.current_folder, lambda p: p.app_output)),
    )

    def __init__(self, input):
        """
        Call the Base Campaign initializer
        """
        super(SW_Faults_Example, self).__init__(input)
        self.generate_patches() # Generates the patches containing the SW faults

    def __del__(self):
        if not self.keep_patches:
            self.cleanup_patches()
        self.restore_sources()

    def run(self, run_id):
        super(SW_Faults_Example, self).run(run_id)
        self.create_run_folder()
        if len(self.patch_files) > run_id:
            self.patch_file = self.patch_files[run_id] 
            if self.apply_patch(self.patch_file) and self.recompile():
                self.pre_launch()
                # TODO: Add support for background execution in remote host and have a proper watchdog
                self.launch()
                #self.post_launch()
                #self.peak_loop()
                self.post_finish()
                self.extract_data()
                self.run_transformers()
                self._exec_parsers()
                
                self.restore_source_file()
            if self.validate_data():
                return self.row
        return None

    def generate_patches(self):
        if self.fi is not None:
            (fi_f, fi_target, fi_path) = self.fi
            print(fi_f, fi_target, fi_path)
            if fi_target == "localhost":
                (_, self.patch_files) = fi_f.launch_fi(fi_target, fi_path, self.source_path)
            else:
                (_, self.patch_files) = fi_f.launch_fi(self.remote_hosts[fi_target], fi_path, self.source_path)

        # Makes a backup of the source file (post-FI) before the patches are applied
        print(self.target, self.source_path, self.source_path)
        utils.copy_anywhere(self.target, self.source_path, self.source_path + ".backuped")
    
    def cleanup_patches(self):
        for patch_file in self.patch_files:
            utils.delete_anywhere(self.target, patch_file)
    
    def restore_sources(self):
        # Remove the backup.c
        utils.delete_anywhere(self.target, self.source_path + ".backuped")

        # Restore the original source file
        local_dir = os.path.dirname(self.source_path)
        file = os.path.basename(self.source_path)
        splitted = file.split(".")
        origin_file = ".".join(splitted[:-1]) + ".origin." + splitted[-1]
        src_origin = os.path.join(local_dir, origin_file)
        dst_origin = os.path.join(local_dir, file)
        utils.delete_anywhere(self.target, self.source_path)
        utils.copy_anywhere(self.target, src_origin, dst_origin)

    def apply_patch(self, patch_file):
        utils.run_anywhere(self.target, "patch", " ".join([self.source_path, patch_file]), None, None, False)
        #if rcode == 0:
        return True
        #else:
        #    return False

    def restore_source_file(self):
        # Restores the unpatched source file
        utils.copy_anywhere(self.target, self.source_path + ".backuped", self.source_path)

    def recompile(self):
        utils.run_anywhere(self.target, self.recompile_cmd, " ".join(self.recompile_args), None, None, False)
        #if rcode == 0:
        return True
        #else:
        #    return False

    def pre_launch(self):
        self._start_clock()
        self._pre_launch_probes(self)
        self._stop_clock("prelaunch")

    def launch(self):
        self._start_clock()
        self.app_output = utils.run_anywhere(self.target, self.app_path, self.app_input, True, True, False)
        self.app_returncode = 0
        self._stop_clock("launch")

    def post_launch(self):
        self._start_clock()
        self._post_launch_probes(self)
        self._stop_clock("postlaunch")

    def peak_loop(self):
        self._start_clock()
        #watchdog.launch_watchdog(self.watchdog_dur, self.p, [])
        #(self.app_output, self.app_returncode) = utils.proc_communicate(self.p)
        #watchdog.stop_watchdog()
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