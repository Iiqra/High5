#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"

class Connection {
public:
	int id;
	ACE_SOCK_Stream *socket;
	std::string userid;

	Connection(int i, ACE_SOCK_Stream *a, std::string u) : id(i), socket(a), userid(u) {}
};

class ConnectionManager {
public:
	static std::vector<Connection> connections;

	static void addconnection(Connection conn);

	static void removeconnection(int id);


	static void getconnectionforuser(std::string userid, Connection &connection);
};