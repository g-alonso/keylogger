/*
 * tasmania.c
 * 
 * create a deamon
 *
 * 2013 Gabriel Alonso <gbr.alonso@gmail.com>
 * 
 */

#include <stdlib.h>
#include <unistd.h>
#include "tasmania.h"

int tasmanizame(){

  pid_t pid, sid;

  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    perror("Can't create new sid");
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    perror("Can't change to the root directory");
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}