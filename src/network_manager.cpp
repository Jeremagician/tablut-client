#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>

#include "network_manager.hpp"
#include "game.hpp"

extern "C"
{
#include "tafl-lib/packet.h"
#include "tafl-lib/struct_io.h"
}
using namespace std;

tafl::network::network_manager::network_manager(game* game)
	: is_open_(false)
	, game_(game)
	, thread_(nullptr)
{

}

tafl::network::network_manager::~network_manager()
{
	if(is_open_)
		close(sfd_);
	if(thread_ != nullptr)
	{
		/*
		  MEMORY LEAK MUHAHAHAHA
		thread_->join();
		delete thread_;
		*/
	}
}

void tafl::network::network_manager::send_move(int sx, int sy, int dx, int dy)
{
	if(!is_open_)
		return;

	struct packet p;
	p.data.move.from.x = sx;
	p.data.move.from.y = sy;
	p.data.move.to.x = dx;
	p.data.move.to.y = dy;
	struct_io_write_packet_header(&p.header, sfd_);
	struct_io_write_move(&p.data.move, sfd_);
}

void tafl::network::network_manager::run(void)
{
	bool flag = true;
	while(game_->running_)
	{
		struct packet p;
		struct_io_read_packet_header(&p.header, sfd_);
		struct_io_read_game_state(&p.data.game_state, sfd_);

		for(int i = 0; i < p.data.game_state.move_count; i++)
		{
			if(p.data.game_state.moves[i].type == 0)
			{
				game_->board_->pawn_add(p.data.game_state.moves[i].from.x,
										p.data.game_state.moves[i].from.y,
										static_cast<board::pawn_type>(p.data.game_state.moves[i].to.x));
			}
			else if(p.data.game_state.moves[i].type == 1)
			{
				game_->board_->pawn_remove(p.data.game_state.moves[i].from.x,
										p.data.game_state.moves[i].from.y);
			}
			else if(p.data.game_state.moves[i].type == 2)
			{
				game_->board_->pawn_move(p.data.game_state.moves[i].from.x,
										 p.data.game_state.moves[i].from.y,
										 p.data.game_state.moves[i].to.x,
										 p.data.game_state.moves[i].to.y);
			}
		}

		if(flag)
		{
			flag = !flag;
			if(p.data.game_state.state == 0)
				game_->team_ = game::MUSCOVITE;
			else
				game_->team_ = game::SWEDISH;
		}

		game_->should_play_ = p.data.game_state.state == 0;
	}
	mutex mtx;
	mtx.lock();
	game_->running_ = false;
	mtx.unlock();
}

void tafl::network::network_manager::start(void)
{
	thread_ = new thread(&network_manager::run, this);
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
