""" Emulates a failure by rebooting the PC. Not a very interesting fault model for our purposes, but can be used for
testing.
"""


import logging

from core import utils

logger = logging.getLogger(__name__)

def launch_fi(target, path, pid, input):
    logger.info("Launching hard reboot injector")

    #reboot --no-sync --no-wtmp --no-wall -f; reboot --no-sync --no-wtmp --no-wall -f;
    # Some systems do not have --no-sync
    utils.run_anywhere(target, "reboot", "--no-wtmp --no-wall -f", False, None, False)

    logger.info("Leaving hard reboot injector")
