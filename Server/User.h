#include<string>

class User {
	int userId;
	std::string userName;
	std::string password;
	bool isactive;
public:
	void registerUser(std::string, std::string password);
	void authenticateUser();
	void blockUser(std::string blockerId, std::string targetId);
	void searchUser(std::string userId);
	void deleteUser(std::string userId);
	void updateProfile(std::string userId);
};



/*
1- class diagram finishing.
2- sequence of functions and their structure. 
 
*/