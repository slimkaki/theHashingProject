#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
    char *username;
    char *hashpwd;
} credentials;

int encryptPWD(char myString[], int size) {
    int hash = 0;
    int aux;
    for (int i = 0; i < size; i++) {
        int x = (int) myString[i];
        aux = 1;
        x = x * 85432100 * (i + 1);
        for (int j = 512; j > 1; j /= 2) {
            hash = hash + (aux*x)%j;
            aux *= -1;
        }
        hash *= 100000001;
    }
    return hash;
}

int checkIfUserExists(char *user) {
    /* TODO: Check if user exists */
    return 0;
}

int checkPassword() {
    /* TODO: Check if passed string is assigned to the user */
    return 0;
}

int main(int argc, char *argv[]) {
    /* 
    Arguments:
        -u <USERNAME>: Username to be signed in;
        -p <PASSWORD>: Password to respectively username;
        --login: Check if given password is correct to that username;
    */
    int fd;
    if ((fd = open("./data/login.txt", O_CREAT |  O_RDONLY | O_WRONLY, 0600)) == NULL) {
        fprintf(stderr, "Error trying to create login data file\n");
        exit(1);
    }
    int filho;
    int login = 0;
    char *myUSER;
    char *myPWD;
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
                printf("senha: %s\n", myPWD);
            }
            if (strstr(argv[i], "--login")) {
                login++;
            }
        }

        if (myUSER == NULL) {
            fprintf(stderr, "Need a username to continue...\n");
            exit(1);
        } else {
            // if (checkIfUserExist(myUSER)) {
            //     fprintf(stdout, "User already exists...\n");
            //     exit(1);
            // }
            credentials *newUser = malloc(sizeof(credentials));
            newUser->username = myUSER;
            char hexHash[16];
            char theNewUser[256];
            if ((filho = fork()) == 0) {
                int mySIZE = strlen(myPWD);
                int hash = encryptPWD(myPWD, mySIZE);
                printf("Hash = 0x%x\n", hash);
                sprintf(hexHash, "%x", hash);
                newUser->hashpwd = hexHash;
                sprintf(theNewUser, "{\n\tusername: %s,\n\tpassword: %s\n}", newUser->username, newUser->hashpwd);
                int nbytes = strlen(theNewUser);
                write(fd, theNewUser, nbytes);
            } else {
                wait(&filho);
                free(newUser);
            }
        }
    }
    
    close(fd);
    return 0;
}