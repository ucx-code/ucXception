# Returns simple stats about the runs (e.g., min, max and median runtime) per (language, workload) pair

import pandas as pd
import sys
from pandasql import sqldf
sys.path.append('..')
from put_into_tabular import *

if (len(sys.argv) == 2):
	filename = sys.argv[1]

        df = pd.read_csv(filename, header=0, index_col=0, dtype=schema, na_filter=False)

	query = "SELECT language, workload, count(1),  min(run_duration), max(run_duration) FROM df GROUP BY language, workload"
	results = sqldf(query, locals())

	print results


