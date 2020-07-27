import re
import logging
from core import utils

logger = logging.getLogger(__name__)

import pandas as pd
from StringIO import StringIO


class Pidstat_to_CSV:

    def run(self, target, paths, keep=True):
        logger.info("Starting Pidstat to CSV transformer for files %s" % paths)

        new_paths = []
        for path in paths:
            with open(path, "r") as f:
                x = f.read().split("\n\n")[1:]

            dfs = []
            for y in x[:-1]:
                partido = y.split("\n")
                header = [a for a in filter(None, (partido[0][1:].replace(" ", ";").split(";")))]
                treated_text = [a for a in filter(None, ("\n".join(partido[1:]).replace("  ", ";").split(";")))]
                treated_text = [a.strip() for a in treated_text]
                #print ";".join(treated_text)
                together = ";".join(header) + "\n" + ";".join(treated_text)
                #print together
                tmp = pd.read_csv(StringIO(together),
                                  sep=";", header=0, decimal=',', index_col=["Time", "UID", "TGID", "TID"])
                #print tmp
                dfs.append(tmp)
                
            final = pd.concat(dfs)# dfs[0].join(dfs[1])
            new_path = ".".join(path.split(".")[:-1]) + ".csv"
            final.to_csv(new_path, sep=";")
            new_paths.append(new_path)


        logger.info("Leaving Sar to CSV transformer")
        return None if new_paths == [] else new_paths
