""" Very simple parser that just keeps the results folder name associated with each run.
"""
import numpy as np

class App_Returncode():
    def parse(self, current_folder, rcode):
        row = {}

        row["app_rcode"] = np.int32(rcode)

        return row
