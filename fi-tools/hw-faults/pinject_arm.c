#include <time.h>
//include <sys/reg.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>


void
show_regs (struct user_regs *regs)
{
  printf ("ORIG_R0: %08lx\n", regs->uregs[17]);
  printf ("CPSR: %08lx\n", regs->uregs[16]);
  printf ("PC: %08lx\n", regs->uregs[15]);
  printf ("LR: %08lx\n", regs->uregs[14]);
  printf ("SP: %08lx\n", regs->uregs[13]);
  printf ("IP: %08lx\n", regs->uregs[12]);
  printf ("A1: %08lx\n", regs->uregs[0]);
  printf ("A2: %08lx\n", regs->uregs[1]);
  printf ("A3: %08lx\n", regs->uregs[2]);
  printf ("A4: %08lx\n", regs->uregs[3]);
  printf ("V1: %08lx\n", regs->uregs[4]);
  printf ("V2: %08lx\n", regs->uregs[5]);
  printf ("V3: %08lx\n", regs->uregs[6]);
  printf ("V4: %08lx\n", regs->uregs[7]);
  printf ("V5: %08lx\n", regs->uregs[8]);
  printf ("V6: %08lx\n", regs->uregs[9]);
  printf ("V7: %08lx\n", regs->uregs[10]);
  printf ("V8/IP: %08lx\n", regs->uregs[11]);
}

inline unsigned long long
do_bitflip (unsigned long long reg, int bit_pos)
{
  reg ^= 1 << bit_pos;
  return reg;
}

int
main (int argc, char **argv)
{
  long ret = -2;
  int status = 0, pid;
  struct user_regs uregs;
  int bit_pos = 0, register_type = -1;
  unsigned long sleep_time = -1;

  if (argc != 5)
    {
      printf
	("Incorrect amount of parameters\nShould be <pid> <register_type> <bit_pos> <sleep_time>\n");
      return -2;
    }

  pid = atoi (argv[1]);
  register_type = atoi (argv[2]);
  bit_pos = atoi (argv[3]);
  sleep_time = atol (argv[4]);

  struct timespec t1, t2;

  t1.tv_sec = (sleep_time / 1000);
  t1.tv_nsec = (sleep_time % 1000) * 1000000;

  // Waits until we can inject
  if (nanosleep (&t1, &t2) < 0)
    {
      printf ("Nano sleep system call failed \n");
      return -1;
    }

  // Attach to the process
  ret = ptrace (PTRACE_ATTACH, pid, 0, 0);
  if (ret == -1)
    {
      printf ("Error while attaching to %d\n", pid);
      return -1;
    }

  wait (&status);

  // Obtain current register values
  ptrace (PTRACE_GETREGS, pid, 0, &uregs);

  printf ("Old register values\n");
  show_regs (&uregs);

  // Perform the bitflip in the desired register and bit
  uregs.uregs[register_type] =
    do_bitflip (uregs.uregs[register_type], bit_pos);
  //printf("QUE ISSTO %d\n",  sizeof(uregs.uregs) / sizeof(uregs.uregs[0]));
  printf ("New register values\n");
  show_regs (&uregs);

  // Update new register values
  ptrace (PTRACE_SETREGS, pid, 0, &uregs);
  ptrace (PTRACE_DETACH, pid, 0, 0);

  return 0;
}
