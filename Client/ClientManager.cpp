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
