#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

class UserEntity {
public:
	static int id;
	std::string userId;
	std::string userName;
	std::string password;
	
	UserEntity(std::string u, std::string un, std::string p) : userId(u), userName(un), password(p) {}
};

enum userauthenticationstatus { OK, Exist, UsernamePasswordMismatch, UserNotfound, AlreadyLoggedIn };

class UserManager {
public:
	static std::vector<UserEntity> users;
	static userauthenticationstatus registerUser(std::string username, std::string password); 
	
	static userauthenticationstatus authenticateUser(std::string username, std::string password);

	static std::string getsenderId(std::string username);

};

