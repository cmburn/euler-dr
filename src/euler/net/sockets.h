/* SPDX-License-Identifier: ISC */

#ifndef EULER_NET_SOCKETS_H
#define EULER_NET_SOCKETS_H

#include <steam/isteamnetworkingsockets.h>

#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::net {
class ListenSocket;
class NetConnection;
class Identity;
class Message;

class Sockets final : public util::Object {
public:
	Sockets();
	~Sockets() override;

	util::Reference<ListenSocket> create_listen_socket_ip();
	util::Reference<NetConnection> connect_by_ip_address();
	util::Reference<ListenSocket> create_listen_socket_p2p();
	util::Reference<NetConnection> connect_p2p();
	std::pair<util::Reference<NetConnection>,
	    util::Reference<NetConnection>>
	create_socket_pair();
	util::Reference<Identity> identity();
	void init_authentication();
	mrb_sym authentication_status();
	/* not yet supported */
	// bool received_relay_auth_ticket();
	// int find_relay_auth_ticket_for_server();
	util::Reference<NetConnection> connect_to_hosted_dedicated_server();
	uint16_t hosted_dedicated_server_port();
	uint32_t hosted_dedicated_server_pop_id();

	[[nodiscard]] ISteamNetworkingSockets *
	sockets() const
	{
		return _sockets;
	}

	[[nodiscard]] util::Reference<util::State> state() const;

private:
	ISteamNetworkingSockets *_sockets = nullptr;
	util::WeakReference<util::State> _state;
};
} /* namespace euler::net */

#endif /* EULER_NET_SOCKETS_H */
