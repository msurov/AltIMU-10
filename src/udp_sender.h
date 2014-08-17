#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>



class udp_sender
{
private:
    int         sock;
    sockaddr_in addr;

public:
    udp_sender(char const* addr, uint_t port);
    ~udp_sender();

    void send(uint8_t const* msg, uint_t len);
    void send(float f);

    template <typename T>
    void send(T const& a)
    {
        uint8_t const* msg = reinterpret_cast<uint8_t const*>(&a);
        send(msg, sizeof(T));
    }
};

