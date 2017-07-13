//#include<string>
//#include<vector>
//#include <ctime>
//#include "Message.h"
//#include <iostream>
//
//class request {
//public:
//	char type;
//	char sender[6];
//	char recipient[6];
//	int length;
//
//	unsigned char* buffer;
//};
//
//void processmessage(std::string raw, request & r) {
//	int pointer = 0;
//	r.type = std::stoi(std::string(&raw[pointer++])); // 9999 9999
//
//	int indexer = 0;
//	r.sender[indexer++] = raw[pointer++];
//	r.sender[indexer++] = raw[pointer++];
//	r.sender[indexer++] = raw[pointer++];
//	r.sender[indexer++] = raw[pointer++];
//	r.sender[indexer++] = raw[pointer++];
//	r.sender[indexer++] = raw[pointer++];
//	
//	indexer = 0;
//	r.recipient[indexer++] = raw[pointer++];
//	r.recipient[indexer++] = raw[pointer++];
//	r.recipient[indexer++] = raw[pointer++];
//	r.recipient[indexer++] = raw[pointer++];
//	r.recipient[indexer++] = raw[pointer++];
//	r.recipient[indexer++] = raw[pointer++];
//
//	char length[8];
//	indexer = 0;
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//	length[indexer++] = raw[pointer++];
//
//	r.length = std::stoi(std::string(length));
//	r.buffer = new unsigned char[r.length];
//
//	auto buffersize = pointer + r.length;
//	indexer = 0;
//	while (pointer < buffersize) {
//		if (pointer == raw.length()) {
//			break;
//		}
//		r.buffer[indexer++] = raw[pointer++];
//	}
//}
//
//// { 3 (1..128) | Foo (6) | Bar (6) | 6,00000 }
//
//void printintent(request request) {
//	// Assume this.
//	if (request.type == 1) {
//		// Text
//		std::cout << "[Text] Message sent by " << request.sender << " to " << request.recipient << "." << std::endl;
//		std::cout << "[Length = " << request.length << "]" << request.buffer << std::endl;
//	}
//	else if (request.type == 2) {
//		// Binary
//		std::cout << "[Binary] Message sent by " << request.sender << " to " << request.recipient << "." << std::endl;
//		std::cout << "[Length = " << request.length << "]" << "[BLOB]" << std::endl;
//	}
//}
//
//int main() {
//	std::string first = "1pervrtfineld00000010,wfrr0jfw";  // type 1 byte  1
//	// "Type=1|From=Bar|To=Bar|Length=255|hdfknfkjsf"
//	//  1. Split into string arrays ... using | 
//	std::string second = "1pervrtfineld0000009f43xtc34t";
//	std::string third = "2pervrtfineld000000074tcxf";
//
//	request a, b, c;
//	processmessage(first, a);
//	processmessage(second, b);
//	processmessage(third, c);
//
//	printintent(a);
//	printintent(b);
//	printintent(c);
//
//	system("pause");
//}
//
//void MessageEntity::fetchMessage(MessageEntity){
//// client will click on action from C# code and respective message will be formed here
//}
//bool MessageEntity::validateMessage() {
//	// validate the header - 17bytes and only then make buffer for payload and read it
//	/*
//	1 byte for type
//	6 bytes for senderId
//	6 byutes for receiverId
//	4 bytes for size of the data
//	*/
//
//	// read header bytes and then decide next.
//	return 0;
//}
//void MessageEntity:: processMessage(std::string actionType){
//	//const int length = msg.length;
//	//char buffer[msg.length];
//
//}
//
//
//// Id | Type | Date | Message | Misc -- string, blob, int primary key jstdie uname, jldj  1   xxxxu1 
//
///*
//server:    logger-> gjhjkk
//
//Logger::log(LogType::error, date(), "Any message", senderId, ipEndpoint);
// info error, user error, exception code 
//static void log(...) {
//   CpSQliteDb db;
//   db.execDML("insert into");-- 
//}
//
//*/