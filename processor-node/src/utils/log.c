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

int writeToLogInt(char *message, char *auxMessage, int number, int portNumber) {
    FILE *fp;

    fp = fopen("log", "a");

    time_t now;
    time(&now);

    fprintf(fp, "%s - %s %d %s %d\n", ctime(&now), message, number,auxMessage,portNumber);
    
    fclose(fp);
    
    return 0;
}
