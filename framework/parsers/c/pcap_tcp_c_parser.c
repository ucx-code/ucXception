#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#define HEADER_SIZE 295
#define SEP ';'

int total = 0, acks = 0, fin = 0, reset = 0, syn = 0;
int keep_alive = 0, lost_segment = 0, out_of_order = 0, retr = 0, fast_retr = 0, spur_retr = 0;

void p(const char * data, int begin, int end)
{
	/*  0 frame.time,
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
        */

	total = 0; acks = 0; fin = 0; reset = 0; syn = 0;
	keep_alive = 0; lost_segment = 0; out_of_order = 0; retr = 0; fast_retr = 0; spur_retr = 0;

	int pos = 0;
	for (int i = begin; i < end; ) {
		if (pos == 8) // ack
		{
			acks  += (data[i-1] == '1');
			fin   += (data[i+1] == '1');
			reset += (data[i+3] == '1');
			syn   += (data[i+5] == '1');

			unsigned char t1 = !(data[i+7] == SEP);
			unsigned char t2 = !(data[i+8+t1] == SEP);
			unsigned char t3 = !(data[i+9+t1+t2] == SEP);
			unsigned char t4 = !(data[i+10+t1+t2+t3] == SEP);
			unsigned char t5 = !(data[i+11+t1+t2+t3+t4] == SEP);
			unsigned char t6 = !(data[i+12+t1+t2+t3+t4+t5] == '\n');

			keep_alive += t1;
			lost_segment += t2;
			out_of_order += t3;
			retr += t4;
			fast_retr += t5;
			spur_retr += t6;
			++total;

			pos = 0; i += 13+t1+t2+t3+t4+t5;
		} else	if (data[i] == SEP)
		{
			++pos;
			i+=2;
		} else
		++i;
	}
	//printf("%d %d %d %d %d\n %d %d %d %d %d\n", acks, fin, reset, syn, keep_alive, lost_segment, out_of_order, retr, fast_retr, spur_retr);
}

void parse(char * file)
{
	const char *memblock;
	int fd;
	struct stat sb;

	fd = open(file, O_RDONLY);
	if (fd != -1) {
		fstat(fd, &sb);

		memblock = (const char *) mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		assert(memblock != MAP_FAILED);

		p(memblock, HEADER_SIZE, sb.st_size);
	}
}

int main(int argc, char *argv[])
{
	parse(argv[1]);
}