//
// Created by Amaael Antonini on 4/26/17.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main3()
{

    int result = mkdir("/Users/amaaelantonini/Google Drive/Local/Spring_2017/EE 180DB/Real_Time_trial/home/me/", 0777);
    printf("%d\n", result);
    return 0;
}

