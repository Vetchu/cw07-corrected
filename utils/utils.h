//
// Created by vetch on 16.03.19.
//

#ifndef ZESTAW2_UTILS_H
#define ZESTAW2_UTILS_H

#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

#define ARGUMENT_ERROR -2
#define TYPE_MISMATCH -3

/*
#define OTHER_ERROR -1
static clock_t st_time;
static clock_t en_time;
static struct tms st_cpu;
static struct tms en_cpu;

void start_clock();

char *stop_clock();
void write_to_report_file(char *filepath, char *operation, char *stopclock);
*/
unsigned long _convert_to_unsigned_long(char *string);
long getCurrentTime();

#endif //ZESTAW2_UTILS_H
