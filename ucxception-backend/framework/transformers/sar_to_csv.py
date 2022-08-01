import re
import logging
from framework.core import utils
logger = logging.getLogger(__name__)

import pandas as pd
import io

class Sar_to_CSV:

    def run(self, target, paths, keep=True):
        logger.info("Starting Sar to CSV transformer for files %s" % paths)

        new_paths = []
        for path in paths:
            cmd = "-U -d %s -- -b -B -q -r -S -u" % path
            
            (out, _, rcode) = utils.run_anywhere(target, "sadf", cmd, True, None, False)
            
            # Divide the output into several parts, according to headers
            parts = out.split("#")
            dfs = []
            for part in parts[1:]:
                tmp = pd.read_csv(io.StringIO(part.lstrip()), sep=";", header=0, decimal=',', index_col='timestamp')
                # Because of join() we need to delete the repeated columns, which are also useless for us...
                del tmp["interval"]
                del tmp["hostname"]
                dfs.append(tmp)

            final = dfs[0].join(dfs[1])
            new_path = ".".join(path.split(".")[:-1])+".csv"
            final.to_csv(new_path, sep=";")
            new_paths.append(new_path)
            
        logger.info("Leaving Sar to CSV transformer")
        return None if new_paths == [] else new_paths
