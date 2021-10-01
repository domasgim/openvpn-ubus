#include <libubox/blobmsg_json.h>
#include <libubus.h>

#include "tcp_helper.h"
#include "uci_helper.h"

#include <uci.h>
#include <string.h>
#include <arpa/inet.h>

/**
 * @brief Parse management server reply containing client info
 * 
 * @param token_pointer A single tokenised line ("\r\n")
 * @param common_name Refrence to client's common name string
 * @param real_address Refrence to client's real address
 * @param bytes_received Refrence to client's received bytes during the session
 * @param client_bytes_sent Refrence to client's sent bytes during the session
 * @param connected_since Refrence to client's time connected to the session
 * @return int return code
 */
int get_client_info(char *token_pointer, char **common_name, char **real_address, char **bytes_received, char **client_bytes_sent, char **connected_since);

/**
 * @brief Parse address sent through openvpn procd
 * 
 * @param token_pointer Line to be parsed
 * @return int return code
 */
int get_address_info(char *token_pointer);

/**
 * @brief Request for management server status about connected clients 
 * and print it to ubus
 * 
 * @param ctx 
 * @param obj 
 * @param req 
 * @param method 
 * @param msg 
 * @return int 
 */
static int management_status(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

/**
 * @brief Request management server to disconnect a client
 * 
 * @param ctx 
 * @param obj 
 * @param req 
 * @param method 
 * @param msg 
 * @return int 
 */
static int management_kill(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

/**
 * @brief Start a ubus process
 * 
 * @param server_name Server name
 */
int ubus_start(char *server_name);

