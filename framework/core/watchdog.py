
from threading import Semaphore, Timer
import psutil
import logging

logger = logging.getLogger(__name__)

# Internal variables
_watchdog_timer = None
_watchdog_state = False # False means no kill yet, True means kill
_watchdog_active = False
_watchdog_lock =  Semaphore()

# Intenal function that ensures everything is dead
def _kill_the_processes(proc_list, func_list):
	global _watchdog_lock, _watchdog_active, _watchdog_state, _watchdog_timer
	
	logger.warning("Watchdog is killing the processes! -> %s", proc_list)
	_watchdog_lock.acquire()

	for proc in (proc_list, ):
		try:
			while (proc.poll() == None):
				parent = psutil.Process(proc.pid)
				for child in parent.children(recursive=True):  # or parent.children() for recursive=False
					child.kill()
				proc.kill()
		except Exception as e:
			logger.debug(e)
			pass

	for func in func_list:
		func()

	_watchdog_state = True
	_watchdog_lock.release()


# Launch a watchdog timer that will wait 'timeout_ms' mills until it will kill all
# processes in proc_list and call all funcs in 'func_list'
def launch_watchdog(timeout_ms, proc_list, func_list):
	global _watchdog_lock, _watchdog_active, _watchdog_state, _watchdog_timer
	_watchdog_lock.acquire()
	if (_watchdog_active == False):
		_watchdog_state = False # we havent killed anything yet
		_watchdog_timer = Timer(timeout_ms / 1000, _kill_the_processes, [proc_list, func_list])
		_watchdog_active = True
		_watchdog_timer.start()
		_watchdog_lock.release()
		return True
	else:
		_watchdog_lock.release()
		return False

def stop_watchdog():
	global _watchdog_lock, _watchdog_active, _watchdog_state, _watchdog_timer
	_watchdog_lock.acquire() # Dont stop it while it runs
	#if (_watchdog_state == False):
	_watchdog_timer.cancel()
	_watchdog_active = False
	_watchdog_lock.release()

def is_active():
	global _watchdog_lock, _watchdog_active, _watchdog_state, _watchdog_timer
	_watchdog_lock.acquire()
	v = _watchdog_active
	_watchdog_lock.release()
	return v

def has_killed():
	global _watchdog_lock, _watchdog_active, _watchdog_state, _watchdog_timer
	_watchdog_lock.acquire()
	v = _watchdog_state
	_watchdog_lock.release()
	return v