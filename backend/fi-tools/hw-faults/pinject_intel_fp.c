#include <time.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
//include <linux/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <sys/procfs.h>


/*
struct user_fpregs_struct
{
  unsigned short int        cwd;
  unsigned short int        swd;
  unsigned short int        ftw;
  unsigned short int        fop;
  __extension__ unsigned long long int rip;
  __extension__ unsigned long long int rdp;
  unsigned int                mxcsr;
  unsigned int                mxcr_mask;
  unsigned int                st_space[32];   // 8*16 bytes for each FP-reg = 128 bytes 
  unsigned int                xmm_space[64];  // 16*16 bytes for each XMM-reg = 256 bytes 
  unsigned int                padding[24];
};


*/
void
show_regs2(elf_fpregset_t * regs) {
	printf("CWD: %08x\n", regs->cwd);
	printf("SWD: %08x\n", regs->swd);
	printf("FTW: %08x\n", regs->ftw);
	printf("FOP: %08x\n", regs->fop);
	printf("RIP: %016lx\n", regs->rip);
	printf("RDP: %016llx\n", regs->rdp);
	printf("MXCSR: %08x\n", regs->mxcsr);
	printf("MXCR_MASK: %08x\n", regs->mxcr_mask);
	for (int i = 0; i < 32; i++) {
		printf("ST[%d]: %08x\n", 
i, regs->st_space[i]);
	}
	for (int i = 0; i < 64; i++) {
		printf("XMM[%d]: %08x\n", 
i, regs->xmm_space[i]);
	}
for (int i = 0; i < 24; i++) {
		printf("Padding[%d]: %08x\n", 
i, regs->padding[i]);
	}

}

unsigned long long
do_bitflip(unsigned long long reg, int bit_pos);

inline unsigned long long
do_bitflip(unsigned long long reg, int bit_pos) {
    reg ^= 1 << bit_pos;
    return reg;
}

int
main(int argc, char **argv) {
    long ret = -2;
    int status = 0, pid;
    elf_fpregset_t fpregs[32];

#ifdef _PRINT_REGS_
	elf_fpregset_t fpregs_old[32];

#endif
#ifdef _PRINT_TS_
#define BILLION  1000000000L
#define T(X) (X.tv_sec * BILLION + X.tv_nsec)
    struct timespec now2;
#endif

    int bit_pos = 0, register_type = -1;
    unsigned long sleep_time = -1;

    if (argc != 5) {
        fprintf
                (stderr, "Incorrect amount of parameters\nShould be <pid> <register_type> <bit_pos> <sleep_time>\n");
        return -2;
    }


    pid = atoi(argv[1]);
    register_type = atoi(argv[2]);
    bit_pos = atoi(argv[3]);
    sleep_time = atol(argv[4]);

    struct timespec t1, t2;

    t1.tv_sec = (sleep_time / 1000);
    t1.tv_nsec = (sleep_time % 1000) * 1000000;

    // Waits until we can inject
    if (nanosleep(&t1, &t2) < 0) {
        fprintf(stderr, "Nano sleep system call failed \n");
        return -1;
    }

    // Attach to the process
    ret = ptrace(PTRACE_ATTACH, pid, 0, 0);
    if (ret == -1) {
        fprintf(stderr, "Error while attaching to %d\n", pid);
        return -1;
    }

    wait(&status);

    // Obtain current register values
    unsigned long res = ptrace(PTRACE_GETFPREGS, pid, 0, (long) &fpregs);
	assert(res >= 0);
#ifdef _PRINT_REGS_
    memcpy(&fpregs_old, &fpregs, sizeof(fpregs));
#endif
    // Perform the bitflip in the desired register and bit
	 switch (register_type) {
		case 0:
		    fpregs->cwd = do_bitflip(fpregs->cwd, bit_pos);
		    break;
		case 1:
		    fpregs->swd = do_bitflip(fpregs->swd, bit_pos);
		    break;	
		case 2:
		    fpregs->ftw = do_bitflip(fpregs->ftw, bit_pos);
		    break;	
		case 3:
		    fpregs->fop = do_bitflip(fpregs->fop, bit_pos);
		    break;	
		case 4:
		    fpregs->rip = do_bitflip(fpregs->rip, bit_pos);
		    break;	
		case 5:
		    fpregs->rdp = do_bitflip(fpregs->rdp, bit_pos);
		    break;
		case 6:
		    fpregs->mxcsr = do_bitflip(fpregs->mxcsr, bit_pos);
		    break;
		case 7:
		    fpregs->mxcr_mask = do_bitflip(fpregs->mxcr_mask, bit_pos);
		    break;
		case 8 ... 39:
			fpregs->st_space[register_type - 8] = do_bitflip(fpregs->st_space[register_type - 8], bit_pos);
		    break;
		case 40 ... 103:
			fpregs->xmm_space[register_type - 40] = do_bitflip(fpregs->xmm_space[register_type - 40], bit_pos);
			break;	
	}
    // Update new register values
    ptrace(PTRACE_SETFPREGS, pid, 0, &fpregs);
#ifdef _PRINT_TS_
    clock_gettime(CLOCK_REALTIME, &now2);
#endif
    ptrace(PTRACE_DETACH, pid, 0, 0);

#ifdef _PRINT_REGS_
    printf ("Old register values\n");
    show_regs2 (&fpregs_old);

    printf ("New register values\n");
    show_regs2 (&fpregs);
#endif

#ifdef _PRINT_TS_
    printf("Timestamp: %ld\n", T(now2));
#endif
    return 0;
}
