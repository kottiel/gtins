#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>


//#define DIRECTORY_NAME "idocstest"
//#define ERROR_FILE     "invalid_GTINS (test).txt"
#define DIRECTORY_NAME "idocs"
#define ERROR_FILE     "invalid_GTINS"
#define SUFFIX         ".txt"
#define TESTFLAG       ""

#define GTIN             20
#define LINE_LEN       1000
#define GTIN_LBL_LEN     12   //BARCODETEXT or GS1 or GTIN
#define REC_TYPE_LEN     10   //LBL, BARCODETEXT, GS1 or GTIN
#define GTIN_LBL_START   63
#define GTIN_START       93
#define LABEL_LEN         9

/* length of GTIN-13                                                     */
#define GTIN_13          13

#define GTIN_14          14

/* divide a 14-digit GTIN by this value to isolate its first digit       */
#define GTIN_14_DIGIT 10000000000000

/* divide a 13-digit GTIN by this value to isolate its first digit       */
#define GTIN_13_DIGIT 1000000000000

/* divide a GTIN by these values to isolate company prefix               */
#define GTIN_14_CPNY_DIVISOR 1000000
#define GTIN_13_CPNY_DIVISOR 100000



/**
    Returns true (non-zero) if character-string parameter represents
    a signed or unsigned floating-point number. Otherwise returns
    false (zero).
    @param str is the numeric string value to evaluate
    @return true if str is a number, false otherwise
 */
int isNumeric(const char *str) {

    if (str == NULL || str[0] == '\0')
        return 0;

    int i = 0;
    while (str[i] != '\0')
        if (isdigit(str[i]) == 0)
            return 0;
        else
            i++;
    return 1;
}

/**
    determine the check digit of a GTIN-13 format value
    @param lp is the GTIN-13 value to calculate a check digit for
    @return a check digit
 */
int checkDigit(const long long *llp) {

    long long gtin = *llp;
    gtin = gtin / 10;
    short digit;
    int sum = 0;

    while (gtin > 0) {
        digit = (short) (gtin % 10);
        sum += 3 * digit;
        gtin /= 10;
        digit = (short) (gtin % 10);
        sum += 1 * digit;
        gtin /= 10;
    }

    return (sum % 10 == 0 ? 0 : ((((sum / 10) * 10) + 10) - sum));
}

void check_gtins(FILE *fp, char *filename, FILE *fpout) {

    char record_type[REC_TYPE_LEN];
    char line[LINE_LEN];
    char gtin[GTIN];
    char label[LABEL_LEN + 1] = {};
    char *endptr, *cp;
    int  line_num = 0;
    char gtin_first_digit;

    // skip idoc header
    fgets(line, LINE_LEN, fp);

    while (fgets(line, LINE_LEN, fp) != NULL) {

        //strncpy(record_type, line + GTIN_LBL_START, REC_TYPE_LEN);
        cp = line + GTIN_LBL_START;
        for (int i = 0; i < 9; i++) {
            record_type[i] = cp[i];
        }
        record_type[9] = '\0';

        if (strstr(record_type, "LBL") != NULL)
            strcpy(label, record_type);

        if ((strcmp(record_type, "BARCODETE") == 0) || (strcmp(record_type, "GTIN     ") == 0) || (strcmp(record_type, "GS1      ") == 0)) {
            strncpy(gtin, line + GTIN_START, GTIN);
            int i = GTIN - 1;
            while (gtin[i] == ' ') {
                gtin[i--] = '\0';
            }
/*            if (isNumeric(gtin)) {
                // convert string to long long integer to verify GTIN length and check digit
                long long gtin_dec = strtoll(gtin, &endptr, 10);
                int gtin_ctry_prefix = 0;
                int gtin_cpny_prefix = 0;

                if ((strlen(gtin) == GTIN_13) || (strlen(gtin) == GTIN_13 + 1)) {
                    if ((strlen(gtin) == GTIN_13 + 1) && (gtin_dec % 10 == checkDigit(&gtin_dec))) {
                        gtin_ctry_prefix = (int) (gtin_dec / GTIN_14_DIGIT);
                        gtin_cpny_prefix = (int) ((gtin_dec - (gtin_ctry_prefix * GTIN_14_DIGIT)) /
                                                  GTIN_14_CPNY_DIVISOR);
                    } else if (strlen(gtin) == GTIN_13) {
                        gtin_ctry_prefix = (int) (gtin_dec / GTIN_13_DIGIT);
                        gtin_cpny_prefix = (int) ((gtin_dec - (gtin_ctry_prefix * GTIN_13_DIGIT)) /
                                                  GTIN_13_CPNY_DIVISOR);
                    } else
                        fprintf(fpout, "Invalid GTIN chkdig \"%-14s\" in record %4d of file %22s (label %s).\n", gtin,
                                line_num, filename, label);

                    // verify GTIN prefixes if it's nonzero (otherwise it's just a placeholder)
                    // verify the GTIN prefixes (country: 0, 1, 2, 3, company: 4026704 or 5060112)
                    if ((gtin_ctry_prefix > 4) ||
                        (gtin != 0) &&
                        (gtin_cpny_prefix != 4026704 &&
                         gtin_cpny_prefix != 5060112))
                        fprintf(fpout, "Invalid GTIN prefix \"%-14d\" in record %4d of file %22s (label %s).\n", gtin_cpny_prefix,
                                line_num, filename, label);
                } else
                    fprintf(fpout, "Invalid GTIN length \"%-14s\" in record %4d of file %22s (label %s).\n", gtin, line_num, filename, label);
            } else if (gtin[0] != 'N') {
                fprintf(fpout, "Nonnmrc GTIN value  \"%-14s\" in record %4d of file %22s (label %s).\n", gtin, line_num,
                        filename, label);
            }*/
            gtin_first_digit = gtin[0];

            if ((gtin_first_digit != 'N') && (gtin_first_digit != 'Y') && (gtin_first_digit != 'G') && (gtin_first_digit != '(')) {
                int gtin_length = strlen(gtin);
                // convert string to long long integer to verify GTIN length and check digit
                long long gtin_dec = strtoll(gtin, &endptr, 10);

                //if ((gtin_length != GTIN_14) && (gtin_length != GTIN_13))
                    //fprintf(fpout, "Invalid GTIN length \"%-20s\" in record %4d of file %-28s (label %s).\n", gtin, line_num, filename, label);
                if (isNumeric(gtin) == 0)
                    fprintf(fpout, "Nonnmrc GTIN value  \"%-20s\" in record %4d of file %-28s (label %s).\n", gtin, line_num, filename, label);
                else if ((gtin_length == GTIN_14) && (gtin_dec % 10 != checkDigit(&gtin_dec)))
                    fprintf(fpout, "Invalid GTIN chkdig \"%-20s\" in record %4d of file %-28s (label %s).\n", gtin, line_num, filename, label);
            }
        }
        line_num++;
    }
}

int main()
{
    DIR *folder;
    FILE *fp, *fpout;
    char path[] = DIRECTORY_NAME TESTFLAG;
    char fullpath[255];
    int folderlen;
    int files;

    strcpy(fullpath, path);
    strcat(fullpath, "\\");
    folderlen = strlen(fullpath);

    struct dirent *entry;
    files = 0;

    folder = opendir(path);
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

/* Opening common file for writing */
    fpout = fopen(ERROR_FILE TESTFLAG SUFFIX, "a+");
    if ((fpout  == NULL)) {
        fprintf(stderr, "Error : Failed to open output_file\n");
        return 1;
    }

    while((entry = readdir(folder))) {
        if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
            files++;
            printf("Checking %3d: %s\n", files, entry->d_name);
            fullpath[folderlen] = '\0';
            strcat(fullpath, entry->d_name);
            fp = fopen(fullpath, "r");
            if (fp == NULL)
                printf("Could not open file %s\n", fullpath);
            else
                check_gtins(fp, entry->d_name, fpout);
            fclose(fp);
        }

    }

    fclose(fpout);
    closedir(folder);

    return(0);
}