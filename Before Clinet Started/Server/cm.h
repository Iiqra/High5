#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"

class Connection {
public:
	int id;
	ACE_SOCK_Stream *socket;
	std::string userid;

	Connection(int i, ACE_SOCK_Stream *sock, std::string userid) :
		id(i), socket(sock), userid(userid) {}
};

class ClientManager {

public:
	static std::vector<Connection> connections;

	static void addconnection(Connection conn);

	static void removeconnection(int id);

	static void getpeerinfo(std::string userid, Connection &connection);
