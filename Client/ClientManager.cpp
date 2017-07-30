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
// std::vector<Connection> Group::connections = std::vector<Connection>();

void GroupManager::addgroups() {
	// 2 groups --> same pattern, g0000
	groups.push_back(Group("gtecht")); //techtalks  -- enter t
	groups.push_back(Group("gfubar")); //foobar    -- entre f 
} 

void GroupManager::addgroup(std::string n) {
	groups.push_back(n);
}

void GroupManager::getconnections(std::string groupId, std::vector<Connection>& connections) {
	for (auto _ : _allconnections) {
		if (_.second == groupId) {
			connections.push_back(_.first);
		}
	}
}
 
std::map<Connection, std::string, ConnectionCompare> GroupManager::_allconnections =
std::map<Connection, std::string, ConnectionCompare>();

void GroupManager::addconnection(std::string name, Connection& c) {
	for (auto group : groups) {
		GroupManager::_allconnections[c] = name;
	} 
}

std::string GroupManager::getuserlist(char groupId[6]) {
	std::string conns; 
	for (auto c : GroupManager::_allconnections) {
		// first --> key
		// second --> value
		if (c.second == std::string (groupId)) {
			conns += c.first.userid + ", ";
		}
	}
	return conns;
}