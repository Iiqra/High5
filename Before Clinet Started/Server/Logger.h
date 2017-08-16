#include <sstream>
#include <string>
#include <stdio.h>
#include <ctime>

inline std::string NowTime() {
	time_t t;
	return ctime(&t);
}

enum TLogLevel { logERROR, logWARNING, logINFO, logDEBUG };

class LogManager
{
public:
	std::ostringstream& Get(TLogLevel level = logINFO) {

	}
	void logger(TLogLevel level, int l, int ecode, time_t dt, std::string coninfo) {

	}
	
   private:
	int line;
	int errorcode;
	std::string datetime;
	std::string clientinfo;
};

