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

enum userauthenticationstatus { OK, Exist, UsernamePasswordMismatch, UserNotfound };
// int userIdGlobal = 0;

class UserManager {
public:
	static std::vector<UserEntity> users;

	static userauthenticationstatus registerUser(std::string username, std::string password) {
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

		// User with that username and password does not exist
		return userauthenticationstatus::OK;
	}

	static userauthenticationstatus authenticateUser(std::string username, std::string password) {
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

	/*static void blockUser(std::string blockerId, std::string targetId);
	static void searchUser(std::string userId);
	static void deleteUser(std::string userId);
	static void updateProfile(std::string userId);*/

	static std::string getsenderId(std::string username) {
		for (auto user : users) {
			if (user.userName == username) {
				return user.userId;
			}
		}
		return "0000NA"; // User does not exist
	}
};

/*
1- class diagram finishing.
2- sequence of functions and their structure.

*/#pragma once
