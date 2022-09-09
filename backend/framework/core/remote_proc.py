def create_remote_proc(target, pid, local_stdout, local_stderr, remote_stdout, remote_stderr):
	return Remote_Proc(target, pid, local_stdout, local_stderr, remote_stdout, remote_stderr)

class Remote_Proc():

	def __init__(self, target, pid, local_stdout, local_stderr, remote_stdout, remote_stderr):
		self.target = target
		self.pid = pid
		self.local_stdout = local_stdout
		self.local_stderr = local_stderr
		self.remote_stdout = remote_stdout
		self.remote_stderr = remote_stderr

	target = None
	pid = None
	local_stdout = None
	local_stderr = None
	remote_stdout = None
	remote_stderr = None