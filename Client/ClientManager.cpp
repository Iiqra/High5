#include<iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <utility>
#include "ClientManager.h"

std::vector<Connection> ClientManager::connections = std::vector<Connection>();
void ClientManager::addconnection(Connection conn) {
	connections.push_back(conn);
}

void ClientManager::removeconnection(int id) {
	int index = 0;
	for (auto _ : connections) {
		if (_.id == id) {
			connections.erase(connections.begin() + index);
			return;
		}
		index++;
	}

}

int GroupManager::mapId = 0;
int Group::id = 3;

//void ClientManager::getpeerinfo(std::string userid, Connection &connection) {
//	for (auto con : ClientManager::connections) {
//		if (con.userid == userid) { connection = con; }
//	}
//}


// Groups
std::vector<Group> GroupManager::groups = std::vector<Group>();
// std::vector<Connection> Group::connections = std::vector<Connection>();

void GroupManager::defaultgroups() {
	// 2 groups --> same pattern, g0000
	groups.push_back(Group("g00001", "Tech Talks")); //techtalks  -- enter t
	groups.push_back(Group("g00002", "Foooobaaar")); //foobar    -- entre f 
} 

bool GroupManager::groupExists(std::string groupname) {
	for (auto _ : groups) {
		if (_.name == groupname) {
			return true;
		}
	}
	return false;
}

//void GroupManager::addgroup(std::string n) {
//	groups.push_back(n);
//}

void GroupManager::getconnections(std::string groupId, std::vector<Connection>& connections) {
	for (auto _ : _allconnections) {
		if (_.second.second == groupId) {
			connections.push_back(_.second.first);
		}
	}
}
 
std::map<int, std::pair<Connection, std::string>> GroupManager::_allconnections =
std::map<int, std::pair<Connection, std::string>>();

groupauthentication GroupManager::creategroup(std::string groupname) {
	// if group already exist?
	for (auto _g : GroupManager::groups) {
		if (_g.friendly_name == groupname) {
			return  groupauthentication::Exist;
		}
	}

	// Create group
	std::string groupid;
	std::stringstream ss;
	ss << "g" << std::setw(5) << std::setfill('0') << Group::getId();
	groupid = ss.str();
	GroupManager::groups.push_back(Group(groupid, groupname));
	return  groupauthentication::Created;
}

void GroupManager::removeFromAll(std::string username) {
	for (auto _ : _allconnections) {
		if (_.second.first.userid == username) {
			_allconnections.erase(_.first);
		}
	}
}

std::string GroupManager::getGroupId(std::string groupname) {
	for (auto _ : groups) {
		if (_.friendly_name == groupname) {
			return _.name;
		}
	}
	return "g0ERROR";
}

groupauthentication GroupManager::joinGroup(std::string name, Connection& c) {
	// If pair exists
	std::pair<Connection, std::string> mypair = std::pair<Connection, std::string>();
	mypair.first = c;
	mypair.second = name;
	for (auto _ : _allconnections) {
		if (_.second == mypair) {
			// Connection exists in group
			return groupauthentication::Exist;
		}
	}

	GroupManager::_allconnections[GroupManager::mapId++] = mypair;
	return groupauthentication::Added;
}

std::string GroupManager::getuserlist(char groupId[6]) {
	std::string conns; 
	for (auto c : GroupManager::_allconnections) {
		// first --> key int
		// second --> value std::pair
		if (c.second.second == std::string (groupId)) {
			conns += c.second.first.userid + ", ";
		}
	}
	return conns;
}