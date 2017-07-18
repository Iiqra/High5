#pragma once
#include<string>


class UserEntity {
public:
	int userId;
	std::string userName;
	std::string password;
	bool isactive;
};

enum userauthenticationstatus { OK, Exist, UsernamePasswordMismatch };

class UserManager {
public:
	static userauthenticationstatus registerUser(std::string, std::string password) {
		// ok and exist cases implementation 

		// #DBCALL 

		return userauthenticationstatus::OK;
		// assign new id aftre checkig if already exists or not 
	}
	static userauthenticationstatus authenticateUser(std::string, std::string password) {
		// it will check in the db againt the or the uname and pasword combination etc.. 
		//#DBCALL
		// ok and usernamepasswordmissmatch cases will be implemented here  
		return userauthenticationstatus::OK;
	}
	static void blockUser(std::string blockerId, std::string targetId);
	static void searchUser(std::string userId);
	static void deleteUser(std::string userId);
	static void updateProfile(std::string userId);
	static std::string getsenderId(std::string userId) {
		return "u00001";
	}
};

/*
1- class diagram finishing.
2- sequence of functions and their structure.

*/#pragma once
