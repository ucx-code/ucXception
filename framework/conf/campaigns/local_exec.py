import core.utils as utils
import core.watchdog as watchdog
from core.eval_it import Eval_It

from core.ssh_config import *

from probes.probe_sar import Probe_Sar
from probes.probe_pidstat import Probe_Pidstat
from probes.probe_tcpdump import Probe_Tcpdump
from probes.probe_xentrace import Probe_Xentrace
from probes.probe_intelpcm import Probe_Intelpcm
from probes.probe_ping import Probe_Ping
from probes.probe_logs import Probe_Logs

from validator.ensure_injection import Ensure_Injection

from fi import local_hw_fi
from fi import hard_reboot

from transformers.ping_to_csv import Ping_to_CSV
from transformers.sar_to_csv import Sar_to_CSV
from transformers.pcap_to_tcp_csv import Pcap_to_TCP_CSV

from parsers.ucXception_fi_parser import *
from parsers.pcap_tcp_parser import *

from conf.campaigns.base_campaign import Base_Campaign

import time

class Local_Exec(Base_Campaign):
	"""Example class. Launches a program locally, some probes, watchdog, FI and some parsers.
	"""
	p = None # Holds the process of the workload
	fi_ret = None # Holds the return value from the FI

	def __init__(self):
		super(Local_Exec, self).__init__()
		
	def pre_launch(self):
		self._start_clock()
		self._pre_launch_probes(self)
		self._stop_clock("prelaunch")


	def launch(self):
		self._start_clock()
		self.p = utils.run_anywhere(self.remote_hosts["faulty"], "/bin/sleep", "10", None, None, True)
		self._stop_clock("launch")


	def post_launch(self):
		self._start_clock()
		self._post_launch_probes(self)
		self._stop_clock("postlaunch")

	def launch_fi(self):
		self._start_clock()
		if self.fi is not None:
			(fi_f, fi_target, fi_path) = self.fi
			#selector = local_hw_fi.find_threads_by_pid
			#self.fi_ret = fi_f.launch_fi(self.remote_hosts[fi_target], fi_path, (selector, self.p.pid), (1, 3))
			self.fi_ret = fi_f.launch_fi(self.remote_hosts[fi_target], fi_path, self.p.pid, (1, 3))

		self._stop_clock("launchfi")

	def peak_loop(self):
		self._start_clock()
		#watchdog.launch_watchdog(30000, self.p, [])
		#utils.proc_wait(self.p)
		time.sleep(12) # Usualy the workload generator is local so no problem about this
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

	def run(self):
		self.create_run_folder()
		self.pre_launch()
		self.launch()
		self.post_launch()
		self.launch_fi()
		self.peak_loop()
		self.post_finish()
		#self.restore_state()
		self.extract_data()
		self.run_transformers()
		self._exec_parsers()
		if self.validate_data():
			return self.row
		else:
			return None

	experiment_data_path = "./results/"

	remote_hosts = {
			"frederico" : ssh("127.0.0.1", "root"),
			"faulty"    : ssh("faulty", "root"),
			"kahvefali" : ssh("kahvefali", "root"),

		}

	campaigns = (
			("nome", 1),
		)

	pre_probes = (
			#(Probe_Sar(),      "kahvefali", ("/tmp/sar_out", "1")),
			#(Probe_Tcpdump(),  "kahvefali", ("em2", "/tmp/pcap.pcap")),
			#(Probe_Xentrace(), "kahvefali", ("/tmp/xentrace.out",)),
			#(Probe_Intelpcm(), "kahvefali", ("/home/baskan/intelpcm/pcm", "0.1", "/tmp/",)),
			(Probe_Sar(),      "faulty", ("/tmp/sar_out", "1")),
			(Probe_Tcpdump(),  "faulty", ("enp3s0", "/tmp/pcap.pcap")),
			(Probe_Intelpcm(), "faulty", ("/root/intelpcm/pcm", "0.1", "/tmp/",)),
			(Probe_Ping(),     "localhost",  (remote_hosts["faulty"],)),
			#(Probe_Perf(), "kahvefali", ("/home/baskan/intelpcm/pcm", "0.1", "/tmp/",)),
	)

	post_probes = (
			#(Probe_Pidstat(), "kahvefali", (lambda locals: locals.p.pid, "1")),
			(Probe_Logs(), "faulty", (("linux",),)),
	)

	#fi = (local_hw_fi, "kahvefali", "/root/HW_Injectors_ucXception/newest_injector/pinject_intel")
	fi = (local_hw_fi, "faulty", "/root/ucxception_fi_framework/HW_Injectors_ucXception/newest_injector/pinject_intel")
	#fi = (hard_reboot, "faulty", None)
	#fi = None

	parsers = (
			(ucXception_fi_parser, (Eval_It("self.fi_ret"),)),
			#(pcap_tcp_parser, (lambda p: p.gen_files[Pcap_to_TCP_CSV().__class__],)),
		)

	validators = (
			#(Ensure_Injection()),
		)

	transformers = (
		#(Ping_to_CSV(),      ("", lambda p: p.gen_files[Probe_Ping().__class__])),
		#(Sar_to_CSV(),       ("localhost", lambda p: p.gen_files[Probe_Sar().__class__])),
		#(Pcap_to_TCP_CSV(),  ("localhost", lambda p: p.gen_files[Probe_Tcpdump().__class__])),
	)
