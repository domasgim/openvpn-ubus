#include "openvpn-ubus_helper.h"

enum {
	CLIENT_VALUE,
	__CLIENT_MAX
};

static const struct blobmsg_policy client_policy[] = {
	[CLIENT_VALUE] = { .name = "common_name", .type = BLOBMSG_TYPE_STRING },
};

static const struct ubus_method management_methods[] = {
	UBUS_METHOD_NOARG("status", management_status),
	UBUS_METHOD("kill", management_kill, client_policy)
};

static struct ubus_object_type management_object_type =
	UBUS_OBJECT_TYPE("management", management_methods);

int get_client_info(char *token_pointer, char **common_name, char **real_address, char **bytes_received, char **client_bytes_sent, char **connected_since) {
    char *token;
    int rc = 0;
    if((token = strtok_r(token_pointer, ",", &token_pointer)) == NULL) { rc = 1; }
    *common_name = token;
    
    if((token = strtok_r(token_pointer, ",", &token_pointer)) == NULL) { rc = 1; }
    *real_address = token;

    if((token = strtok_r(token_pointer, ",", &token_pointer)) == NULL) { rc = 1; }
    *bytes_received = token;

    if((token = strtok_r(token_pointer, ",", &token_pointer)) == NULL) { rc = 1; }
    *client_bytes_sent = token;

    if((token = strtok_r(token_pointer, ",", &token_pointer)) == NULL) { rc = 1; }
    *connected_since = token;

    return rc;
}

int get_address_info(char *token_pointer) {
    char *token;
    int rc = 0;
    if((token = strtok_r(token_pointer, " ", &token_pointer)) == NULL) { rc = 1; }
    IP_ADDR = token;

    if((token = strtok_r(token_pointer, " ", &token_pointer)) == NULL) { rc = 1; }
    PORT = token;

    return rc;
}

static int management_status(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg) 
{
    char *reply;
    int bytes_received;
    int rc;
    rc = tcp_send_msg("status", &reply, &bytes_received, IP_ADDR, PORT);

    printf("sending status request to %s:%s\n", IP_ADDR, PORT);
    if (rc) {
        fprintf(stderr, "tcp_send_msg error\n");
        return 1;
    }

    char input[strlen(reply)];
    strcpy(input, reply);

    void *array;
    void *table;

    struct blob_buf b = {};
        
    blob_buf_init(&b, 0);

    char *common_name;
    char *real_address;
    char *client_bytes_received;
    char *client_bytes_sent;
    char *connected_since;

    char *token, *token_pointer;
    token_pointer = input;

    /* Starting to tokenise reply message. The first 4 lines
     of the reply will be general informationso we parse the token 4 times */
    if((token = strtok_r(token_pointer, "\r\n", &token_pointer)) == NULL) { goto end; }
    if ((strstr(token, "CLIENT LIST")) == NULL) { 
        blobmsg_add_string(&b, "clients", "no clients");
        fprintf(stderr, "Command 'status' does not print clients with this configuration\n");
        goto end; 
    }
    if((token = strtok_r(token_pointer, "\r\n", &token_pointer)) == NULL) { goto end; }
    if((token = strtok_r(token_pointer, "\r\n", &token_pointer)) == NULL) { goto end; }
    if((token = strtok_r(token_pointer, "\r\n", &token_pointer)) == NULL) { goto end; }

    if((strstr(token, "ROUTING TABLE") == NULL)) { 
        array = blobmsg_open_array(&b, "clients");
    } else {
        blobmsg_add_string(&b, "clients", "no clients");
        goto end;
    }

    while((strstr(token, "ROUTING TABLE") == NULL)) {
        char *common_name;
        char *real_address;
        char *client_bytes_received;
        char *client_bytes_sent;
        char *connected_since;

        char *token_copy = strdup(token);

        if(get_client_info(token_copy, &common_name, &real_address, &client_bytes_received, &client_bytes_sent, &connected_since) == 0) {
            table = blobmsg_open_table(&b, "client");
            blobmsg_add_string(&b, "common_name", common_name);
            blobmsg_add_string(&b, "real_address", real_address);
            blobmsg_add_string(&b, "bytes_received", client_bytes_received);
            blobmsg_add_string(&b, "bytes_sent", client_bytes_sent);
            blobmsg_add_string(&b, "connected_since", connected_since);
            blobmsg_close_table(&b, table);
        }
        token = strtok_r(token_pointer, "\r\n", &token_pointer);
    }

    blobmsg_close_array(&b, array);
    
    end:
        ubus_send_reply(ctx, req, b.head);
        blob_buf_free(&b);

	return 0;
}

static int management_kill(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg) {

	struct blob_attr *tb[__CLIENT_MAX];
	struct blob_buf b = {};

    blobmsg_parse(client_policy, __CLIENT_MAX, tb, blob_data(msg), blob_len(msg));

    if (!tb[CLIENT_VALUE])
		return UBUS_STATUS_INVALID_ARGUMENT;

    char *reply;
    int bytes_received;
    int rc;
    char *client_killed = blobmsg_get_string(tb[CLIENT_VALUE]);

    char tcp_msg[strlen(client_killed) + 7];
    snprintf(tcp_msg, strlen(client_killed) + 6, "kill %s", client_killed);

    rc = tcp_send_msg(tcp_msg, &reply, &bytes_received, IP_ADDR, PORT);
    if (rc) {
        fprintf(stderr, "TCP error\n");
        return 1;;
    }

    if((strstr(reply, "SUCCESS")) == NULL) {
        fprintf(stderr, "TCP connected but management returned an error\n");
        return 1;
    }

    blob_buf_init(&b, 0);

    blobmsg_add_string(&b, "killed", client_killed);

    end: 
        ubus_send_reply(ctx, req, b.head);
        blob_buf_free(&b);
        return 0;

}

int ubus_start(char *server_name) {

    struct ubus_object management_object = {
	.name = server_name,
	.type = &management_object_type,
	.methods = management_methods,
	.n_methods = ARRAY_SIZE(management_methods),
    };

    struct ubus_context *ctx;

	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &management_object);
	uloop_run();

	ubus_free(ctx);
	uloop_done();

    return 0;
}