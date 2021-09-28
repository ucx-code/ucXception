#include "xc_private.h"
#include "xc_core.h"
#include "xg_private.h"
#include "xg_save_restore.h"
#include <xen/memory.h>
#include <xen/hvm/hvm_op.h>

int xc_inject_fault(xc_interface *xch, unsigned long domid, unsigned long bit, unsigned long reg, unsigned long range_start, unsigned long range_end)
{
	return xencall5(xch->xcall, __HYPERVISOR_fault_injection, domid, bit, reg, range_start, range_end);
}

