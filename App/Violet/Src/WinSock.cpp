
/*
    Socket.cpp
*/

#include "../Networking.h"
#include "../Logging.h"
#include <string>

namespace Vi {

    void WinSock::inc_socket_counter() {
        counter++;
        if (counter == 1) {
            const int major_version = 2;
            const int minor_version = 2;
            WSAData wsa_data;
            int status = WSAStartup(MAKEWORD(major_version, minor_version), &wsa_data);
            if (status != 0) {
                Log::error("WinSock initialization failed" + std::to_string(status));
                std::terminate();
                return;
            }
        }
    }

    void WinSock::dec_socket_counter() {
        counter--;
        if (counter == 0) {
            WSACleanup();
        }
    }
}

