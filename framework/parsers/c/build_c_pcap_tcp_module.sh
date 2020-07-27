#!/usr/bin/env bash

NOW=`pwd`
DIR="$(cd "$(dirname "$0")" && pwd)"

cd $DIR;

rm -rf tmp_c; mkdir tmp_f;
cd tmp_f;
cp ../pcap_tcp_c_parser.i .;
cp ../pcap_tcp_c_parser.c .;

swig -c++ -python pcap_tcp_c_parser.i
g++ -O3 -c pcap_tcp_c_parser.c
g++ -c -fPIC pcap_tcp_c_parser.c pcap_tcp_c_parser_wrap.cxx -I/usr/include/python2.7/
g++ -shared  pcap_tcp_c_parser.o pcap_tcp_c_parser_wrap.o -o _pcap_tcp_c_parser.so

cp -f _pcap_tcp_c_parser.so ..;
cp -f pcap_tcp_c_parser.py ..;
cd ..;
rm -rf tmp_f;

cd $NOW;
