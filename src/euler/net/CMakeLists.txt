add_library(euler_net STATIC
        connection_signaling.cpp
        connection_signaling.h
        fake_udp_port.cpp
        fake_udp_port.h
        game_id.cpp
        game_id.h
        messages.cpp
        messages.h
        signaling_receive_context.cpp
        signaling_receive_context.h
        sockets.cpp
        sockets.h
        steam_id.cpp
        steam_id.h
        utils.cpp
        utils.h
)

target_link_libraries(euler_net PUBLIC
        euler_util
)