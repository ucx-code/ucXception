import pandas as pd
import numpy as np
import csv
import logging

logger = logging.getLogger(__name__)

def read_or_create_table(path, columns, typecasting = True):

	hasSchema = False
	try:
		with open(path + ".schema", "r") as f:
			dtypes = {}
			for line in f,readlines():
				(col, typ) = line.split(" ")
				dypes[col] = typ.rstrip()
			hasSchema = True
	except:
		pass

	try:
		if hasSchema:
			table = pd.read_csv(path, header=0, na_filter=False, dtype=dtypes, float_precision="round_trip", quotechar='"', quoting=csv.QUOTE_NONNUMERIC)
		else:
			table = pd.read_csv(path, header=0, na_filter=False, float_precision="round_trip", quotechar='"', quoting=csv.QUOTE_NONNUMERIC)
	except Exception as e:
		logger.info("Failed to read CSV file with exception", e)
		table = pd.DataFrame(columns=columns)

		"""if (typecasting):
			for k, v in schema.iteritems():
				table[k] = table[k].astype(v)"""
	return table 

def save_csv(path, table):
	if not table.empty:
		#if (enforce):
		table.to_csv(path, quotechar='"', quoting=csv.QUOTE_NONNUMERIC, index=False)

		# Store the schema for future use...
		with open(path + ".schema", "w") as f:
			for dt in table.columns:
				print(f, dt, str(table[dt].dtype), file=f)

def enforce_schema(table, row):
	for i in row:
		#print i, type(row[i])
		if type(row[i]) == type(np.int64()) or type(row[i]) == type(np.int()):
			table[i] = table[i].astype("int64")
		elif type(row[i]) == type(np.int32()):
				table[i] = table[i].astype("int32")
		elif type(row[i]) == type(float):
			table[i] = table[i].astype("float64")
		elif type(row[i]) == type(bool):
			table[i] = table[i].astype("bool")
	return table

## TODO: Profile this. could be very slow (?)
def add_row(table, row):
	res = table.append(row, ignore_index=True)
	#res = enforce_schema(res, row)

	return res
