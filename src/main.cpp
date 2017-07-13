#include "functions.cpp"

/**
 * TO-DO
 * ------------------------
 * get database connection string
 * Add SQLAPI++ and connect to database
 * Add query to recover fields's type and length
 * Parse WHERE condition
 * Parse GROUP BY condition
 * Parse ORDER BY condition
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

	for (const auto& p : Tables)
  		cout << p.first << ", \t" << p.second << endl;

  	getFields(query);

  	cout << endl;
  	for (int i = 0; i < Fields.size(); ++i)
  		cout << "Name:" << Fields[i].Name << ", \tAlias:" << Fields[i].Alias << ", \tTable:" << Fields[i].Table << ", \tTableAlias:" << Fields[i].TableAlias << endl;

	return 0;
}