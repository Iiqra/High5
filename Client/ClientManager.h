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
	Connection() {}

	bool operator ==(const Connection& c1) const {
		return (this->userid == c1.userid) ? true : false;
	}
};

enum class GroupAvailability { Public, Private };

class Group {
	// friend ostream operator<<(const Group& obj);
	static int id;

public:
	std::string name;
	std::string friendly_name;
	std::string adminid;
	bool _private;

	// std::vector<Connection> connections;
	Group(std::string n, std::string f, std::string a, bool p)
		: name(n), friendly_name(f), adminid(a), _private(p) {
		// connections = std::vector<Connection>();
	}

	static int getId() { return id++; }
};

enum class groupauthentication { Created, Exist, Added, Private /*ask admin to add you*/ };
class GroupManager {
	static int mapId;
public:
	static std::vector<Group> groups;
	// std::map<std::string, int> ist;

	static std::map<int, std::pair<Connection, std::string>> _allconnections;

	static std::string getGroupId(std::string groupname);
	static groupauthentication joinGroup(std::string groupname, Connection& c, std::string memberid);
	static void defaultgroups();
	// static void addgroup(std::string);
	static groupauthentication creategroup(std::string groupname, std::string admin, bool _public);
	static std::string getuserlist(char groupId[6]);
	static void getconnections(std::string groupId, std::vector<Connection>& connections);
	static bool groupExists(std::string groupname);

	static void removeFromAll(std::string username);
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
