#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int encryptPWD(char myString[], int size) {
    /* TODO: Dncrypt a string to a hash */
    int hash = 0;
    for (int i = 0; i < size; i++) {
        int x = (int) myString[i];
        int aux = 1;
        x = x * 85432100 * (i + 1);
        for (int j = 512; j > 1; j /= 2) {
            hash = hash + (aux*x)%j;
            aux *= -1;
        }
        hash *= 10000;
    }
    return hash;
}

int checkIfPasswordExist() {
    /* TODO: Check if passed string exists */
    return 0;
}

int main(int argc, char *argv[]) {
    /* 
    Arguments:
        -p <PASSWORD>: passes a password to the code;
        --check: flag to check if it's a known password;
    */
    int filho;
    int check = 0;
    char *myUSER;
    char *myPWD;
    printf("senha: %s\n", argv[2]);
    if (argc <= 2) {
        if (argc == 1) {
            fprintf(stderr, "No argumments were passed\n");
        } else {
            if (strstr(argv[1], "-h") || strstr(argv[1], "-help")) {
            }
        }
        exit(1);
    } else {
        for (int i = 1; i < argc; i++) {
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
            int mySIZE = strlen(myPWD);
            int hash = encryptPWD(myPWD, mySIZE);
            printf("Hash = 0x%x\n", hash);

        } else {
            wait(&filho);
        }

    }
    
    return 0;
}