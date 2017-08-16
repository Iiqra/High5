#pragma once 
#include "User.h"
#include "_ClientManager.h"
int UserEntity::id = 1;

std::vector<UserEntity> UserManager::users = std::vector<UserEntity>();
 userauthenticationstatus UserManager::registerUser(std::string username, std::string password) {
	for (auto user : users) {
		if (user.userName == username) {
			return userauthenticationstatus::Exist;
		}
	}
	// Create profile
	std::string userId;
	std::stringstream ss;
	ss << "u" << std::setw(5) << std::setfill('0') << UserEntity::id++;
	userId = ss.str();
	UserEntity entity(userId, username, password);

	// You had commented this out as well. :| 
	users.push_back(entity);

	// User with that username and password does not exist
	return userauthenticationstatus::OK;
}

 userauthenticationstatus UserManager::authenticateUser(std::string username, std::string password) {
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

	 for (auto con : ClientManager::connections) {

		 if (con.userid == UserManager::getsenderId(username))
			 return userauthenticationstatus::AlreadyLoggedIn;
	 }



	 // List had no user with that username
	 return userauthenticationstatus::UserNotfound;
 }
  std::string UserManager::getsenderId(std::string username) {
	 for (auto user : users) {
		 if (user.userName == username) {
			 return user.userId;
		 }
	 }
	 return "0000NA"; // User does not exist
 }