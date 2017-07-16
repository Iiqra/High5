#include<string>
#include<vector>
#include <ctime>
#include <string>
#include <sstream>
#include "User.h"
// #include "ClientManager.h"


class request;
class response;
 enum MessageType { Register = 1, Login, Message, Group, Broadcast, Image, File };
 enum RequestStatus { Invalid, HeaderError, Blocked, Unauthorized, OK };
// just for the db entity -- ignore for now
//class MessageEntity {
//		
//	
//	/*
//	Register=1, Message=2,Broadcast=3, Multicast=5,Image=41,File=42
//	*/
//public:
//	int Id;
//	// MessageType type;
//	std::string sender;
//	std::string recipient;
//	std::time_t datetime;
//	std::string filepath;
//	std::string filename;
//	std::string fileextension;
//	int length; 
//	std::string actualcontent;
//};

 // IceCreamFactory --> Users Comes --> IceCreateFactory.Create(Strawberry); --> IceCream || IceCreamFactory

// every responsibilty related to protocol somehow

template<class T>
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

private:
	static bool isrequest(T type) {
		std::string(typeid(type).name()) == std::string("class request") ? true : false;
	}

public:
	static T string_to_object(std::string s);
	static std::string object_to_string(T & object) {
		if (isrequest(object)) {
			// Parse the object
		}
		else {
			// Response
		}
	}
	static T validate_request(T r);
	static response process_message(request r); // this message will need following 3 message to get its work done
	static void send_message(request r);
	/*static void broadcast_message(request r);
	static void multicast_message(request r);*/
	static T createrequest<T>(MessageType type, std::string sender, std::string recipient) {}
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
class requesthelper {
public:


	static RequestStatus validate_request(request& r) {
		// if(r.length != r.buffer.size())  {}

		return RequestStatus::OK;
	}

	static void process_message(request& r, RequestStatus status, response &resp) {
		if (status == RequestStatus::OK) {
			switch (r.type)
			{
			case Register:
				// UserHelper regsiter?
				break;
			case Login:
				// UserHelper login
			case Message:
				auto cons = ClientMessage::connections;

				for (auto con : cons) {
					if (r.recipient == con.userid) {
						auto str = responsehelper::parseresponse(resp);
						con.socket.send_n(str, str.size());
					}
				}
				break;
			case Group: 
				auto cons = ClientMessage::connections;

				std::vector<User> users = DbHelper::groupclientinfo();
				for (auto us : users) {
					for (auto con : cons) {
						if (us.userid == con.userid) {
							auto str = responsehelper::parseresponse(resp);
							con.socket.send_n(str, str.size());
						}
					}
				}
				break;
			case Broadcast:
				// 1. Get client sockets
				// 2. Tell DbHelper to log all msgs -- broadcasts go to all
				// 3. Send message to online people. 
				auto cons = ClientManager::connections;
				for (auto con : cons) {
					con.stream.send_n();
				}
				break;
				// Others
			}
		}
		else if (status == RequestStatus::Blocked) {
			resp.type == ResponseType::Blocked;
		}
		else if (status == RequestStatus::Invalid || status == RequestStatus::HeaderError) {
			// LOg the information
		}
		else if (status == RequestStatus::Unauthorized) {
			// Log unauth access
		}
	}

	static request* createrequest(MessageType type, std::string sender, std::string recipient) {
		switch (type)
		{
		case Register:
			break;
		case Login:
			break;
		case Message:
			break;
		case Group:
			break;
		case Broadcast:
			break;
		case Image:
			break;
		case File:
			break;
		default:
			break;
		}
	}

	static RequestStatus parseheader(std::string str, request &r) {
		int type = std::stoi(&str[0]);

		// Register
		if (type == 1) {
			auto length = new char[4];
			length[0] = str[1];
			length[1] = str[2];
			length[2] = str[3];
			length[3] = str[4];

			int _length = std::stoi(length);
		} // else cases

		// Check validations
		return validate_request(r);
	}

	static std::string parserequest(request& r) {
		std::stringstream str;
		switch (r.type)
		{
		case Register:
			str << "1";
			str << "0020";
			str << r.buffer;
			break;
		case Login:
			str << "2";
			str << "0020";

			str << r.buffer;
			break;
		case Message:
			str << "3";
			str << r.sender;
			str << r.recipient;
			str << r.length;
			str << r.buffer;
			break;
		case Group:

			str << "4";
			str << r.sender;
			str << r.recipient;
			str << r.length;
			str << r.buffer;


			break;
		case Broadcast:
			str << "5";
			str << r.sender;
			str << r.recipient;
			str << r.length;
			str << r.buffer;
			break;
		case Image:
			break;
		case File:
			break;
		default:
			break;
		}
		return str.str();
	}
};

// make
enum ResponseType { Invalid, HeaderError, Blocked, Unauthorized, OK };
class response {
public:
	ResponseType type;
};

class responsehelper {
public:
	static response* createresponse(RequestStatus status) {
		response *r = new response();
		switch (status)
		{
		case Invalid:
			r->type = ResponseType::Invalid;
			break;
		case HeaderError:
			r->type = ResponseType::HeaderError;
			break;
		case Blocked:
			r->type = ResponseType::Blocked;
			break;
		case Unauthorized:
			r->type = ResponseType::Unauthorized;
			break;
		case OK:
			r->type = ResponseType::OK;
			break;
		default:
			break;
		}
		return r;
	}

	static std::string parseresponse(response &r) {
		// here.
	}
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




//string --- from string to object ---- validate---process 



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