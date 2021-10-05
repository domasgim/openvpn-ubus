#include "tcp_helper.h"

void send_msg(int socket_peer, char *read) {
    int bytes_sent = send(socket_peer, read, strlen(read), 0);
    return;
}

int recv_msg_string(int socket_peer, char **reply,  int *bytes_received) {
    char read[4096];
    *bytes_received = recv(socket_peer, read, 4096, 0);
    if (*bytes_received < 1) {
        fprintf(stderr, "Connection closed by peer.\n");
        return 1;
    }
    *reply = read;
    return 0;
}

int recv_msg_empty(int socket_peer) {
    char read[4096];
    int bytes_received = recv(socket_peer, read, 4096, 0);
    if (bytes_received < 1) {
        fprintf(stderr, "Connection closed by peer.\n");
        return 1;
    }
    return 0;
}

int configure_remote_addr(struct addrinfo **peer_address, char *ip_addr, char *port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip_addr, port, &hints, peer_address)) {
        fprintf(stderr, "getaddrinfo() failed to %s:%s. (%d)\n", ip_addr, port, errno);
        return 1;
    }

    return 0;
}

void print_remote_addr(struct addrinfo *peer_address) {
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer),
            NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);
}

int create_socket(struct addrinfo *peer_address) {
    int socket_peer;
    socket_peer = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(socket_peer, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d)\n", errno);
        return 1;
    }

    return socket_peer;
}

int connect_socket(struct addrinfo *peer_address, int socket_peer) {
    if (connect(socket_peer,
                peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr, "connect() failed. (%d)\n", errno);
        return 1;
    }
    return 0;
}

int tcp_send_msg(char *msg, char **reply, int *bytes_received, char *ip_addr, char *port) {
    char *reply_tmp;
    int bytes_received_tmp;
    struct addrinfo *peer_address;

    if(configure_remote_addr(&peer_address, ip_addr, port)) {
        return 1;
    }

    int socket_peer = create_socket(peer_address);
    if(socket_peer == 1) {
        return 1;
    }

    if(connect_socket(peer_address, socket_peer)) {
        return 1;
    }
    freeaddrinfo(peer_address);


    /* Sending empty message to receive and discard the opening message of management server */
    send_msg(socket_peer, "\n");
    recv_msg_empty(socket_peer);

    /* Actual message */
    send_msg(socket_peer, msg);
    send_msg(socket_peer, "\n");
    recv_msg_string(socket_peer, &reply_tmp, &bytes_received_tmp);

    *reply = reply_tmp;
    *bytes_received = bytes_received_tmp;

    close(socket_peer);

    return 0;
}