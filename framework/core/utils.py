from core.ssh_config import *
from core.remote_proc import *

import subprocess
import signal
import time
import logging
import shutil
import os

logger = logging.getLogger(__name__)

DEVNULL = open(os.devnull, 'w')

def get_ip(target):
	if target == "localhost":
		return target
	else:
		return target.ip

# Launches a process (locally) with the specified program path and args-
# Returns a Popen structure
def proc_launch_locally(program, args, _stdout=None, _stderr=None, _shell=False):
	# Hack to make a valid cmd_arg
	#cmd_arg = [program, ] + [str(x) for x in (args, )]
	if type(args) == list:
		cmd_arg = [program, ] + args
	else:
		cmd_arg = [program, ] + args.split(" ")

	p = subprocess.Popen(cmd_arg, stdin=None, stdout=_stdout, stderr=_stderr, shell=_shell)
	return p

def proc_wait(proc):
	return (None, None, proc.wait())

def proc_communicate(proc):
	(out, err) = proc.communicate()
	return (out, err, proc.returncode)

def proc_launch_ssh(ssh_config, program, args, stdin=None, stdout=None, stderr=None, background=True):

	login_info = ssh_config.username + "@" + ssh_config.ip 
	cmd_arg = " ".join([program, ] + [str(x) for x in (args, )])
	
	logger.debug("Called proc_launch_ssh in %s for program '%s' and background %s" % (ssh_config.ip, cmd_arg, background))
	# ssh frederico@127.0.0.1 'nohup sleep 30 > out 2> err < /dev/null & echo $!'

	if not background:
		p = subprocess.Popen(["/usr/bin/ssh", "-o ServerAliveInterval=5", "-o ServerAliveCountMax=6", login_info, cmd_arg],
							 stdin=None, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False)
		(out, err) = p.communicate()

		# Check if the SSH connection timedout
		if (p.returncode == 255) and (err == 'Timeout, server %s not responding.\r\n' % ssh_config.ip):
			logger.error("proc_launch_ssh: Remote server timedout...")

		return (out, err, p.returncode)
	else:
		now = time.time()
		stdout_file = stderr_file = stdin_file =  "/dev/null"
		if stdin is not None:
			stdin_file = stdin
		if stdout is not None and type(stdout) == bool:
			stdout_file = "/tmp/ucxception_out_" + str(now)
		if stderr is not None and type(stderr) == bool:
			stderr_file = "/tmp/ucxception_err_" + str(now)

		ssh_arg = "/usr/bin/nohup %s > %s 2> %s < %s & echo $!" % (cmd_arg, stdout_file, stderr_file, stdin_file)
		p = subprocess.Popen(["/usr/bin/ssh", "-o ServerAliveInterval=5", "-o ServerAliveCountMax=6", login_info, ssh_arg],
							 stdin=None, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False)
		out = p.communicate()
		assert(p.returncode != 255) # Make sure SSH didnt fail

		# Convert the output from ssh into a Remote_Proc for future easy use
		return create_remote_proc(ssh_config, int(out[0][:-1]), stdout, stderr, stdout_file, stderr_file)

def remote_get(ssh_config, in_path, out_path, keep=False):
	logger.debug("Called remote_get in %s for files %s -> %s" % (ssh_config.ip, in_path, out_path))

	remote_args = ssh_config.username + "@" + ssh_config.ip + ":" + in_path
	if keep:
		cmd = ["/usr/bin/rsync", "-avz", remote_args, out_path]
	else:
		cmd = ["/usr/bin/rsync", "-avz", "--remove-source-files", remote_args, out_path]

	p = subprocess.Popen(cmd, stdin=None, stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=False)
	(stdout, stderr) = p.communicate()
	if p.returncode != 0:
		logger.error("Remote_Get failed with returncode %d" % p.returncode)
		return False
	logger.debug("Leaving remote_get with success")
	return True

def local_get(in_path, out_path, keep=False):
	logger.debug("Called local_get for files %s -> %s" % (in_path, out_path))
	try:
		if keep:
			shutil.copy(in_path, out_path)
		else:
			shutil.move(in_path, out_path)
	except shutil.Error as e:
		logger.error(e)
		return False
	return True


def is_local(target):
	if target == "localhost":
		return True
	else:
		return False

def stop_local(proc, hasOutput=True):
	logger.debug("Called stop_local with hasOutput=%s" % hasOutput)
	proc.send_signal(signal.SIGINT)
	if not hasOutput:
		proc.wait()
	else:
		return proc.communicate()

def stop_remote(target, proc, hasOutput=True):
	logger.debug("Called stop_remote in %s for PID %d with hasOutput=%s" % (target.ip, proc.pid, hasOutput))

	#kill_cmd = "-- -$(/bin/ps -o pgid=%d | /bin/grep -o '[0-9]*')" % proc.pid
	kill_cmd = "%d" % proc.pid
	proc_launch_ssh(target, "/bin/kill", kill_cmd, stdin=None, stdout=None, stderr=None, background=False)

	if hasOutput:
		# Move the remote stdout and stderr files to their local position, if required
		if proc.local_stdout is not None:
			remote_get(target, proc.remote_stdout, proc.local_stdout)
		if proc.local_stderr is not None:
			remote_get(target, proc.remote_stderr, proc.local_stderr)

def get_anywhere(target, in_path, out_path, keep=False):
	if is_local(target):
		return local_get(in_path, out_path, keep)
	else:
		return remote_get(target, in_path, out_path, keep)

def _convert_pipes(pipe):
	if pipe is not None or pipe is not False:
		if type(pipe) == str:
			return open(pipe, "w")
		else:
			return subprocess.PIPE

	else:
		return DEVNULL

def run_anywhere(target, program, args, stdout, stderr, background):
	if is_local(target):
		# local
		stdout = _convert_pipes(stdout)
		stderr = _convert_pipes(stderr)

		local_handle = proc_launch_locally(program, args, stdout, stderr, False)
		if background:
			return local_handle
		else:
			if (stdout != DEVNULL) or (stderr != DEVNULL):
				return proc_communicate(local_handle)
			else:
				return proc_wait(local_handle)
	else:
		# remote
		return proc_launch_ssh(target, program, args, None, stdout, stderr, background)

def stop_anywhere(target, proc, hasOutput=True):
	if is_local(target):
		return stop_local(proc, hasOutput)
	else:
		stop_remote(target, proc, hasOutput)

def mkdir_anywhere(target, path):
	if is_local(target):
		return local_mkdir(path)
	else:
		(_, _, rcode) = remote_mkdir(target, path)
		return (rcode == 0)

def move_anywhere(target, origin, destination):
	if is_local(target):
		return local_move(origin, destination)
	else:
		return remote_move(target, origin, destination)

def delete_anywhere(target, path):
	if is_local(target):
		return local_delete(path)
	else:
		(_, _, rcode) = remote_delete(target, path)
		return (rcode == 0)

def listdir_anywhere(target, path):
	if is_local(target):
		return local_listdir(path)
	else:
		return remote_listdir(target, path)

def copy_anywhere(target, origin, destination):
	if is_local(target):
		return local_get(origin, destination, keep=True)
	else:
		(_, _, rcode) = remote_copy(target, origin, destination)
		return rcode == 0

def local_delete(path):
	try:
		os.remove(path) # will throw exception if doesnt exist
	except OSError as e:
		logger.debug("Exception during local_delete: " + e)
		return False
	return True

def communicate_anywhere(target, proc):
	if is_local(target):
		return proc_communicate(proc)
	else:
		return remote_proc_communicate(proc)

def remote_delete(ssh_config, path):
	return proc_launch_ssh(ssh_config, "/usr/bin/rm", "-f " + path, stdin=None, stdout=True, stderr=True, background=False)

def local_mkdir(path):
	# Always try to create base folder
	try:
		os.mkdir(path)
	except Exception as e:
		# logger.debug
		pass

def remote_mkdir(ssh_config, path):
	return proc_launch_ssh(ssh_config, "/usr/bin/mkdir", path, stdin=None, stdout=True, stderr=True, background=False)

def local_move(origin, destination):
	shutil.move(origin, destination)
	return True

def remote_move(ssh_config, origin, destination):
	return proc_launch_ssh(ssh_config, "/usr/bin/mv", origin + " " + destination, stdin=None, stdout=True, stderr=True, background=False)

def local_listdir(path):
	return (os.listdir(path), None, 0)

def remote_listdir(ssh_config, path):
	(stdout, stderr, rcode) = proc_launch_ssh(ssh_config, "/usr/bin/ls", "-1 " + path, stdin=None, stdout=True, stderr=True, background=False)
	return (stdout.split("\n"), stderr, rcode)

def remote_copy(ssh_config, origin, destination):
	return proc_launch_ssh(ssh_config, "/usr/bin/cp", origin + " " + destination, stdin=None, stdout=True, stderr=True, background=False)

def wait_for_remote_host(ssh_config, timeout=120):
	login_info = ssh_config.username + "@" + ssh_config.ip 
	logger.debug("Entering wait_for_remote_host for %s" % ssh_config.ip)
	success = False
	start_time = time.time()
	while not success and time.time() < (start_time + timeout):
		p = subprocess.Popen(["/usr/bin/ssh", login_info, "echo 1"], stdin=None, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=False)
		(out, err) = p.communicate()
		logger.debug("Received stdout=%s stderr=%s" % (out, err))
		if p.returncode == 0:
			success = True

	return success

def remote_check_for_pid(ssh_config, pid):
	(_, _, rcode) = proc_launch_ssh(ssh_config, "/usr/bin/ps", "-p " + str(pid),  stdin=None, stdout=False, stderr=False, background=False)
	if rcode == 0:
		return True
	else:
		return False

def remote_proc_communicate(remote_proc):
	while remote_check_for_pid(remote_proc.target, remote_proc.pid):
		time.sleep(1)

	# Get return code TODO
	#print proc_launch_ssh(remote_proc.target, "wait", str(remote_proc.pid), stdin=None, stdout=False, stderr=False, background=False)

	local_stdout = "/tmp/out_" + str(time.time())
	local_stderr = "/tmp/err_" + str(time.time())

	# Get stdout and stderr
	remote_get(remote_proc.target, remote_proc.remote_stdout, local_stdout, False)
	remote_get(remote_proc.target, remote_proc.remote_stderr, local_stderr, False)

	with open(local_stdout, "r") as f:
		stdout = f.readlines()

	with open(local_stderr, "r") as f:
		stderr = f.readlines()

	if stdout:
		stdout = stdout[0]
	else:
		stdout = ""

	if stderr:
		stderr = stderr[0]
	else:
		stderr = ""


	return (stdout, stderr, 0)
