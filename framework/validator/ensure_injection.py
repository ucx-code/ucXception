
""" Ensures that an injection (bitflip) really occurred by looking at the old vs new register values
"""
import logging

logger = logging.getLogger(__name__)

class Ensure_Injection():

    register_cols = ("rip", "rsp", "rax", "rbx", "rcx", "rdx", "cs", "ss", "eflags", "rbp", "r8", "r9",
                     "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "rdi", "orig_rax", "fs_base", "gs_base",
                     "ds", "es", "fs", "gs")

    def run(self, fi_row, prefix=""):
        logger.info("Starting the ensure_injection validator")

        if ("reg" not in fi_row) or (fi_row["reg"] == ""):
            logger.warning("ensure_injection: Register is empty or not defined")
            return False

        if ("bit" not in fi_row) or (fi_row["bit"] == ""):
            logger.warning("ensure_injection: Bit is empty or not defined")
            return False

        passed = True
        for reg in self.register_cols:
            old = fi_row["%sold_%s" % (prefix, reg)]
            new = fi_row["%snew_%s" % (prefix, reg)]

            if (old != new) and (fi_row["reg"] != reg):
                passed = False
                logger.warning("ensure_injection: Register %s changed from %s to %s when injection took place "
                               "on reg. %s" % (reg, old, new, fi_row["reg"]))

        logger.info("Leaving ensure_injection validator with passed=%s" % passed)
        return passed
