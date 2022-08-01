import requests
import time

endpoint = "http://localhost:5000/exec/current"

# Start a new execution
req = requests.post(endpoint, data = {'start_time' : time.time(), 'n_campaigns' : 10, 'n_runs' : 400})
print req.status_code, req.text
assert(req.status_code == 200)

# A FI run has finished
req = requests.put(endpoint, data = {'current_run' : 2, 'current_campaign' : 1})
print req.status_code, req.text
assert(req.status_code == 200)

# The execution has finished
req = requests.delete(endpoint)
print req.status_code, req.text
assert(req.status_code == 200)
