import numpy as np

class info_parser:
	def parse(self, current_folder, campaign_name, run_n, start_time, end_time):
		row = {}

		row["camp_name"] = campaign_name
		row["run_n"] = np.int32(run_n)
		row["start_time"]  = float(start_time)
		row["end_time"] = float(end_time)
		row["duration"] = float(end_time - start_time)
		
		return row
