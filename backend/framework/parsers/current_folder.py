""" Very simple parser that just keeps the results folder name associated with each run.
"""

class Current_Folder():
    def parse(self, folder):
        row = {}

        row["folder_name"] = folder

        return row
