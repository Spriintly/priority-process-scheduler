#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    int fd[2];
    char buf[50];

    // Create pipe
    if (pipe(fd) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();

    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        // CHILD PROCESS → Reader
        close(fd[1]);   // close write end

        read(fd[0], buf, sizeof(buf));
        printf("Child received: %s\n", buf);

        close(fd[0]);
    } else {
        // PARENT PROCESS → Writer
        close(fd[0]);   // close read end

        char msg[] = "Hello from parent!";
        write(fd[1], msg, sizeof(msg));

        close(fd[1]);
        wait(0);
    }

    exit(0);
}
