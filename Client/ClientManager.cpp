#include<iostream>
#include <vector>
#include "ClientManager.h"

std::vector<Connection> ClientManager::connections = std::vector<Connection>();
void ClientManager::addconnection(Connection conn) {
	connections.push_back(conn);
}

void ClientManager::removeconnection(int id) {
	//remove
}

//void ClientManager::getpeerinfo(std::string userid, Connection &connection) {
//	for (auto con : ClientManager::connections) {
//		if (con.userid == userid) { connection = con; }
//	}
//}


// Groups
std::vector<Group> GroupManager::groups = std::vector<Group>();
std::vector<Connection> Group::connections = std::vector<Connection>();

void GroupManager::addgroups() {
	// 2 groups --> same pattern, g0000
	groups.push_back(Group("g00odd"));
	groups.push_back(Group("g0even"));
}

void GroupManager::addconnection(std::string name, Connection& c) {
	for (auto group : groups) {
		if (group.name == name) {
			group.connections.push_back(c);
		}
	}
}
std::string GroupManager::getuserlist(char groupId[6]) {
	std::string conns; 
	for (auto g : GroupManager::groups) {
		if (g.name == groupId) {
			for (auto c : g.connections) {
				conns += c.userid + ", ";
			}
			break;
		}
	}
	return conns;
}