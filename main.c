#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>

#include "errors.h"

int main(int argc, char *argv[])
{
    const char *monitored_program = NULL, *failsafe_program = NULL;
    char * const *monitored_args = { NULL }, * const *failsafe_args = { NULL };
    pid_t pid;

    // 1. Validate the monitored and failsafe programs are passed
    if (argc < 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
	fputs("Usage: ./deadman PROGRAM_X PROGRAM_Y [options]\n", stderr);
	exit(EUSAGE);
    }

    // 2. Validate that the monitored and failsafe programs exist and are executable by this process
    monitored_program = argv[1];
    failsafe_program = argv[2];

    if (access(monitored_program, X_OK) == -1)
    {
	perror("Monitored program (argv[1])");
	exit(EEXECUTABLE);
    }

    if (access(failsafe_program, X_OK) == -1)
    {
	perror("Failsafe program (argv[2])");
	exit(EEXECUTABLE);
    }

    // 3. Spawn the monitored program as a child process
    pid = fork();

    if (pid == 0) /* child process */
    {
	execve(monitored_program, monitored_args, NULL); /* program, args[], env[] */	
    }
    else if (pid < 0) /* failed to fork */
    {
	perror("failed to fork");
	exit(EFORK);
    }
    else /* parent process */
    {
	int status = 0;
	wait(&status);

	if (status == -1)
	{
	    perror("waiting on monitored process");
	    exit(EWAIT);
	}

	if (WEXITSTATUS(status) != 0 || WIFSIGNALED(status) || WCOREDUMP(status) || WIFSTOPPED(status))
	{
	    execve(failsafe_program, failsafe_args, NULL);
	}
    }

    return EXIT_SUCCESS;
}
