#pragma once
#include<string>
#include <map>

class UserEntity {
public:
	int userId;
	std::string userName;
	std::string password;
	bool isactive;
};

enum userauthenticationstatus { OK, Exist, UsernamePasswordMismatch, UserNotfound };
std::map<int, std::pair<std::string, std::string>> usertable;
std::map<int, std::pair<std::string, std::string>>::iterator it;
int id = 0;

class UserManager {
public:
	static userauthenticationstatus registerUser(std::string username, std::string password) {
	
		printf("\nInside register request!");
		
		
		for (it = usertable.begin(); it != usertable.end(); it++)
		{
			if (username != it->second.first)
			{
				usertable.insert(std::make_pair(id++, std::make_pair(username, password)));
			}

			else {

				return userauthenticationstatus::Exist;
			}
		}
		return userauthenticationstatus::OK;
		// assign new id aftre checkig if already exists or not 
	}
	static userauthenticationstatus authenticateUser(std::string, std::string password) {
		

		//if (usertable.find(//id)!= 

		for (it = usertable.begin(); it != usertable.end(); it++)
		{
			if (usertable.find(id) != usertable.end()) {
				return userauthenticationstatus::UserNotfound;
			}
			//if (it->first !)
		}




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
