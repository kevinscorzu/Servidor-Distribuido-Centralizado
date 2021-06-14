#include "config.h"

/**
 * Function that reads the config file
 */
int readConfigFile() {
    config_t cfg;
    config_setting_t *setting;

    const char *serverIpTemp;

    config_init(&cfg);

    if(!config_read_file(&cfg, "config.conf")) {
        printf("Error while opening config file\n");

        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if(config_lookup_string(&cfg, "serverIp", &serverIpTemp)) {
        serverIp = malloc(sizeof(serverIpTemp));
        strcpy(serverIp, serverIpTemp);
        
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