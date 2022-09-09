#include "libxl_osdeps.h"

#include "libxl_internal.h"

int libxl_inject_fault(libxl_ctx *ctx, unsigned long domid, unsigned long bit, unsigned long reg, unsigned long range_start, unsigned long range_end)
{
    return xc_inject_fault(ctx->xch, domid, bit, reg, range_start, range_end);
}
