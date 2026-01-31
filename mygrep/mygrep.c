#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *OUTFILE = NULL;
char *KEYWORD = NULL;
int I_FLAG = 0;

void printUsage() {
    printf("mygrep [-i] [-o outfile] keyword [file...]\n");
}

int writeToFile(char *keyword, FILE *outfile_fp) {
    if (outfile_fp == NULL) {
        fprintf(stdout, "%s", keyword);
    }else {
        fprintf(outfile_fp, "%s", keyword);
    }

    return EXIT_SUCCESS;
}

int parseArgs(int argc, char **argv) {

    if (argc < 2) {
        printUsage();
        goto fail;
    }
    int opt = 0;

    while ((opt = getopt(argc, argv, "io:")) != -1) {
        switch (opt) {
            case 'i':
                I_FLAG = 1;
                break;
            case 'o':
                OUTFILE = fopen(optarg, "w");
                if (OUTFILE == NULL) {
                    perror(optarg);
                    goto fail;
                }
                break;
            default:
                printUsage();
                goto fail;
        }
    }
    if (argc - optind < 1) {
        printUsage();
        goto fail;
    }

    KEYWORD = strdup(argv[optind++]);
    if (!KEYWORD) {
        perror("strdup");
        goto fail;
    }

    return EXIT_SUCCESS;

    fail:
       if (OUTFILE) fclose(OUTFILE);
        OUTFILE = NULL;
        return EXIT_FAILURE;
}

int strLookup(char* keyword, FILE *fp) {
    char *lineptr = NULL;
    size_t len = 0;

    while (getline(&lineptr, &len, fp) != -1) {
        if (I_FLAG) {
            if (strcasestr(lineptr, keyword))
                writeToFile(lineptr, OUTFILE);
        } else {
            if (strstr(lineptr, keyword))
                writeToFile(lineptr, OUTFILE);
        }
    }

    free(lineptr);
    return EXIT_SUCCESS;
}

int cleanUp(void) {
    if (OUTFILE != NULL) fclose(OUTFILE);
    if (KEYWORD != NULL) free(KEYWORD);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (parseArgs(argc, argv) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if (optind >= argc) {
        strLookup(KEYWORD, stdin);
    } else {
        for (int i = optind; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if (!fp) {
                perror(argv[i]);
                return EXIT_FAILURE;
            }
            strLookup(KEYWORD, fp);
            fclose(fp);
        }
    }

    cleanUp();
    return EXIT_SUCCESS;
}

