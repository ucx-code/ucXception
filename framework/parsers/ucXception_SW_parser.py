import os

class ucXception_SW_Parser:
	
	def parse(self, patch_files, patch_file):
		splitted = patch_file.split("_")
		row = {}

		row["total_patches"] = len(patch_files)

		patch_count = {}
		for patch in patch_files:
			operator = os.path.basename(patch).split("_")[-2]
			if operator in patch_count:
				patch_count[operator] = patch_count[operator] + 1
			else:
				patch_count[operator] = 1
		for operator, count in patch_count.iteritems():
			row["operator_" + operator] = count

		row["source_file"] = os.path.basename(splitted[0])
		row["patch_file"] = patch_file
		row["operator"] = splitted[1]

		return row