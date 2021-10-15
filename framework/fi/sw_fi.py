import os
import subprocess
import logging

from core import utils

logger = logging.getLogger(__name__)

def launch_fi(target, fi_path, source_file, preprocess=True, check_extension=True, operator=None, c_parser=True):
	logger.info("Launching ucXception SW FI for generating patch files of %s", source_file)

	flags = ["-jar", fi_path, source_file, "-s"]
	if not preprocess:
		flags.append("--no-preprocessing")
	if not check_extension:
		flags.append("--ignore-extension")
	if operator:
		flags.append("-o=" + operator)
	if not c_parser:
		flags.append("-cpp")

	(output, rcode) = utils.run_anywhere(target, "java", " ".join(flags), None, None, False)
	logger.debug(output)
	logger.debug(rcode)

	local_dir = os.path.dirname(source_file)
	patch_dir = "/tmp/patch_storage"
	# Creates folder to store patch files while run is ongoing
	utils.mkdir_anywhere(target, patch_dir)

	patch_list = []
	# Moves all generated paths to /tmp/folder, restores original source code and removes uneeded files
	for file in utils.listdir_anywhere(target, local_dir):
		src_path = os.path.join(local_dir, file)
		dst_path = os.path.join(patch_dir, file)
		if ".patch" in file:
			utils.move_anywhere(target, src_path, dst_path)
			patch_list.append(dst_path)
		elif "._FORMATTED_" in file:
			utils.move_anywhere(target, src_path, dst_path)

	return (None, patch_list)