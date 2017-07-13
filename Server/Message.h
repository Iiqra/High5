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

class request {
public:
	char type;
	char sender[6];
	char recipient[6];
	int length;

	unsigned char* buffer;
};

class response {
	enum ResponseType { Invalid, HeaderError, Blocked, Unauthorized };
public:
	ResponseType type;
	char message[19];
};

// 
class MessageHelper {
public:
	static response processmessage(request r);
};

/*
pmsg (r) {

  if(r.tye == register || r.type == login) { 
       // process
	   if(successfull) {
	      // store the socket
	   }
  } else {
	  // store the mst
	  // generate response 
  }

  return response
}
*/