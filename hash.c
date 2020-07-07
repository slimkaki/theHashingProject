#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

typedef struct {
    int index;
} myHash;

int encrypt() {
    /* TODO: Dncrypt a string to a hash */

    return 0;
}

int decrypt() {
    /* TODO: Decrypt a hash to a string */
    return 0;
}

int checkIfPasswordExist() {
    /* TODO: Check if passed string exists */
}

int main(int argc, char *argv[]) {
    /* 
    Arguments:
        -p <PASSWORD>: passes a password to the code;
        --check: flag to check if it's a known password;
    */
    int size = sizeof(argv)/sizeof(argv[0]);
    int filho;
    int check = 0;
    char *myUSER;
    char *myPWD;
    if (argc <= 2) {
        if (argc == 1) {
            fprintf(stderr, "No argumments were passed\n");
        } else {
            if (strstr(argv[1], "-h") || strstr(argv[1], "-help")) {
                printf();
            }
        }
        exit(1);
    } else {
        for (int i = 1; i < size; i++) {
            if (strstr(argv[i], "-u")) {
                i++;
                myUSER = argv[i];
            }
            if (strstr(argv[i], "-p")) {
                i++;
                myPWD = argv[i];
            }
            if (strstr(argv[i], "--check")) {
                check++;
            }
        }

        if ((filho = fork()) == 0) {
            /* TODO: CREATE HASH */

        } else {
            wait(&filho);
        }

    }
    
    return 0;
}