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

static inline int FindPATTERN(std::string str, std::string pattern){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	std::transform(pattern.begin(), pattern.end(),pattern.begin(), ::toupper);
	return str.find(pattern, 0);
}


/**
 * Find the join pose of a specific join based on the type parameter 
 *
 * @param str String with from statement.
 * @param type Int representing the join type.
 * @return position of the specific join in str.
 */
static inline int FindJOINS(std::string str, int type){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	std::string join = joinType.at(type);
	std::transform(join.begin(), join.end(),join.begin(), ::toupper);
	return str.find(join, 0);
}


/**
 * Find the first join in the string str
 *
 * @param str String with from statement.
 * @param offset If true include the lenght of the join string.
 * @return position of the first join in str.
 */
static inline int FindFirstJOIN(std::string str, bool offset = false){	
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	int min = INT_MAX;
	for(auto const &ent : joinType) 
	{
		// TO-DO : Sistemare il tipo di join, se la query non contiene esattamente la stringa va in errore!!!	
		std::string join = ent.second + " join";
		std::transform(join.begin(), join.end(),join.begin(), ::toupper);
		// if not found (res = -1) return INT_MAX otherwise the join pos in the string
		min = std::min(min, (int)(str.find(join, 0) == -1 ? INT_MAX : (offset ? str.find(join, 0) + join.size() : str.find(join, 0))));
		//std::cout << "type: " << join << ", pos: " << str.find(join, 0) << ", min: " << min << std::endl;
		
	}
	return min == INT_MAX ? -1 : min;
}


/**
 * Fill the Tables vector with the pair phisic_table, alias
 *
 * @param query String with the SQL query.
 */
void getTables(std::string query){
	int start = FindFROM(query);
	int end   = FindWHERE(query);
	int end1  = FindGROUPBY(query);
	int end2  = FindORDERBY(query);

	std::vector<std::string> v;
	std::string table;
	std::string fromString;

	fromString = query.substr(start, (end != -1 ? end : (end1 != -1 ? end1 : (end2 != -1 ? end2 : query.size())))-start);

	// Get first table
	start = FindFirstJOIN(fromString);
	if(start > 0)
		table = fromString.substr(5, start - 5);
	else 
		table = fromString.substr(5, start);

	v = split(table, ' ');
	Tables.push_back(std::pair<std::string, std::string>(v.at(0), v.at(1)));
	
	// update start position and searched string
	start = FindFirstJOIN(fromString, true);
	
	while (start > 0)
	{
		//std::cout << "start: "  << start << " size: "  << fromString.size()  << " len: "   << fromString.size()-start << std::endl;
		fromString = fromString.substr(start, fromString.size()-start);
		end = FindPATTERN(fromString, " on ");
		//std::cout << "start: "  << start  <<  " size: "  << fromString.size() << " len: "   << end-1  << " str: "   << fromString << std::endl;
		v.empty();
		v = split(fromString.substr(1, end-1), ' ');
		Tables.push_back(std::pair<std::string, std::string>(v.at(0), v.at(1)));
		start = FindFirstJOIN(fromString, true);
	}
}


/**
 * Find the next comme in a select statement
 *
 * @param selectString String with select statement.
 * @return position of the first comma not included in parenthesis.
 */
int findNextComma(std::string selectString){
	int count = 0;
	for(std::string::size_type i = 0; i < selectString.size(); ++i) {
    	if( selectString[i] == ',' && count == 0)		return i;
    	if( selectString[i] == '(' )	count++;
    	if( selectString[i] == ')' )	count--;
	}
	return -1;
}


/**
 * Read the SQL query from file
 *
 * @param file String with the file path.
 * @return query statement red from file.
 */
std::string readQuery(std::string name){
	std::string line;
	std::string query;
	std::ifstream file;

	// Convert backslash into slash
	for ( std::string::iterator it=name.begin(); it!=name.end(); ++it)
		if(*it == '\\')
			name.replace(it, it+1, "/");

	file.open(name);
	while (std::getline(file, line))
		query += trim(line) + " ";
	file.close();

	return query;
}
