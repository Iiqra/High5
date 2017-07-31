//#include "ace/Reactor.h"
//#include <iostream>
//#include "ace/Svc_Handler.h"
//#include "ace/Acceptor.h"
//#include "ace/Synch.h"
//#include "ace/SOCK_Acceptor.h"
//#include <string>
//#include <cctype>
//#include "ace/OS.h"
//#include "ClientManager.h"
//#include "MessageProtocol.h"
//#include "QueueManager.h"
//#include "User.h"
//
//class MyServiceHandler; //forward declaration
//
//char id = 'A';
//std::string ids = "A";
//
//typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
//typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;
//
//enum RequestTypes { Register = 1, Login, Message, CreateGroup, AddtoGroup, Broadcast };
//enum ActiveGroup { TechTalks = 1, Foodbar, CricketInsighter, Custom };
////enum MessageReceiver { User=1, Group, Offline, Unknown };
//
//class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
//	ACE_thread_t thread_names[1];
//public:
//	int open(void*) {
//		__peer = peer();
//		ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID: (%t) open\n"));
//		activate(THR_NEW_LWP, 1, 1, ACE_DEFAULT_THREAD_PRIORITY, -1, this, 0, 0, 0, thread_names);
//
//		peer().send_n("It works", sizeof("It works"));
//		return 0;
//	}
//
//	void read(void) {
//		std::string container; int status; request r;
//		std::vector<Connection> conVector;
//
//		status = requesthelper::request_reader(__peer, container, 1);
//		if (container == "") {
//			return;
//		}
//		int _type = std::stoi(&container[0]);
//		response _response;
//		response _forward;
//
//		userauthenticationstatus accountSts;
//		std::string __response = "";
//		std::string buffer = "";
//		std::string content; // buffer for user and group message
//		Connection con;
//		std::string username, password;
//		std::string sender = "", recipient = "";
//		std::string buffResp;
//		switch ((RequestTypes)_type)
//		{
//		case Register:
//			status = requesthelper::request_reader(__peer, container, 10);
//			username = container;
//			status = requesthelper::request_reader(__peer, container, 10);
//			password = container;
//			//userauthenticationstatus accountSts; 	std::string username;
//			accountSts = UserManager::registerUser(username, password);
//			if (accountSts != userauthenticationstatus::OK) {
//				_response.type = (int)(ResponseMessage::ExistAlready);
//				responsehelper::parseresponse(_response, "", false);
//			}
//			else {
//				_response.type = (int)(ResponseMessage::RegisterOK);
//				Connection con(id, &this->peer_, UserManager::getsenderId(username));
//				ClientManager::addconnection(con);
//				buffResp = responsehelper::parseresponse(_response, con.userid, false);
//				QueueManager::addresponse(_response);
//				/*	accountSts = UserManager::authenticateUser(username, password);
//				if (accountSts == userauthenticationstatus::OK) {
//				Connection con(id, &this->peer_, UserManager::getsenderId(username));
//				ClientManager::addconnection(con);
//				_response.type = (int)(ResponseMessage::LoginOK);
//				responsehelper::parseresponse(_response, con.userid, false);
//				QueueManager::addresponse(_response);
//				}
//				else if (accountSts == userauthenticationstatus::UsernamePasswordMismatch)
//				{
//
//				_response.type= (int)(ResponseMessage::UsernamePasswordMismatch);
//				responsehelper::parseresponse(_response, "", false);
//				QueueManager::addresponse(_response);
//				}
//				else if (accountSts == userauthenticationstatus::UserNotfound) {
//				_response.type = (int)(ResponseMessage::Notfound);
//				responsehelper::parseresponse(_response, "", false);
//				}	*/
//			}
//			break;
//		case Login:
//			status = requesthelper::request_reader(__peer, container, 10);
//			username = container;
//			status = requesthelper::request_reader(__peer, container, 10);
//			password = container;
//			//userauthenticationstatus accountSts;
//			//accountSts = UserManager::authenticateUser(buffer.substr(0, 9), buffer.substr(10, 19));
//			accountSts = UserManager::authenticateUser(username, password);
//			if (accountSts == userauthenticationstatus::OK) {
//				con = Connection(id, &this->peer_, UserManager::getsenderId(buffer.substr(0, 9)));
//				ClientManager::addconnection(con);
//				responsehelper::parseresponse(_response, con.userid, false);
//			}
//			else if (accountSts == userauthenticationstatus::UsernamePasswordMismatch)
//			{
//				// User not found or password mismatch
//				_response.type = (int)(ResponseMessage::UsernamePasswordMismatch);
//				responsehelper::parseresponse(_response, "", false);
//
//
//			}
//			else if (accountSts == userauthenticationstatus::UserNotfound) {
//				_response.type = (int)(ResponseMessage::Notfound);
//				responsehelper::parseresponse(_response, "", false);
//			}
//			break;// 3Amessage
//		case Message:
//			requesthelper::request_reader(__peer, container, 6);
//			sender = container;
//			requesthelper::request_reader(__peer, container, 6); // u00001
//			recipient = container;
//			if (container[0] == 'u' || container[0] == 'U') {
//				// username = content;
//				_response.type = (int)(ResponseMessage::UserMessage);
//				requesthelper::request_reader(__peer, container, 4);
//				char* msgLength = (char*)container.c_str();
//				int length = std::stoi(msgLength);
//				for (auto _con : ClientManager::connections) {
//					if (_con.userid == username.c_str()) {
//						requesthelper::request_reader(__peer, container, length); // limit of a text
//						content += container;
//						///send call
//						_forward.socket = _con.socket;
//						responsehelper::parseresponse(_forward, content, false);
//						QueueManager::addresponse(_forward);
//						break;
//					}
//				}
//			}
//			else if (container[0] == 'g' || container[0] == 'G') {
//				_forward.type = (int)(ResponseMessage::GroupMessage);
//				content += container.substr(1, 5);
//
//				requesthelper::request_reader(__peer, container, 1000); // limit of a text
//				content += container;
//				for (auto _ : conVector) {
//					_forward.socket = _.socket;
//
//					responsehelper::parseresponse(_forward, content, false);
//				}
//			}
//			break;
//
//		default:
//			break;
//		}
//		//responsehelper::parseresponse(_response,, true);
//		// push into queueu	
//	}
//	static void write(void) {
//		while (1) {
//			// Continue to loop
//			response res;
//			if (QueueManager::getresponse(res) == 1) {
//				std::string responseBuffer = responsehelper::parseresponse(res, "", true);
//				res.socket->send_n(responseBuffer.c_str(), responseBuffer.length());
//
//				//__peer.send_n(responseBuffer.c_str(), 3);
//				ACE_DEBUG((LM_DEBUG, "(%t) QueueThread \n"));
//			}
//		}
//	}
//
//	int svc(void) {
//		ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
//		if (ACE_Thread::self() == thread_names[0]) {
//			while (1) {
//				read();
//			}
//		}
//		return 0; // keep the compiler happy.
//	}
//
//private:
//	ACE_SOCK_Stream __peer;
//	bool readytowrite = false;
//	std::string buffer;
//};
//
//int main(int argc, char* argv[]) {
//	ACE_INET_Addr addr(50009);
//	ACE_DEBUG((LM_DEBUG, "Thread: (%t) main \n"));
//
//	Acceptor myacceptor(addr, Reactor::instance());
//	ACE_Thread::spawn((ACE_THR_FUNC)MyServiceHandler::write);
//	GroupManager::addgroups();
//
//
//	while (1)
//		Reactor::instance()->handle_events();
//	return 0;
//}
//
