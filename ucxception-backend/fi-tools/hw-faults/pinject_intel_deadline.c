#include <time.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//include <linux/user.h>
#include <sys/types.h>
#include <sys/wait.h>


void
show_regs2(struct user_regs_struct *regs) {
    printf("RIP: %016llx\n", regs->rip);
    printf("RSP: %016llx\n", regs->rsp);
    printf("RBP: %016llx\n", regs->rbp);
    printf("RAX: %016llx\n", regs->rax);
    printf("RBX: %016llx\n", regs->rbx);
    printf("RCX: %016llx\n", regs->rcx);
    printf("RDX: %016llx\n", regs->rdx);
    printf("R8: %016llx\n", regs->r8);
    printf("R9: %016llx\n", regs->r9);
    printf("R10: %016llx\n", regs->r10);
    printf("R11: %016llx\n", regs->r11);
    printf("R12: %016llx\n", regs->r12);
    printf("R13: %016llx\n", regs->r13);
    printf("R14: %016llx\n", regs->r14);
    printf("R15: %016llx\n", regs->r15);
    printf("RSI: %016llx\n", regs->rsi);
    printf("RDI: %016llx\n", regs->rdi);
    printf("CS: %016llx\n", regs->cs);
    printf("SS: %016llx\n", regs->ss);
    printf("DS: %016llx\n", regs->ds);
    printf("ORIG_RAX: %016llx\n", regs->orig_rax);
    printf("FS_BASE: %016llx\n", regs->fs_base);
    printf("GS_BASE: %016llx\n", regs->gs_base);
    printf("ES: %016llx\n", regs->es);
    printf("FS: %016llx\n", regs->fs);
    printf("GS: %016llx\n", regs->gs);
    printf("EFLAGS: %016llx\n", regs->eflags);
}

unsigned long long
do_bitflip(unsigned long long reg, int bit_pos);

inline unsigned long long
do_bitflip(unsigned long long reg, int bit_pos) {
    reg ^= 1 << bit_pos;
    return reg;
}

#define BILLION  1000000000L
#define T(X) (X.tv_sec * BILLION + X.tv_nsec)

int
main(int argc, char **argv) {
    long ret = -2;
    int status = 0, pid;
    struct user_regs_struct uregs;
#ifdef _PRINT_REGS_
    struct user_regs_struct uregs_old;
#endif

    int bit_pos = 0, register_type = -1;
    long desired_deadline = -1;
    struct timespec now;
#ifdef _PRINT_TS_
    struct timespec now2;
#endif
    long sleep_time;
    struct timespec t1;

    if (argc != 5) {
        fprintf
                (stderr, "Incorrect amount of parameters\nShould be <pid> <register_type> <bit_pos> <deadline>\n");
        return -2;
    }

    pid = atoi(argv[1]);
    register_type = atoi(argv[2]);
    bit_pos = atoi(argv[3]);
    desired_deadline = atol(argv[4]);

    clock_gettime(CLOCK_REALTIME, &now);

    sleep_time = desired_deadline - T(now);

    t1.tv_sec = (sleep_time / BILLION);
    t1.tv_nsec = (sleep_time % BILLION);

    // Waits until we can inject
    if (nanosleep(&t1, NULL) < 0) {
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
    ptrace(PTRACE_GETREGS, pid, 0, &uregs);

#ifdef _PRINT_REGS_
    memcpy(&uregs_old, &uregs, sizeof(struct user_regs_struct));
#endif

    // Perform the bitflip in the desired register and bit
    switch (register_type) {
        case 0:
            uregs.rip = do_bitflip(uregs.rip, bit_pos);
            break;
        case 1:
            uregs.rsp = do_bitflip(uregs.rsp, bit_pos);
            break;
        case 2:
            uregs.rax = do_bitflip(uregs.rax, bit_pos);
            break;
        case 3:
            uregs.rbx = do_bitflip(uregs.rbx, bit_pos);
            break;
        case 4:
            uregs.rcx = do_bitflip(uregs.rcx, bit_pos);
            break;
        case 5:
            uregs.rdx = do_bitflip(uregs.rdx, bit_pos);
            break;
        case 6:
            uregs.cs = do_bitflip(uregs.cs, bit_pos);
            break;
        case 7:
            uregs.ss = do_bitflip(uregs.ss, bit_pos);
            break;
        case 8:
            uregs.eflags = do_bitflip(uregs.eflags, bit_pos);
            break;
        case 9:
            uregs.rbp = do_bitflip(uregs.rbp, bit_pos);
            break;
        case 10:
            uregs.r8 = do_bitflip(uregs.r8, bit_pos);
            break;
        case 11:
            uregs.r9 = do_bitflip(uregs.r9, bit_pos);
            break;
        case 12:
            uregs.r10 = do_bitflip(uregs.r10, bit_pos);
            break;
        case 13:
            uregs.r11 = do_bitflip(uregs.r11, bit_pos);
            break;
        case 14:
            uregs.r12 = do_bitflip(uregs.r12, bit_pos);
            break;
        case 15:
            uregs.r13 = do_bitflip(uregs.r13, bit_pos);
            break;
        case 16:
            uregs.r14 = do_bitflip(uregs.r14, bit_pos);
            break;
        case 17:
            uregs.r15 = do_bitflip(uregs.r15, bit_pos);
            break;
        case 18:
            uregs.rsi = do_bitflip(uregs.rsi, bit_pos);
            break;
        case 19:
            uregs.rdi = do_bitflip(uregs.rdi, bit_pos);
            break;
        case 20:
            uregs.orig_rax = do_bitflip(uregs.orig_rax, bit_pos);
            break;
        case 21:
            uregs.fs_base = do_bitflip(uregs.fs_base, bit_pos);
            break;
        case 22:
            uregs.gs_base = do_bitflip(uregs.gs_base, bit_pos);
            break;
        case 23:
            uregs.ds = do_bitflip(uregs.ds, bit_pos);
            break;
        case 24:
            uregs.es = do_bitflip(uregs.es, bit_pos);
            break;
        case 25:
            uregs.fs = do_bitflip(uregs.fs, bit_pos);
            break;
        case 26:
            uregs.gs = do_bitflip(uregs.gs, bit_pos);
            break;
            //default:
    }

    // Update new register values
    ptrace(PTRACE_SETREGS, pid, 0, &uregs);

#ifdef _PRINT_TS_
    clock_gettime( CLOCK_REALTIME, &now2 );
#endif

    ptrace(PTRACE_DETACH, pid, 0, 0);

#ifdef _PRINT_REGS_
    printf ("Old register values\n");
    show_regs2 (&uregs_old);
    printf ("New register values\n");
    show_regs2 (&uregs);
#endif

#ifdef _PRINT_TS_
    printf("Timestamp: %ld\n", T(now2));
#endif

    return 0;
}
