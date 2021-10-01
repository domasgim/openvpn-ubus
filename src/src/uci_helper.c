#include "uci_helper.h"

char * uci_get_config_entry (char *path) {
    struct uci_context *c;
    struct uci_ptr ptr;
    char *result;

    c = uci_alloc_context ();
    if (uci_lookup_ptr (c, &ptr, path, true) != UCI_OK) {
        uci_perror (c, "get_config_entry Error");
        return 1;
    }

    switch(ptr.o->type) {
        case UCI_TYPE_STRING:
            result = strdup(ptr.o->v.string);
            break;
        default:
            break;
        }

    uci_free_context (c);
    return result;
}