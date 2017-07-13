#include<iostream>
#include <map>
class DbConnection
{
public:

	enum commandType {user, logger, exception};
	template<class T>
	//int Create(std::string storedProcedure, std::map<std::string, std::string> parameters);
	int Create(commandType, T & object);
	int Update(commandType, void* object);
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