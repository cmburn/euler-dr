add_library(euler_net STATIC
        identity.cpp
        identity.h
        ip_address.cpp
        ip_address.h
        listen_socket.cpp
        listen_socket.h
        messages.cpp
        messages.h
        message.cpp
        message.h
        net_connection.cpp
        net_connection.h
        sockets.cpp
        sockets.h
        utils.cpp
        utils.h
)

target_link_libraries(euler_net PUBLIC
        euler_util
        GameNetworkingSockets::static
)