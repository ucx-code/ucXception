import os
import random
import time
import subprocess
import logging

from core import utils

logger = logging.getLogger(__name__)

def find_local_threads_by_pid(target, pid):
	# Get all the threads from the process and choose one of them randomly to target
	# Uses Linux-specific way of doing so

	# walk() can block if there is no process anymore (TODO fix)
	thread_list = next(os.walk('/proc/' + str(pid) + '/task'))[1]
	return int(random.choice(thread_list))

def find_remote_threads_by_pid(target, pid):
	thread_list = utils.listdir_anywhere(target, '/proc/' + str(pid) + '/task')[0]
	thread_list.remove("")
	return int(random.choice(thread_list))

def launch_fi(target, fi_path, pid_selector_f, times, regs=None, bits=None):
	logger.info("Launching ucXception HW FI")
	(min_time, max_time) = times

	min_reg = 0
	max_reg = 26
	if regs is not None:
		(min_reg, max_reg) = regs

	min_bit = 0
	max_bit = 63
	if bits is not None:
		(min_bit, max_bit) = bits

	# Choose random injection time inside [min_time, max_time]
	inj_time = random.randint(min_time, max_time)

	# Choose random register to inject
	reg = random.randint(min_reg, max_reg)

	# Choose random bit to inject
	bit = random.randint(min_bit, max_bit)

	time.sleep(inj_time/1000.0)

	if type(pid_selector_f) == int:
		chosen_thread = pid_selector_f
	else:
		chosen_thread = apply(pid_selector_f[0], pid_selector_f[1:])

	logger.debug("FI thread is %d" % chosen_thread)

	# Build the params
	fi_param = "%d %d %d %d" % (chosen_thread, reg, bit, inj_time)
	logger.debug("FI params are " + fi_param)
	fiHandle = utils.run_anywhere(target, fi_path, fi_param, True, None, False)
	(stdout, stderr, _) = fiHandle
	logger.debug("FI stdout: %s" % stdout)
	logger.debug("FI stderr: %s" % stderr)
	# Launch the injector and wait for it to finish to read the results
	#(stdout, stderr) = fiHandle.communicate()


	# Parse the results from stdout
	return inj_time, reg, bit, chosen_thread, stdout, stderr
