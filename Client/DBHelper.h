#include <string>
#include <vector>
#include <ctime>

class MessageEntity {
	enum MessageType { Inbox, Group, Broadcast };
public:

	int id;
	std::string sender;
	std::string recipient;
	std::time_t datetime;
	MessageType type;
	std::string filepath;
	std::string filename;
	std::string textcontent;
};

// 1|Bar|Foo|123456|1     --> { 1, "Bar", "Foo", 123456, 1 }
// 2|Foo|Barney|654321|2
// 3|Foo|ALL|13579|3

class MessageTable {
public:
	static std::vector<MessageEntity> fetch() {

	}

	static void deletemessage(int id) {

	}


};

class UserTable {

};
