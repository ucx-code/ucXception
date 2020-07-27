import numpy as np

def map_reg_to_text(reg_code):
	reg_dict = ("rip", "rsp", "rax", "rbx", "rcx", "rdx", "cs", "ss", "eflags", "rbp", "r8", "r9",
				"r10", "r11", "r12", "r13", "r14", "r15", "rsi", "rdi", "orig_rax", "fs_base", "gs_base",
				"ds", "es", "fs", "gs")

	return reg_dict[reg_code]


class ucXception_fi_parser:
	
	def parse(self, inj_time, reg, bit, chosen_thread, stdout, stderr):
		row = {}

		row["inj_time"] = inj_time
		row["reg"] = map_reg_to_text(reg)
		row["reg_d"] = np.int32(reg)
		row["bit"] = np.int32(bit)
		row["pid"] = np.int32(chosen_thread)

		# Get the values of old and new registers
		prefix = "none"
		for line in stdout.split("\n")[:-1]:
			if "Old register values" in line:
				prefix="old_"
			elif "New register values" in line:
				prefix="new_"
			else:	
				(reg_name, reg_val) = line.split(": ")
				reg_name = reg_name.rstrip().lower()
				reg_val = "0x%s" % reg_val.rstrip()
				#print reg_name, reg_val, type(reg_val)
				row[prefix + reg_name] = reg_val
				# We also add the register value in decimal
				row[prefix + reg_name + "_d"] = np.int(reg_val, 16) # np.int64 gives a strange exception--- (numpy bug?)
		return row