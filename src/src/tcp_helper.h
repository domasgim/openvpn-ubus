#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define PRINT_SOCKET_MESSAGE_EXT(n, s) (printf("Received (%d bytes): %.*s\n\n", n, n, s))
#define PRINT_SOCKET_MESSAGE(n, s) (printf("%.*s\n\n", n, s))

#include <stdio.h>
#include <string.h>
#include "uci_helper.h"

/**
 * @brief Send a message through a socket
 * 
 * @param socket_peer Socket
 * @param read Message contents
 */
void send_msg(int socket_peer, char *read);

/**
 * @brief Receive a message through a socket with refrences to reply strings to be stored
 * 
 * @param socket_peer Socket
 * @param reply Refrence to reply string
 * @param bytes_received Refrence to received bytes count
 * @return int 
 */
int recv_msg_string(int socket_peer, char ***reply,  int **bytes_received);

/**
 * @brief Receive a message through a socket, not printing any reply
 * 
 * @param socket_peer Socket
 * @return int 
 */
int recv_msg_empty(int socket_peer);

/**
 * @brief Configure a remote address
 * 
 * @param peer_address Refrence to the peer address
 * @param ip_addr IP address
 * @param port port
 * @return int 
 */
int configure_remote_addr(struct addrinfo **peer_address, char *ip_addr, char *port);

/**
 * @brief Print remote address information to console
 * 
 * @param peer_address peer address
 * @return int 
 */
int print_remote_addr(struct addrinfo *peer_address);

/**
 * @brief Create a socket object
 * 
 * @param peer_address peer address
 * @return int 
 */
int create_socket(struct addrinfo *peer_address);

/**
 * @brief Connect to a socket
 * 
 * @param peer_address peer address
 * @param socket_peer socket peer
 * @return int 
 */
int connect_socket(struct addrinfo *peer_address, int socket_peer);

/**
 * @brief Send a message through a socket and receive a reply 
 * (specified for ovpn management server)
 * 
 * @param msg message to be sent
 * @param reply refrence to reply string
 * @param bytes_received refrence to the count of received bytes
 * @param ip_addr management server IP address
 * @param port management server port
 * @return int 
 */
int tcp_send_msg(char *msg, char **reply, int *bytes_received, char *ip_addr, char *port);