#include "openvpn-ubus_helper.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s server_name \"ip_addr port\"\n", argv[0]);
        return 1;
    }

    if(get_address_info(argv[2])) {
        fprintf(stderr, "error: wrong address - %s\n", argv[2]);
        return 1;
    }

    char server_name[strlen(argv[1]) + 10];
    snprintf(server_name, strlen(argv[1]) + 9, "openvpn.%s", argv[1]);

    ubus_start(server_name);

	return 0;
}