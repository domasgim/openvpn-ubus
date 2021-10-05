#include "openvpn-ubus_helper.h"
#include "openvpn-ubus.h"

int main(int argc, char **argv) {
    struct arguments arguments;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if(get_address_info(arguments.address)) {
        fprintf(stderr, "error: wrong address - %s\n", arguments.address);
        return 1;
    }

    char server_name[strlen(arguments.server_name) + 10];
    snprintf(server_name, strlen(arguments.server_name) + 9, "openvpn.%s", arguments.server_name);

    ubus_start(server_name);

	return 0;
}