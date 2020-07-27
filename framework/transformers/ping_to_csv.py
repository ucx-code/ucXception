import re
import logging
from core import utils
logger = logging.getLogger(__name__)


class Ping_to_CSV:

    def run(self, target, paths, keep=True):
        logger.info("Starting Ping to CSV transformer for files %s" % paths)

        new_paths = []
        for path in paths:
            csv = [("ts,target,id,lat\n"),]
            with open(path, "r") as f:
                for l in f.readlines()[1:]:
                    #print l
                    out = re.match(r"\[([0-9]*\.[0-9]*)] [0-9][0-9] bytes from (.*): icmp_seq=([0-9]*) ttl=([0-9]*) time=([0-9]*.?[0-9]*) ms\n",
                        l)
                    if out is not None:
                        out = out.groups()
                        csv.append(("%s,%s,%s,%s\n" % (out[0], out[2], out[3], out[4])),)
            new_path = ".".join(path.split(".")[:-1])+".csv"
            if not keep:
                utils.local_delete(path)

            with open(new_path, "w") as f:
                f.writelines(csv)
                f.flush()
            new_paths.append(new_path)

        logger.info("Leaving Ping to CSV transformer")
        return None if new_paths == [] else new_paths
