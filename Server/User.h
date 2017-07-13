#include<string>

class User {
	int userId;
	std::string userName;
	std::string password;
public:
	void registerUser(std::string, std::string password);
	void authenticateUser();
	void blockUser(std::string blockerId, std::string targetId);
	void searchUser(std::string userName);
	void deleteUser(std::string userId);

};
