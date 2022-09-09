# Merge all the CSVss
import pandas as pd
import sys 
sys.path.append('..')
from put_into_tabular import *

if (len(sys.argv) > 2):
	n_files = int(sys.argv[1])
	files = []
	for i in xrange(n_files):
		files.append(sys.argv[i+2])
	
	# Load all the CSVs with panda
	data_list = []
	for filename in files:
		try:
                	df = pd.read_csv(filename, header=0, index_col=0, dtype=schema, na_filter=False)
	        except Exception as e:
        	        print "Failed to read CSV file with exception", e

	        for k, v in schema.iteritems():
        	        #print k, v
                	df[k] = df[k].astype(v)

    		data_list.append(df)
	
	all_together = pd.concat(data_list)
	all_together.to_csv("./all_together.csv", quotechar='"', quoting=csv.QUOTE_NONNUMERIC, columns=columns)
