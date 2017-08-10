// This is the main DLL file.
#pragma once
#include "stdafx.h"
#include "ManagedWrapper.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\Dll\Dll\MsgProtocol.h"
#include "C:\Users\iali\Documents\Visual Studio 2015\Projects\Dll\Dll\MsgProtocol.cpp"

ManagedWrapper::requestWrapper::requestWrapper(){
	sender = gcnew array<Byte>(6);
	recipient = gcnew array<Byte>(6);
}



ManagedWrapper::responseWrapper::responseWrapper() {
	// constructor of responsewrapper class 
}


void ManagedWrapper::responsehelperWrapper::getlength(char ** c, int l) {

	std::stringstream ss;
	ss << std::setw(4) << std::setfill('0') << l;
	*c = _strdup(ss.str().c_str());
}



ManagedWrapper::requesthelperWrapper::requesthelperWrapper() {
	// constructor of requestehelperwrapper class 

}
ManagedWrapper::responsehelperWrapper::responsehelperWrapper() {
	// constructor of responsehelperwrapper class 

}
