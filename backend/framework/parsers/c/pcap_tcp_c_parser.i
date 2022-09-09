%module pcap_tcp_c_parser
%{
extern int total, acks, fin, reset, syn;
extern int keep_alive, lost_segment, out_of_order, retr, fast_retr, spur_retr;
extern void parse(char * file);

%}

extern int total, acks, fin, reset, syn;
extern int keep_alive, lost_segment, out_of_order, retr, fast_retr, spur_retr;
extern void parse(char * file);
