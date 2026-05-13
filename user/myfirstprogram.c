#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Usage: sleep number\n");
        exit(0);
    }

    int time = atoi(argv[1]);

    pause(time);

    exit(0);
}
