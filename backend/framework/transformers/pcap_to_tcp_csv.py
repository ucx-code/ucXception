""" Extracts the TCP related information from a raw PCAP file. Can also put each row into a separate temporal bin, for
    temporal analysis.
"""
import logging

from core import utils

logger = logging.getLogger(__name__)


class Pcap_to_TCP_CSV():

    def run(self, target, paths, tshark_filter="tcp", background=False):
        logger.info("Starting PCAP to TCP CSV transformer for file %s" % paths)
        new_paths = []
        for path in paths:
            params = ["-r", "%s" % path, "-E", "header=y", "-E", "separator=;", "-Y", "%s" % tshark_filter,
                               "-T", "fields", "-e", "frame.time_relative", "-e", "frame.time", "-e", "frame.number",
                               "-e", "tcp.stream", "-e", "ip.src", "-e", "ip.dst", "-e", "tcp.srcport", "-e", "tcp.dstport",
                               "-e", "tcp.flags.ack", "-e", "tcp.flags.fin", "-e", "tcp.flags.reset", "-e", "tcp.flags.syn",
                               "-e", "tcp.analysis.keep_alive", "-e", "tcp.analysis.lost_segment",
                               "-e", "tcp.analysis.out_of_order", "-e", "tcp.analysis.retransmission",
                               "-e", "tcp.analysis.fast_retransmission", "-e", "tcp.analysis.spurious_retransmission"]
            new_path = ".".join(path.split(".")[:-1])+"_tcp.csv"
            utils.run_anywhere(target, "tshark", params, new_path, None, background)
            new_paths.append(new_path)

        logger.info("Leaving PCAP to TCP CSV transformer")
        return None if new_paths == [] else new_paths
