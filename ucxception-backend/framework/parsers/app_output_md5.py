import hashlib
import numpy as np

class App_Output_Md5:
	def parse(self, current_folder, app_output, expected_len, expected_md5):
		row = {}
		
		if type(app_output) is tuple:
			stdout = app_output[0]
		else:
			stdout = app_output

		stdout = stdout.encode()
		md5_hash = hashlib.md5(stdout).hexdigest()
	
		# Compare whether less or more output was produced than expected
		diff_output_len = len(stdout) - expected_len
		row["output_dist"] = np.int64(diff_output_len)
		row["output_len"] = np.int64(len(stdout))
		if md5_hash == expected_md5:
			row["output_matches"] = "True"
		else:
			row["output_matches"] = "False"
		return row
