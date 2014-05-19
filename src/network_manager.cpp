#include "network_manager.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

tafl::network::network_manager::network_manager()
	: is_open_(false)
{

}

tafl::network::network_manager::~network_manager()
{
	if(is_open_)
		close(sfd_);
}

bool tafl::network::network_manager::open(std::string host, std::string port)
{
	int s;
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	s = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	if(s != 0)
	{
		return false;
		// Maybe use exception : throw network_error(string("getaddrinfo: ")  + string(gai_strerror(s)));
	}

	for(rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(sfd_ == -1)
			continue;
		if(connect(sfd_, rp->ai_addr, rp->ai_addrlen) != -1)
			break;
		close(sfd_);
	}

	if(rp == NULL)
	{
		freeaddrinfo(result);
		return false;
		// same as above throw network_error("Couldn't connect");
	}
	freeaddrinfo(result);
	is_open_ = true;
	return true;
}

tafl::network::network_error::network_error(string msg)
	: exception()
	, msg_(msg)
{
}

tafl::network::network_error::~network_error()
{

}

const char* tafl::network::network_error::what() const noexcept
{
	return msg_.c_str();
}
