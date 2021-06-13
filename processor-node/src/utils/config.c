#include "config.h"

int readConfigFile() {
    config_t cfg;
    config_setting_t *setting;

    config_init(&cfg);

    if(!config_read_file(&cfg, "config.conf")) {
        printf("Error while opening config file\n");

        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if(config_lookup_int(&cfg, "port", &port) && config_lookup_int(&cfg, "id", &id)) {
        printf("Successfully read config file\n");
        config_destroy(&cfg);
        return 0;
    }
    else {
        printf("Error while reading config file\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }
}