#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void work() {
    for(volatile int i = 0; i < 200000000; i++);
}

int main() {
    int go[3][2];    
    int ready[3][2]; 

    for(int i = 0; i < 3; i++) {
        pipe(go[i]);
        pipe(ready[i]);
    }

    // Low priority child
    if(fork() == 0) {
        set_priority(1);
        char buf[1];
        write(ready[0][1], "r", 1);
        // wait for go
        read(go[0][0], buf, 1);
        work();
        printf("Low priority process finished (priority 1)\n");
        exit(0);
    }

    // Medium priority child
    if(fork() == 0) {
        set_priority(5);
        char buf[1];
        write(ready[1][1], "r", 1);
        read(go[1][0], buf, 1);
        work();
        printf("Medium priority process finished (priority 5)\n");
        exit(0);
    }

    // High priority child
    if(fork() == 0) {
        set_priority(10);
        char buf[1];
        write(ready[2][1], "r", 1);
        read(go[2][0], buf, 1);
        work();
        printf("High priority process finished (priority 10)\n");
        exit(0);
    }

    
    char buf[1];
    for(int i = 0; i < 3; i++)
        read(ready[i][0], buf, 1);

    
    for(int i = 0; i < 3; i++)
        write(go[i][1], "g", 1);

    wait(0); wait(0); wait(0);
    exit(0);
}