class SSH_Config:
	ip=None
	username=None

	def __init__(self, ip, username):
		self.ip = ip
		self.username = username

def ssh(ip, username):
	return SSH_Config(ip, username)