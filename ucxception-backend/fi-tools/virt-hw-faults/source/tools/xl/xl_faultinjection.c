#include <fcntl.h>
#include <inttypes.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include <libxl.h>
#include <libxl_utils.h>
#include <libxlutil.h>

#include "xl.h"
#include "xl_utils.h"
#include "xl_parse.h"

#define __HYPERVISOR_VIRT_START 0xFFFF800000000000
#define __HYPERVISOR_VIRT_END   0xFFFF880000000000

int main_inject_fault(int argc, char **argv)
{
	unsigned long domid, bit, reg, range_start, range_end;

	// By default range_start and range_end limits injections to the hypervisor region
	range_start = __HYPERVISOR_VIRT_START;
	range_end = __HYPERVISOR_VIRT_END;

	if (argc != 4)
	{
		fprintf(stderr, "Expecting <domid> <bit> <reg> [range_start] [range_end]\n");
		return -1;
	}

	sscanf(argv[1], "%lu", &domid);
	sscanf(argv[2], "%lu", &bit);
	sscanf(argv[3], "%lu", &reg);
	if (argc ==  6) {
		sscanf(argv[4], "%lu", &range_start);
		sscanf(argv[5], "%lu", &range_end);
	}
	return libxl_inject_fault(ctx, domid, bit, reg, range_start, range_end);
}
