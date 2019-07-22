#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define GTIN 108
#define GTIN_LBL_LEN 4
#define GTIN_LBL_START 64

void check_gtins(FILE *fp, FILE *fpout) {

    char line[GTIN];
    char gtinlabel[5];
    char *cp;

    while (fgets(line, GTIN, fp) != NULL) {
        cp = line + GTIN_LBL_START;
        strncpy(gtinlabel, cp, GTIN_LBL_LEN);
        gtinlabel[4] = '\0';

        if ((strcmp(gtinlabel, "GTIN") == 0) ||
            (strcmp(gtinlabel, "GS1") == 0))
            printf("found %s\n", gtinlabel);
    }
}


int main()
{
    DIR *folder;
    FILE *fp, *fpout;

    struct dirent *entry;
    int files = 0;

    folder = opendir("idocstest");
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

/* Opening common file for writing */
    fpout = fopen("invalid_GTINS.txt", "a+");
    if ((fpout  == NULL)) {
        fprintf(stderr, "Error : Failed to open output_file\n");
        return 1;
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        printf("Checking %3d: %s\n", files, entry->d_name);
        fp = fopen("idocstest" entry->d_name, "r");
        if ( fp == NULL )
            printf( "Could not open file %s\n", entry->d_name);
        else
            check_gtins(fp, fpout);
    }

    closedir(folder);

    return(0);
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char **argv) {
    DIR *FD;
    struct dirent *in_file;
    FILE *output_file;
    FILE *entry_file;
    char buffer[BUFSIZ];

    */
/* Opening common file for writing *//*

    output_file = fopen("invalid_GTINS.txt", "a+");
    if (output_file == NULL) {
        fprintf(stderr, "Error : Failed to open output_file\n");

        return 1;
    }

    */
/* Scanning the in directory *//*

    if (NULL == (FD = opendir("idocstest"))) {
        fprintf(stderr, "Error : Failed to open input directory\n");
        fclose(output_file);

        return 1;
    }
    while ((in_file = readdir(FD))) {
        */
/* On linux/Unix we don't want current and parent directories
         * If you're on Windows machine remove this two lines
         *//*

*/
/*            if (!strcmp (in_file->d_name, "."))
                continue;
            if (!strcmp (in_file->d_name, ".."))
                continue;*//*

        */
/* Open directory entry file for common operation *//*



        */
/* TODO : change permissions to meet your need! *//*

        printf("%s\n", in_file->d_name);
        entry_file = fopen(in_file->d_name, "r");
        if (entry_file == NULL) {
            fprintf(stderr, "Error : Failed to open entry file\n");
            fclose(output_file);

            return 1;
        }

        */
/* Doing some stuff with entry_file : *//*


        while (fgets(buffer, BUFSIZ, entry_file) != NULL) {
            printf("reading line %s\n", buffer );
        }

        //fprintf(output_file, "reading file %s", in_file->d_name);
        printf("reading file %s\n", in_file->d_name);

        */
/* When you finish with the file, close it *//*

        fclose(entry_file);
    }

    */
/* Don't forget to close common file before leaving *//*

    fclose(output_file);
    closedir(FD);

    return 0;
}*/
