#include "functions.cpp"


int main(int argc, char **args)
{
	std::string file_name;
	std::string query;
	joinType.insert(std::pair<int,std::string>(0,"Left outer"));
	joinType.insert(std::pair<int,std::string>(1,"Right outer"));
	joinType.insert(std::pair<int,std::string>(2,"Inner"));
	joinType.insert(std::pair<int,std::string>(3,"Full"));
	joinType.insert(std::pair<int,std::string>(4,"Where"));
	
	// Read file path
	if (argc > 1)
		file_name = args[1];
	else
	{
		std::cout << "Inserire il file contenente la query da tradurre:" << std::endl;
		std::cin >> file_name;
	}

	query = readQuery(file_name);

	getTables(query);

	for (const auto& p : Tables)
  		std::cout << p.first << ", \t" << p.second << std::endl;

	return 0;
}