from core import utils
import logging
import os

logger = logging.getLogger(__name__)

class Save_Output():
    """ Save output of the app into a file
    """

    def run(self, target, curr_path, app_out):

        logger.info("Storing the app output using the Save_Output transformer")

        stdout_file = os.path.join(curr_path, "app_stdout.txt")
        with open(stdout_file, "w") as f:
            print >> f, app_out[0]
            f.flush()

        if len(app_out) > 1 and app_out[1] is not None:
            stderr_file = os.path.join(curr_path, "app_stderr.txt")
            with open(stderr_file, "w") as f:
                print >> f, app_out[1]
                f.flush()