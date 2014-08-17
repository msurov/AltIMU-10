#include <stdio.h>
#include <errno.h>
#include <exception>
#include <memory.h>
#include <stdexcept>
#include <sys/types.h>

#include "udp_sender.h"


udp_sender::udp_sender(char const* ip, uint_t port)
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (!sock)
        throw std::runtime_error("can't create socket");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    uint_t h = inet_addr(ip);
    if (h == INADDR_NONE)
        throw std::runtime_error("unknown ip address");

    addr.sin_addr.s_addr = h;
}

udp_sender::~udp_sender()
{
    close(sock);
}

void udp_sender::send(uint8_t const* msg, uint_t len)
{
    int err = sendto(sock, msg, len, 0, (sockaddr*)&addr, sizeof(addr));
    if (err < 0)
        throw std::runtime_error("sendto error");
}

