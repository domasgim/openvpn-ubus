#ifndef UCI_HELPER_H
#define UCI_HELPER_H

#include <uci.h>

/**
 * @brief Get UCI configuration entry object. 
 * NOTE - the path string must be passed as a char[] array; char * causes seg fault. 
 * More info https://dev.archive.openwrt.org/ticket/5848.html
 * 
 * @param path UCI configuration file name
 * @return char* option
 */
char * uci_get_config_entry(char *path);

#endif