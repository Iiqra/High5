#pragma once 
#include "User.h"
#include "MessageProtocol.h"
int UserEntity::id = 1;
std::vector<UserEntity> UserManager::users = std::vector<UserEntity>();
userauthenticationstatus UserManager::uregister(ACE_SOCK_Stream peer, request &r, response &res) {
	r.buffer = new char[20];
	peer.recv_n(r.buffer, 20); // username01password01
#pragma region parsingUsernamePassword
	char username[10];
	char password[10];
	int bIndex = 0, vIndex = 0;
	username[vIndex++] = r.buffer[bIndex++];// 0-1
	username[vIndex++] = r.buffer[bIndex++];// 1-2
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];// 9-10
											//	username[vIndex++] = '\0';
	vIndex = 0;
	password[vIndex++] = r.buffer[bIndex++];// 0-11
	password[vIndex++] = r.buffer[bIndex++];// 1-12
	password[vIndex++] = r.buffer[bIndex++];// 2- 13
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++]; // 9- 20
											 //	password[vIndex++] = '\0';
											 /*r.buffer = new char[21];
											 r.buffer[21] = '\0';*/

	char u[11];
	char p[11];
	bIndex = 0, vIndex = 0;
	u[vIndex++] = username[bIndex++];// 0-11
	u[vIndex++] = username[bIndex++];// 1-12
	u[vIndex++] = username[bIndex++];// 2- 13
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++]; // 9- 20
	u[vIndex++] = '\0';

	vIndex = 0; bIndex = 0;
	p[vIndex++] = password[bIndex++];// 0-1
	p[vIndex++] = password[bIndex++];// 1-2
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];// 9-10
	p[vIndex++] = '\0';
	vIndex = 0;

#pragma endregion
	for (auto user : users) {
		if (user.userName == username && user.password == password) {
			return userauthenticationstatus::Exist;
		}
	}
	// Create profile
	std::string userId;
	std::stringstream ss;
	ss << "u" << std::setw(5) << std::setfill('0') << UserEntity::id++;
	userId = ss.str();
	UserEntity entity(userId, username, password);

	users.push_back(entity);

	// send response as well
	//	response res;
	res.type = 1;
	std::string userrId = UserManager::getsenderId(u);
	std::string message = userrId + "Registered";
	//res.message = (char*)message.c_str();
	res.buffer = (char*)message.c_str();
	res.length = "0006";
	std::string responseStr = responsehelper::parseresponse(res);

	// User with that username and password does not exist
	return userauthenticationstatus::OK;
}

userauthenticationstatus UserManager::ulogin(ACE_SOCK_Stream peer, request &r, response &res) {

	r.buffer = new char[20];
	peer.recv_n(r.buffer, 20);
#pragma region parsingUsernamePassword
	char username[10];
	char password[10];
	int bIndex = 0, vIndex = 0;
	username[vIndex++] = r.buffer[bIndex++];// 0-1
	username[vIndex++] = r.buffer[bIndex++];// 1-2
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];
	username[vIndex++] = r.buffer[bIndex++];// 9-10
											//	username[vIndex++] = '\0';
	vIndex = 0;
	password[vIndex++] = r.buffer[bIndex++];// 0-11
	password[vIndex++] = r.buffer[bIndex++];// 1-12
	password[vIndex++] = r.buffer[bIndex++];// 2- 13
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++];
	password[vIndex++] = r.buffer[bIndex++]; // 9- 20
											 //	password[vIndex++] = '\0';
											 /*r.buffer = new char[21];
											 r.buffer[21] = '\0';*/

	char u[11];
	char p[11];
	bIndex = 0, vIndex = 0;
	u[vIndex++] = username[bIndex++];// 0-11
	u[vIndex++] = username[bIndex++];// 1-12
	u[vIndex++] = username[bIndex++];// 2- 13
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++];
	u[vIndex++] = username[bIndex++]; // 9- 20
	u[vIndex++] = '\0';

	vIndex = 0; bIndex = 0;
	p[vIndex++] = password[bIndex++];// 0-1
	p[vIndex++] = password[bIndex++];// 1-2
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];
	p[vIndex++] = password[bIndex++];// 9-10
	p[vIndex++] = '\0';
	vIndex = 0;

#pragma endregion

	for (auto user : users) {
		if (user.userName == username) {
			if (user.password == password) {
				return userauthenticationstatus::OK;
			}
			else {
				return userauthenticationstatus::UsernamePasswordMismatch;
			}
		}
	}

	// List had no user with that username
	return userauthenticationstatus::UserNotfound;
}
//
//	void User::registerUser(std::string, std::string password){
//		// take appropriate username and password then assign a unique ID
//	}
//	void User::authenticateUser() {   // add user after authnticate it, either already exists or not
//		//code here. 
//	}
//	void User::blockUser(std::string blockerId, std::string targetId) {
//		// block logic here
//	}
//	void User::searchUser(std::string userName) {
//		// find any user using username againt unique userId given by server.
//	}
//	void User::deleteUser(std::string userId) {
//		// delete user in anycase
//	}
//
