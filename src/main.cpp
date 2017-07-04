#include "utility.cpp"



static inline int FindSELECT(std::string str){
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);	
	return str.find("SELECT", 0);
}

static inline int FindFROM(std::string str){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("FROM", 0);
}

static inline int FindWHERE(std::string str){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("WHERE", 0);
}

static inline int FindGROUPBY(std::string str){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("GROUP BY", 0);
}

static inline int FindORDERBY(std::string str){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("ORDER BY", 0);
}


void getTables(std::string query){
	int start = FindFROM(query);
	int end   = FindWHERE(query);
	int end1  = FindGROUPBY(query);
	int end2  = FindORDERBY(query);

	std::cout << query.substr(start, (end != -1 ? end : (end1 != -1 ? end1 : (end2 != -1 ? end2 : query.size())))-start) << std::endl;
}

int main(int argc, char **args)
{
	std::string file_name;
	std::string query;
	std::string line;
	
	// Read file path
	if (argc > 1)
		file_name = args[1];
	else
	{
		std::cout << "Inserire il file contenente la query da tradurre:" << std::endl;
		std::cin >> file_name;
	}

	// Convert backslash into slash
	for ( std::string::iterator it=file_name.begin(); it!=file_name.end(); ++it)
		if(*it == '\\')
			file_name.replace(it, it+1, "/");

	// Read file
	std::ifstream file(file_name);

	while (std::getline(file, line))
	{
		query += trim(line) + " ";
		//query.push_back('\n');
	}

	// Print query for debug
	std::cout << std::endl << query << std::endl;

	getTables(query);

	std::map<int,std::string> joinType;
	joinType.insert(std::pair<int,std::string>(0,"Left outer"));
	joinType.insert(std::pair<int,std::string>(1,"Right outer"));
	joinType.insert(std::pair<int,std::string>(2,"Inner"));
	joinType.insert(std::pair<int,std::string>(3,"Full"));
	joinType.insert(std::pair<int,std::string>(4,"Where"));

	return 0;
}


class Join {

	public:
		int type;
		std::string table1;
		std::string table2;
		std::string condition;
};

