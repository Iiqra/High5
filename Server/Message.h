#include<string>
#include<vector>
#include <ctime>

class MessageEntity {
	enum MessageType {Register, Login,Message, Group, Broadcast, Image, File};
	
	/*
	Register=1, Message=2,Broadcast=3, Multicast=5,Image=41,File=42
	*/
public:
	int Id;
	MessageType type;
	std::string sender;
	std::string recipient;
	std::time_t datetime;
	std::string filepath;
	std::string filename;
	std::string fileextension;
	int length;
	std::string actualcontent;

};
class MessageHelper {

	void readmessage(MessageEntity);
	bool validateheader();
	void processmessage(std::string actionType);
	void sendresponse(std::string);
	void discardmessage(MessageEntity, int id);

};


class request :public MessageEntity {


};

class response {



};