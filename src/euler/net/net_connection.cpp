/* SPDX-License-Identifier: ISC */

#include "euler/net/net_connection.h"

using euler::net::NetConnection;

int
NetConnection::SendFlags::to_int() const
{
	int flags = 0;
	if (reliable) flags |= k_nSteamNetworkingSend_Reliable;
	if (no_nagle) flags |= k_nSteamNetworkingSend_NoNagle;
	if (no_delay) flags |= k_nSteamNetworkingSend_NoDelay;
	if (auto_restart_broken_session)
		flags |= k_nSteamNetworkingSend_AutoRestartBrokenSession;
	if (use_current_thread)
		flags |= k_nSteamNetworkingSend_UseCurrentThread;
	return flags;
}

NetConnection::SendFlags::SendFlags() = default;
NetConnection::ConnectionInfo::Flags::Flags() = default;
