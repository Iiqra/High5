#include "log.h"
#include <cstdlib>

int main1() {
	Log *pLog = new Log("errors.log");
	pLog->Write("Going into our loop");
	for (int i = 0; i < 10; i++) {
		char c[50];
		sprintf_s(c, "Looped: %d times", i);
		pLog->Write(c);
	}
	return 0;
}