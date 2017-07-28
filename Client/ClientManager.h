#pragma once
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include <iostream>
#include <map>

class Connection {
public:
	int id; // (int)'A'  
	ACE_SOCK_Stream *socket;
	std::string userid; // Not needed now, right?

	Connection(int i, ACE_SOCK_Stream *sock, std::string userid) : 
		id(i), socket(sock), userid(userid) {}
};

class Group {
//	friend ostream operator<<(const Group& obj);
public:
	std::string name;
	std::vector<Connection> connections;

	Group(std::string n) : name(n) {
		connections = std::vector<Connection>();
	}
};

struct ConnectionCompare
{
	bool operator() (const Connection& lhs, const Connection& rhs) const
	{
		return lhs.userid < rhs.userid;
	}
};

 class GroupManager {
public:
	static std::vector<Group> groups;
	static std::map<Connection, std::string, ConnectionCompare> _allconnections;
	static void addconnection(std::string groupname, Connection& c);
	static void addgroups();

	static std::string getuserlist(char groupId[6]);
};

class ClientManager {

public:
	static std::vector<Connection> connections;

	// Only gets triggered on successfull logins and successful logouts. 
	static void addconnection(Connection conn);

	// Successfull logouts.
	static void removeconnection(int id);

	static bool clientexists(ACE_INET_Addr& add) {
		ACE_Addr a;
		for (auto con : connections) {
			if (con.socket->get_remote_addr(a) == 0) {
				if (a == add) {
					return true;
				}
			}
		}
		return false;
	}

	// Information for a specific client.
	// static void getpeerinfo(std::string userid, Connection &connection);
};