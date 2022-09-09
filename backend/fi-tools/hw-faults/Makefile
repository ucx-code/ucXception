CCFLAGS=-O3 -Os -Wall
DEFS= -D_PRINT_REGS_ -D_PRINT_TS_

intel: pinject_intel_v2.c
	gcc ${CCFLAGS} ${DEFS} pinject_intel_v2.c -o pinject_intel

intel_fp: pinject_intel_fp.c
	gcc ${CCFLAGS} ${DEFS} pinject_intel_fp.c -o pinject_fp

deadline: pinject_intel_deadline.c
	gcc ${CCFLAGS} ${DEFS} pinject_intel_deadline.c -o pinject_intel_deadline

arm: pinject_arm.c
	gcc ${CCFLAGS} ${DEFS} pinject_arm.c -o pinject_arm

all:
	${MAKE} intel deadline intel_fp

clean:
	rm -f pinject_intel pinject_intel_deadline
