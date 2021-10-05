#include <stdlib.h>
#include <argp.h>

/* Program documentation. */
static char doc[] =
  "openvpn-ubus -- a program which creates an openvpn management ubus process";

/* The options we understand. */
static struct argp_option options[] = {
  {"server-name",  's', "NAME",      0,  "Server name" },
  {"address",    'a', "ADDRESS",      0,  "Address <IP_ADDR PORT>" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *server_name;
  char *address;
  int server_name_flag;
  int address_flag;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 's':
            arguments->server_name = arg;
            arguments->server_name_flag = 1;
            break;
        case 'a':
            arguments->address = arg;
            arguments->address_flag = 1;
            break;

        case ARGP_KEY_END:
            if (arguments->address_flag != 1 || arguments->server_name_flag != 1) {
                argp_usage (state);
            }
        break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

/* argp parser. */
static struct argp argp = { options, parse_opt, 0, doc };