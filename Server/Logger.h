#include <sstream>
#include <string>
#include <stdio.h>
#include <ctime>
inline std::string NowTime();

enum TLogLevel { logERROR, logWARNING, logINFO, logDEBUG };

class Log
{
public:
	Log();
	virtual ~Log();
	std::ostringstream& Get(TLogLevel level = logINFO);
	void logger(TLogLevel level , int l, int ecode, time_t dt, std::string coninfo);
	
   private:
	std::ostringstream os;
	int line;
	int Errorcode;
	time_t datetime;
	std::string connectioninfo;
};

