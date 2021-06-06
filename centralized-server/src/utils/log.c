#include "log.h"

int writeToLog(char *message) {
    FILE *fp;

    fp = fopen("log", "a");

    time_t now;
    time(&now);

    fprintf(fp, "%s - %s\n", ctime(&now), message);
    
    fclose(fp);
    
    return 0;
}

int writeToLogInt(char *message, int number) {
    FILE *fp;

    fp = fopen("log", "a");

    time_t now;
    time(&now);

    fprintf(fp, "%s - %s %d\n", ctime(&now), message, number);
    
    fclose(fp);
    
    return 0;
}
