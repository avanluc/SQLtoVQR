#include "functions.cpp"

/**
 * TO-DO
 * ------------------------
 * get database connection string
 * Add SQLAPI++ and connect to database
 * Add query to recover fields's type and length
 * Parse WHERE condition
 * Parse JOINS
 * Parse GROUP BY condition
 * Parse ORDER BY condition
 * Flags controls
 * Error handling
 * Write on file
 **/


int main(int argc, char **args)
{
	string file_name;
	string query;
	joinType.insert(pair<int,string>(0,"Left outer"));
	joinType.insert(pair<int,string>(1,"Right outer"));
	joinType.insert(pair<int,string>(2,"Inner"));
	joinType.insert(pair<int,string>(3,"Full"));
	joinType.insert(pair<int,string>(4,"Where"));
	
	// Read file path
	if (argc > 1)
		file_name = args[1];
	else
	{
		cout << "Inserire il file contenente la query da tradurre:" << endl;
		cin >> file_name;
	}

	query = readQuery(file_name);

	getTables(query);

	cout << endl << "TABLE LIST" << endl << "----------------------------" << endl;
	for (const auto& p : Tables)
  		cout << "Table: " << setw(18) << left << p.first
  			 << "Alias: " << setw(15) << left << p.second << endl;

  	getFields(query);

	cout << endl << "FIELD LIST" << endl << "----------------------------" << endl;
  	for (int i = 0; i < Fields.size(); ++i)
  		cout << "Name: "  << setw(15) << left << Fields[i].Name 
  			 << "Alias: " << setw(15) << left << Fields[i].Alias 
  			 << "Table: " << setw(18) << left << Fields[i].Table 
  			 << "TableAlias: " << setw(15) << left << Fields[i].TableAlias << endl;

  	cout << endl;
  	getFilters(query);



/*
  	SAConnection dbConn;

  	try
  	{
  		dbConn.Connect("localhost\\SQL2014@x_infinity_demo24", "", "", SA_SQLServer_Client);
  		cout << "We're connected! <3" << endl;
  		dbConn.Disconnect();
  		cout << "We're disconnected! :(" << endl;
  	}
  	catch(SAException &e)
  	{
  		printf("%s\n", (const char*)e.ErrText());
  	}
*/
	return 0;
}
