""" High-Speed C-based parser for Tshark-generated CSV files with the following format:

        0 frame.time,
        1 frame.relative_time,
        2 frame.number,
        3 tcp.stream,
        4 ip.src,
        5 ip.dst,
        6 tcp.srcport,
        7 tcp.dstport,
        8 tcp.flags.ack,
        9 tcp.flags.fin,
        10 tcp.flags.reset,
        11 tcp.flags.syn,
        12 tcp.analysis.keep_alive,
        13 tcp.analysis.lost_segment,
        14 tcp.analysis.out_of_order,
        15 tcp.analysis.retransmission,
        16 tcp.analysis.fast_retransmission,
        17 tcp.analysis.spurious_retransmission

        Using ; as a separator

        e.g., tshark -r dump.pcap -E header=y -E separator=';' -Y 'tcp and (tcp.srcport == 8080 or tcp.dstport == 8080)'
        -T fields -e frame.time_relative -e frame.time -e frame.number -e tcp.stream -e ip.src -e ip.dst -e tcp.srcport
        -e tcp.dstport -e tcp.flags.ack -e tcp.flags.fin  -e tcp.flags.reset -e tcp.flags.syn -e tcp.analysis.keep_alive
         -e tcp.analysis.lost_segment -e tcp.analysis.out_of_order -e tcp.analysis.retransmission
         -e tcp.analysis.fast_retransmission -e tcp.analysis.spurious_retransmission  > dump_tcp.csv
"""
import c.pcap_tcp_c_parser as c_pcap_tcp_c_parser
import numpy as np

class pcap_tcp_parser():
    def parse(self, current_folder, csv_path):
        row = {}
        c_pcap_tcp_c_parser.parse(csv_path[0]) # Uses a C-based app to parse the CSV (through SWIG)

        result=c_pcap_tcp_c_parser.cvar # cant use 'with <> as <>', so we shorten the name like this instead
        row["total"] = np.int64(result.total)
        row["rst"] = np.int64(result.reset)
        row["fin"] = np.int64(result.fin)
        row["ack"] = np.int64(result.acks)
        row["syn"] = np.int64(result.syn)

        row["keep_alive"] = np.int64(result.keep_alive)
        row["lost_seg"] = np.int64(result.lost_segment)
        row["out_order"] = np.int64(result.out_of_order)
        row["retr"] = np.int64(result.retr)
        row["fast_retr"] = np.int64(result.fast_retr)
        row["spur_retr"] = np.int64(result.spur_retr)

        return row

