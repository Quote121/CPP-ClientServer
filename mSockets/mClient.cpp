#include "mClient.h"

mClient::mClient() = default;

mClient::mClient(struct sockaddr_storage _client_address, socklen_t _client_len, IPFamily _ip_family, SOCKET _socket_client, int _connectionTime) :
        client_address(_client_address),
        client_len(_client_len),
        ip_family(_ip_family),
        socket_client(_socket_client),
        connectionTime(_connectionTime){}
    
struct sockaddr_storage mClient::getClient_address()
{
    return client_address;
}

void mClient::setClient_address(struct sockaddr_storage &_client_address)
{
    client_address = _client_address;
}

socklen_t mClient::getClientLen()
{
    return client_len;
}

void mClient::setClientLen(socklen_t _client_len)
{
    client_len = _client_len;
}

IPFamily mClient::getIpFamily()
{
    return ip_family;
}

void mClient::setIpFamily(IPFamily _ip_family)
{
    ip_family = _ip_family;
}

SOCKET mClient::getSocketClient()
{
    return socket_client;
}

void mClient::setSocketClient(SOCKET _socket_client)
{
    socket_client = _socket_client;
}

int mClient::getConnectionTime()
{
    return connectionTime;
}

void mClient::setConnectionTime(int _connectionTime)
{
    connectionTime = _connectionTime;
}
