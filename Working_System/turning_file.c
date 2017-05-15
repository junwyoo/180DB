//
// Created by Amaael Antonini on 4/11/17.
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"

void turning_file(float ***turning, const char * file_name, int *row_counts)
{
    printf("training\n");
    char * direct = "";
    int total, i, j, k;
    char * train_file;
    train_file = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(train_file, 0, BUFF_SIZE);
    sprintf(train_file, "%s%s", direct, file_name);
    total = 0;
    for(i = 0; i< 12; i++)
        total+= row_counts[turns[i]];
    FILE *fp;
    printf("total: %d\n", total);

    fp = fopen(train_file, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to write to file \'%s\'.\n",
                train_file
        );
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d\t%d\t%d\n", total, TURN_FEATURES, 3);
    for(i = 0; i < 12; i++) {
        for (j = 0; j < row_counts[turns[i]]; j++) {
//            printf("%d\t %d\t %d\n", n, dimen[i], counts[i][j]);
            for (k = 0; k < TURN_FEATURES; k++) {
                fprintf(fp, "%f\t", (turning[i][j][k]< 1024 && -1024< turning[i][j][k]?  turning[i][j][k]/1024: 0.5));
            }
            fprintf(fp, "\n");
            for(k = 0; k < 3; k++) {
                fprintf(fp, "%d\t", (k == (int) (i / 4.0) ? 1 : -1));
            }
//            for(k = 0; k < 9; k++){
////
//                if(i < 4)
//                    fprintf(fp, "%d\t", (k == 0? 1: -1));
//                else
//                    fprintf(fp, "%d\t", (k == i-3? 1: -1));

//            }
            fprintf(fp, "\n");

        }
    }
    printf("%s, \n", train_file);
    fclose(fp);
}



