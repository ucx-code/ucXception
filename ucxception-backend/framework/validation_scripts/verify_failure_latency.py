# Verifies that the desired injection times have been obeyed

import pandas as pd
import sys
sys.path.append('../..')
from put_into_tabular import *


if (len(sys.argv) == 3):
	filename = sys.argv[1]
	operation = sys.argv[2]
	assert(operation == "assert" or operation == "remove")
	
	df = pd.read_csv(filename, header=0, index_col=0,  na_filter=False)
	df = df.reset_index(drop=True)

	data = df.loc[: , ["inj_rel_time", "run_duration", "start_time"]]
	drop_counts = 0
	for index, row in data.iterrows():
		# startTime and runDuration in seconds
		# injRelTime in ms
		(injRelTime, runDuration, startTime) = row
		absInjTime = startTime + (injRelTime/1000.0)
		totalTime = startTime + runDuration

		print "Failure latency was", (totalTime-absInjTime)
		if (operation == "assert"):
			assert(absInjTime > startTime)
			assert(absInjTime < totalTime)
		elif (operation == "remove"):
			if (absInjTime <= startTime) or (absInjTime >= totalTime):
				df = df.drop(index)
				drop_counts = drop_counts + 1
	if (operation == "remove") and (drop_counts > 0):
		print "Writing new CSV to", filename + "_modified"
		print "Dropped", drop_counts, "rows"
		df.to_csv(filename+"_modified", quotechar='"', quoting=csv.QUOTE_NONNUMERIC, index=False, float_format='%.9f')

	print "Success"
else:
	print "Usage: python verify_filename_latency.py <filename.csv> <assert|remove>"
