#ifndef NETWORK_MANAGER_H_
#define NETWORK_MANAGER_H_

#include <string>
#include <exception>

namespace tafl
{
	namespace network
	{
		class network_error : public std::exception
		{
		public:
			network_error(std::string msg);
			virtual ~network_error();
			const char* what() const noexcept;
		private:
			std::string msg_;
		};

		class network_manager
		{
		public:
			network_manager();
			virtual ~network_manager();
			bool open(std::string host, std::string port);
		private:
			bool is_open_;
			int sfd_;

		public:
#define getter(field)											\
			decltype(field##_) field(void) { return field##_; }
			getter(is_open);
#undef getter
		};
	}
};

#endif /* NETWORK_MANAGER_H_ */
