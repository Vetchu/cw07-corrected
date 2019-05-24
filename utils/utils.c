//
// Created by vetch on 16.03.19.
//


#include <time.h>
#include "utils.h"

/*
void start_clock() {
    st_time = times(&st_cpu);
}

char *stop_clock() {
    en_time = times(&en_cpu);
    int64_t clk_tck = sysconf(_SC_CLK_TCK);
    char *return_value = calloc(100, sizeof(char));
    sprintf(return_value, "%-12.2f %-12.2f %-12.2f %-12.2f  %-12.2f\n",
            (double) (en_time - st_time) / clk_tck,
            (double) (en_cpu.tms_utime - st_cpu.tms_utime) / clk_tck,
            (double) (en_cpu.tms_stime - st_cpu.tms_stime) / clk_tck,
            (double) (en_cpu.tms_cutime - st_cpu.tms_cutime) / clk_tck,
            (double) (en_cpu.tms_cstime - st_cpu.tms_cstime) / clk_tck);
    return return_value;
}
void write_to_report_file(char *filepath, char *operation, char *stopclock) {
    FILE *ofile = fopen(filepath, "a");

    if (strcmp(operation, "none") != 0) {
        fprintf(ofile, "%s\t", operation);
    }
    fprintf(ofile, "%s", stopclock);
    printf("%s %s", operation, stopclock);
    fclose(ofile);
}

long _convert_to_long(char *string) {
    char *temp_string;
    long converted = strtol(string, &temp_string, 10);
    long return_value = strcmp(temp_string, "") == 0 ? converted : TYPE_MISMATCH;

    return return_value;
}
*/

long getCurrentTime(){
    struct timespec Time;
    clock_gettime(CLOCK_REALTIME, &(Time));
    return Time.tv_nsec+1e9*Time.tv_sec;
}

unsigned long _convert_to_unsigned_long(char *string) {
    char *temp_string;
    long converted = strtol(string, &temp_string, 10);
    unsigned long return_value = (strcmp(temp_string, "") == 0 || strcmp(temp_string, "\n")==0) && converted>0 ?(unsigned long) converted : TYPE_MISMATCH;

    return return_value;
}