/* SPDX-License-Identifier: ISC */

#ifndef EULER_NET_NET_CONNECTION_H
#define EULER_NET_NET_CONNECTION_H

#include <limits>
#include <string>
#include <vector>

#include "euler/util/object.h"

// #include <steam/steamnetworkingtypes.h>

namespace euler::net {
class Message;
class Identity;
class ListenSocket;
class IPAddress;

class NetConnection final : public util::Object {
public:
	static constexpr int MAX_MESSAGES = std::numeric_limits<int>::max();
	struct SendFlags {
		bool reliable : 1 = false;
		bool no_nagle : 1 = false;
		bool no_delay : 1 = false;
		bool auto_restart_broken_session : 1 = false;
		bool use_current_thread : 1 = false;
		[[nodiscard]] int to_int() const;
		SendFlags();
		SendFlags(const util::Reference<util::State> &state,
		    mrb_value config);
	};

	struct ConnectionInfo {
		struct Flags {
			bool unauthenticated : 1 = false;
			bool unencrypted : 1 = false;
			bool loopback_buffers : 1 = false;
			bool fast : 1 = false;
			bool relayed : 1 = false;
			bool dual_wifi : 1 = false;
			Flags();
			[[nodiscard]] int to_int() const;
		};

		ConnectionInfo(const util::Reference<util::State> &state,
		    mrb_value config);
		util::Reference<Identity> identity_remote;
		util::Reference<ListenSocket> listen_socket;
		util::Reference<IPAddress> address_remote;
		uint32_t pop_id_remote;
		uint32_t pop_id_relay;
		int state;
		int end_reason;
		std::string end_debug;
		std::string connection_description;
		Flags flags;
	};

	enum class ConnectionState {
		None,
		Connecting,
		FindingRoute,
		Connected,
		ClosedByPeer,
		ProblemDetectedLocally,
	};

	struct ConnectionRealTimeLaneStatus {
		int pending_unreliable;
		int pending_reliable;
		int sent_unacked_reliable;
		int64_t usec_queue_time;
	};

	struct ConnectionRealTimeStatus {
		std::vector<ConnectionRealTimeLaneStatus> lanes;
		ConnectionState state;
		int ping;
		float connection_quality_local;
		float connection_quality_remote;
		float out_packets_per_sec;
		float out_bytes_per_sec;
		float in_packets_per_sec;
		float in_bytes_per_sec;
		int send_rate_bytes_per_second;
		int pending_unreliable;
		int pending_reliable;
		int sent_unacked_reliable;
		int64_t usec_queue_time;
	};

	void accept();
	void close(int reason = 0, const char *debug = nullptr,
	    bool linger = false);
	void set_name(const char *name);
	[[nodiscard]] const char *name() const;
	int64_t send_message(const void *data, uint32_t size,
	    SendFlags flags = SendFlags());
	void flush_messages();
	std::vector<util::Reference<Message>> receive_messages(
	    int max = MAX_MESSAGES);
	ConnectionInfo connection_info();
	ConnectionRealTimeStatus connection_real_time_status();
	std::string detailed_connection_status();

private:
};
} /* namespace euler::net */

#endif /* EULER_NET_NET_CONNECTION_H */
