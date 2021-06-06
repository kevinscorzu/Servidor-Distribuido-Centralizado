#include "config.h"

int readConfigFile() {
    config_t cfg;
    config_setting_t *setting;

    config_init(&cfg);

    if(!config_read_file(&cfg, "config.conf")) {
        writeToLog("Status: Error while opening config file");

        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if(config_lookup_int(&cfg, "port", &port)) {
        writeToLog("Status: Successfully read config file");
        config_destroy(&cfg);
        return 0;
    }
    else {
        writeToLog("Status: Error while reading config file");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }
}