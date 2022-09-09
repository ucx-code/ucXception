# Verifies that each experiemnt has a failure classification (failure mode)
import pandas as pd
import sys
sys.path.append('../..')
from put_into_tabular import *


if (len(sys.argv) == 2):
	filename = sys.argv[1]

	df = pd.read_csv(filename, header=0, index_col=0,  na_filter=False)
	
	data = df.loc[: , ["failure_class"]]

	for index, row in data.iterrows():
		failure = row[0]
		
		assert(failure != None)
		assert(failure != "")
	print "Success"
