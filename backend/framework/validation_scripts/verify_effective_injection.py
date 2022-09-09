# Verifies that one and only one bitflip was performed in the registers

import pandas as pd
import sys
sys.path.append('../..')
from put_into_tabular import *


if (len(sys.argv) == 2):
	filename = sys.argv[1]

	df = pd.read_csv(filename, header=0, index_col=0,  na_filter=False)
	#data = df.loc[: , ["start_time", "inj_rel_time", "run_duration"]]

	registers = ("rip", "rax", "rbx", "rcx", "rdx", "cs", "ss", "eflags", "rbp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "rdi", "orig_rax", "fs_base", "ds", "es",  "rsp", "gs") #gs_base, fs ignored

	"""query = ""
	for i in registers:
		if (query == ""):
			query = "(new_"  + i + " != old_" + i + ")"
		else:
			query = query + " ^ (new_"  + i + " != old_" + i + ")"
	#query = query + ")"
	print query
	print df.query(query)"""

	regList=[]
	for i in registers:
		new = "new_" + i
		old = "old_" + i
		regList.append(new); regList.append(old);
	regList.append("register")

	data = df.loc[: , regList]
	for index, row in data.iterrows():
		thereWasAnInjection = []
		for i in registers:
			new = "new_"+i
			old = "old_"+i

			val_new = row[new]
			val_old = row[old]

			if (val_new != val_old):
				thereWasAnInjection.append(i)
		if (len(thereWasAnInjection) != 1):
			if (len(thereWasAnInjection) == 0) and ((row["register"] == "fs") or (row["register"] == "gs_base")):
				pass
			else:
				print "PROBLEM ", thereWasAnInjection, row["register"]
				#print row

