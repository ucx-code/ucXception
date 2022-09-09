#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{

  int pid = atoi(argv[1]);
  int sleep_time = atol(argv[4]);
  int ret;

  struct timespec t1, t2;

  t1.tv_sec = (sleep_time / 1000);
  t1.tv_nsec = (sleep_time % 1000) * 1000000;

  // Waits until we can inject
  if (nanosleep(&t1, &t2) < 0) {
      fprintf(stderr, "Nano sleep system call failed \n");
      return -1;
  }

  printf("Killing process with PID: %d\n", pid);
  //SIGTERM
  ret = kill(pid, SIGKILL);

  printf("Return code: %d\n", ret );

  return 0;
}
