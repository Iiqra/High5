//#pragma once 
//#include "MessageProtocol.h"
//
//
// int requesthelper::request_reader(ACE_SOCK_Stream& _peer, std::string & container, int len){
//	char* buffer = new char[len];
//	container = "";
//	buffer[len] = '\0';
//	int read = _peer.recv_n(buffer, len);
//	if (read == len) {
//		container += buffer;
//		delete buffer;
//		return 1;
//	}
//	else {
//		delete buffer;
//		return 0;
//	}
//}
