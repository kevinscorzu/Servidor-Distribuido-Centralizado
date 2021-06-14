#include "log.h"

/**
 * Function that writes a message to the log
 * Receives the message to be written
 */
int writeToLog(char *message) {
    FILE *fp;

    fp = fopen("log", "a");

    time_t now;
    time(&now);

    fprintf(fp, "%s - %s\n", ctime(&now), message);
    
    fclose(fp);
    
    return 0;
}

/**
 * Function that writes a number to the log
 * Receives the message and number to be written
 */
int writeToLogInt(char *message, int number) {
    FILE *fp;

    fp = fopen("log", "a");

    time_t now;
    time(&now);

    fprintf(fp, "%s - %s %d\n", ctime(&now), message, number);
    
    fclose(fp);
    
    return 0;
}
