#include<iostream>
#include <map>
class DbConnection
{
public:
	enum CommandType { user, logger, exception };

	template<class T>
	int Create(CommandType, T & object);
	int Update(CommandType, void* object);
	int Delete(std::string storedProcedure, int id);
	void GetAll(std::string storedProcedure);
	void GetOne(std::string storedProcedure, int id);

private:
	   std::string  _connectionString;
	   std::string _connection;
	   std::string  _command;
	   std::string  _adapter;
	   std::string  _datatable;
	 int _status;
};
	

/*

class 

x=connection not found..
logger -> (,,
*/