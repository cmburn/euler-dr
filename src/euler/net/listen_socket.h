/* SPDX-License-Identifier: ISC */

#ifndef EULER_NET_LISTEN_SOCKET_H
#define EULER_NET_LISTEN_SOCKET_H

#include <cstdint>
#include <steam/steamnetworkingtypes.h>

#include "euler/util/object.h"
#include "euler/net/ip_address.h"

namespace euler::net {
class Sockets;

class ListenSocket final : public util::Object {
public:

	ListenSocket();
	~ListenSocket() override;

	void close();
	[[nodiscard]] util::Reference<IPAddress> address() const;

private:
	HSteamListenSocket _socket = k_HSteamListenSocket_Invalid;
	util::Reference<Sockets> _sockets;
};
} /* namespace euler::net */


#endif /* EULER_NET_LISTEN_SOCKET_H */

