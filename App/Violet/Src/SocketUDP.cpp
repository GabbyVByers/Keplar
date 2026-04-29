
/*
    SocketUDP.cpp
*/

#include "../Networking.h"

namespace Vi {

    /* Constructor & Destructor */

    SocketUDP::SocketUDP() {
        WinSock::inc_socket_counter();
        sock = socket(address_family, socket_type, network_protocol);
        if (sock == INVALID_SOCKET) {
            const int error = WSAGetLastError();
            Log::error("Socket Creation Failed: " + std::to_string(error));
            std::terminate();
        }
        u_long non_blocking = 1;
        if (ioctlsocket(sock, FIONBIO, &non_blocking) == SOCKET_ERROR) {
            const int error = WSAGetLastError();
            Log::error("Failed to Set Non-Blocking Mode: " + std::to_string(error));
            std::terminate();
        }
    }

    SocketUDP::~SocketUDP() {
        if (sock != INVALID_SOCKET)
            closesocket(sock);
        WinSock::dec_socket_counter();
    }

    /* Configuration */

    void SocketUDP::set_listening_port(uint16_t port) {
        if (port == 0) {
            Log::warning("Listening port cannot be set to zero");
            return;
        }
        if (listening_port != 0) {
            Log::warning("Cannot reasign listening port: " + std::to_string(listening_port));
            return;
        }
        listening_port = port;
        sockaddr_in sockaddr_in_port{};
        sockaddr_in_port.sin_family = address_family;
        sockaddr_in_port.sin_port = htons(listening_port);
        sockaddr_in_port.sin_addr.s_addr = INADDR_ANY;
        int bind_status = bind(sock, (sockaddr*)&sockaddr_in_port, sizeof(sockaddr_in_port));
        if (bind_status == SOCKET_ERROR) {
            const int error = WSAGetLastError();
            Log::error("Binding to listening port failed: " + std::to_string(error));
            std::terminate();
        }
    }

    void SocketUDP::set_destination_address(std::string ip_address, uint16_t port) {
        sockaddr_in dummy{};
        int status = inet_pton(address_family, ip_address.c_str(), &dummy.sin_addr);
        if ((status == 0) || (status == -1)) {
            Log::warning("Attempted to set invalid destination ip address");
            return;
        }
        if (port == 0) {
            Log::warning("Attempted to set invalid destination port");
            return;
        }
        destination_ip_address = ip_address;
        destination_port = port;
        destination_address.sin_family = address_family;
        destination_address.sin_port = htons(destination_port);
        inet_pton(address_family, ip_address.c_str(), &destination_address.sin_addr);
        destination_address_configured = true;
    }

    /* Getters */

    uint16_t SocketUDP::get_listening_port() const {
        return listening_port;
    }

    std::string SocketUDP::get_destination_address() const {
        return destination_ip_address;
    }

    uint16_t SocketUDP::get_destination_port() const {
        return destination_port;
    }

    /* Sending & Receiving */

    void SocketUDP::send_packet(const char* buffer, int size) const {
        if (buffer == nullptr) {
            Log::warning("Buffer is nullptr");
            return;
        }
        if (size <= 0) {
            Log::warning("Invalid buffer size");
            return;
        }
        if (!destination_address_configured) {
            Log::warning("Destination address has not been set");
            return;
        }
        if (listening_port == 0) {
            Log::warning("Listening port has not been set");
            return;
        }
        int status = sendto(sock, buffer, size, 0, (sockaddr*)&destination_address, sizeof(destination_address));
        if (status == SOCKET_ERROR) {
            const int error = WSAGetLastError();
            if (error == WSAEADDRNOTAVAIL) {
                Log::warning("Configured Address is Not Available");
                return;
            }
            Log::error("Send Failed: " + std::to_string(error));
            std::terminate();
        }
    }

    int SocketUDP::receive_packet(char* buffer, int size) const {
        if (buffer == nullptr) {
            Log::warning("Buffer is nullptr");
            return 0;
        }
        if (size <= 0) {
            Log::warning("Invalid buffer size");
            return 0;
        }
        if (listening_port == 0) {
            Log::warning("Listening port has not been set");
            return SOCKET_ERROR;
        }
        sockaddr_in return_address{};
        int size_sockaddr_in = sizeof(sockaddr_in);
        int bytes_received = recvfrom(sock, buffer, size, 0, (sockaddr*)&return_address, &size_sockaddr_in);
        if (bytes_received == SOCKET_ERROR) {
            const int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                return 0;
            }
            Log::error("Receive Failed: " + std::to_string(error));
            std::terminate();
        }
        return bytes_received;
    }
}

