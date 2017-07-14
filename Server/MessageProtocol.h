#include<string>
#include<vector>
#include <ctime>
#include <string>
#include "User.h"

static enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
// just for the db entity -- ignore for now
class MessageEntity {
		
	
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

// every responsibilty related to protocol somehow
class ProtocolManager {
	enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
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

public:
	template<class T>
	static T string_to_object(std::string s);
	template<class T>
	static std::string object_to_string(T & object);
	static request validate_request(request r);
	static response process_message(request r); // this message will need following 3 message to get its work done
	static void send_message(request r);
	static void broadcast_message(request r);
	static void multicast_message(request r);

};
//enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
class request {
public:
	MessageType type;
	char sender[6];
	char recipient[6];
	 int length;
	char* buffer;
};

class response {
	enum ResponseType { Invalid, HeaderError, Blocked, Unauthorized };
public:
	ResponseType type;
	std::string message;
};


//class MessageHelper {
//public:
//	static response processmessage(request r);
///*
//acceptrequest();
//validaterequest();
//processrequest();
//sendresponse();
//
//*/ 
//	
//
//
//	/*instead of individual calls we can have only one call of process message
//	b/c we have to perform all these pre-steps for all the messages */
//};
//
//
//class action {
//public:
//	void sendmessage();
//	void multicast();
//	void broadcast(/*message or text*/);
//	void sendimage();
//
//
//};








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