# Local framework imports
from conf.current import *
import core.io as io
import core.utils as utils
from parsers.info_parser import *

# General imports
import time
import signal
import logging
import sys
import os

def signal_handler(signal, frame):
	print "Saving table to CSV"
	io.save_csv(main_csv_path, table)

	# Telling the REST service that we are terminating
	# if (rest_enabled): req = requests.delete(endpoint)
	sys.exit(0)

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# ====== main() ============
#assert(os.getuid() == 0)

logger.info("Starting ucXception framework")

# Lets load the previous results
table = io.read_or_create_table(main_csv_path, predefined_cols, False)

# When we receive a Ctrl+C lets save our results before existing
signal.signal(signal.SIGINT, signal_handler)

for plan in plans:
	for campaign in plan.campaigns:
		(campaign_name, n_runs, input) = campaign

		for run in xrange(1, n_runs+1):
			logger.info("Doing run %d out of %d wrt. cmp. %s of plan %s" % (run, n_runs, campaign_name, plan))

			start_time = time.time() # In version >= 3.3 there are better options for high performance counters
			logger.debug("start_time: %f" % start_time)

			row = plan(input).run() # Execute the steps of this campaign

			end_time = time.time() # In version >= 3.3 there are better options for high performance counters
			
			if row is not None:
				row.update(info_parser().parse(campaign_name, run, start_time, end_time))
				table = io.add_row(table, row)
			else:
				logger.warning("main: we didnt include this run because something failed before...")
			logger.debug("end_time: %f" % start_time)

	io.save_csv(main_csv_path, table) # Save between campaigns at least, just in case something fails badly
io.save_csv(main_csv_path, table)

# ===========================
