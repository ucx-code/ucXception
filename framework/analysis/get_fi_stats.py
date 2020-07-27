# Presents basic failure mode and probabilities analysis

import pandas as pd
import sys
from binomial_confidence_stackoverflow import *
sys.path.append('..')
from put_into_tabular import *


if (len(sys.argv) == 2):
	filename = sys.argv[1]

	df = pd.read_csv(filename, header=0, index_col=0, dtype=schema, na_filter=False)
	
	triples = df.loc[: , ["language", "workload", "failure_class"]]

	failures = {} # absoloute failure modes freq.
	failures_perc = {} # percentage failue modes
	totals = {} # total (language, workload) entries

	for key, value in triples.iterrows():
		language = value.loc[:].astype(str).values[0]
		workload = value.loc[:].astype(str).values[1]
		failure_class = value.loc[:].astype(str).values[2]
		entry = (language, workload, failure_class)
		entry2 = (language, workload)

		if (entry in failures):
			failures[entry] = failures[entry] + 1
		else:
			failures[entry] = 1

		if (entry2 in totals):
			totals[entry2] = totals[entry2] + 1
		else:
			totals[entry2] = 1

	for key in failures:
		entry2 = (key[0], key[1])
		failures_perc[key] = failures[key] / (totals[entry2]*1.0)

	# Sort the order before presenting. you can change the sort key freely
	sort_key = sorted(list(failures_perc), key=lambda x: (x[0],x[1], x[2]))
	for i in sort_key:
		reduced_i = (i[0], i[1])
		(_, lower_conf, upper_conf) = binomial_hpdr(failures[i], totals[reduced_i], 0.95)
		print  str(i).ljust(40), ("---> %.3f. 95perc confidence [%.4f, %.4f]. %3s out of %d" % (failures_perc[i], lower_conf, upper_conf, str(failures[i]), totals[reduced_i]))