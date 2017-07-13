#include<iostream>
#include <vector>
#include "ClientManager.h"

std::vector<Connection> ConnectionManager::connections = std::vector<Connection>();
void ConnectionManager::addconnection(Connection conn) {
	connections.push_back(conn);
}

void ConnectionManager::removeconnection(int id) {
	//remove
}

void ConnectionManager::getpeerinfo(std::string userid, Connection &connection) {
	for (auto con : ConnectionManager::connections) {
		if (con.userid == userid) { connection = con; }
	}
}
