#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

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

int isTheFileEmpty(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        return 1;
    }
    return 0;
}

int checkIfUserExists(FILE *fp, char *user) {
    char line[256];
    char needle[256];
    sprintf(needle, "username: %s", user);
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, needle) != NULL) {
            return 1;
        }
    }
    return 0;
}

int checkPassword(FILE *fp, credentials *user) {
    int c = 0;
    char line[256];
    char needle[256];
    sprintf(needle, "username: %s", user->username);
    while (fgets(line, sizeof(line), fp)) {
        if (c) {
            sprintf(needle, "password: %s", user->hashpwd);
            if (strstr(line, needle) != NULL) {
                return 1;
            }
        }
        if (strstr(line, needle) != NULL) {
            c++;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    /* 
    Arguments:
        -u <USERNAME>: Username to be signed in;
        -p <PASSWORD>: Password to respectively username;
        --login: Check if given password is correct to that username;
    */
    FILE *fp;

    if ((fp = fopen("./data/login.txt", "r+")) == NULL) {
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
            }
            if (strstr(argv[i], "--login")) {
                login++;
            }
        }

        if (myUSER == NULL) {
            fprintf(stderr, "Need a username to continue...\n");
            exit(1);
        } else {
            credentials *newUser = malloc(sizeof(credentials));
            newUser->username = myUSER;
            if (login) {
                int checkSIZE = strlen(myPWD);
                int checkHash = encryptPWD(myPWD, checkSIZE);
                char hexHash[16];
                sprintf(hexHash, "%x", checkHash);
                newUser->hashpwd = hexHash;
                if (checkPassword(fp, newUser)) {
                    fprintf(stdout, "Access granted!\n");
                    exit(0);
                }
            }
            if (checkIfUserExists(fp, newUser->username)) {
                fprintf(stdout, "User already exists...\n");
                exit(1);
            }
            char hexHash[16];
            char theNewUser[256];
            if ((filho = fork()) == 0) {
                int mySIZE = strlen(myPWD);
                int hash = encryptPWD(myPWD, mySIZE);
                printf("Hash = 0x%x\n", hash);
                sprintf(hexHash, "%x", hash);
                newUser->hashpwd = hexHash;
                if (isTheFileEmpty(fp)) {
                    sprintf(theNewUser, "{\n\tusername: %s,\n\tpassword: %s\n}", newUser->username, newUser->hashpwd);
                } else {
                    sprintf(theNewUser, ",\n{\n\tusername: %s,\n\tpassword: %s\n}", newUser->username, newUser->hashpwd);
                }
                int nbytes = strlen(theNewUser);
                fwrite(theNewUser, nbytes, 1, fp);
            } else {
                wait(&filho);
                free(newUser);
            }
        }
    }
    fclose(fp);
    return 0;
}