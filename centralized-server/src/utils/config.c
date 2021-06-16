#include "config.h"

/**
 * Function that reads the config file
 */
int readConfigFile() {
    config_t cfg;
    config_setting_t *setting;

    const char *node1IpTemp;
    const char *node2IpTemp;    

    config_init(&cfg);

    if(!config_read_file(&cfg, "config.conf")) {
        writeToLog("Status: Error while opening config file");

        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if(!config_lookup_int(&cfg, "port", &port)) {
        writeToLog("Status: Error while reading config file");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if(!config_lookup_string(&cfg, "node1Ip", &node1IpTemp)) {
        writeToLog("Status: Error while reading config file");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if(!config_lookup_string(&cfg, "node2Ip", &node2IpTemp)) {
        writeToLog("Status: Error while reading config file");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    node1Ip = malloc(strlen(node1IpTemp));
    strcpy(node1Ip, node1IpTemp);

    node2Ip = malloc(strlen(node2IpTemp));
    strcpy(node2Ip, node2IpTemp);   

    writeToLog("Status: Successfully read config file");
    config_destroy(&cfg);
    return 0;
}