#include "utility.cpp"

static inline int FindSELECT(string str){
	transform(str.begin(), str.end(),str.begin(), ::toupper);	
	return str.find("SELECT", 0);
}

static inline int FindFROM(string str){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("FROM", 0);
}

static inline int FindWHERE(string str){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("WHERE", 0);
}

static inline int FindGROUPBY(string str){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("GROUP BY", 0);
}

static inline int FindORDERBY(string str){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str.find("ORDER BY", 0);
}

static inline int FindPATTERN(string str, string pattern){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	transform(pattern.begin(), pattern.end(),pattern.begin(), ::toupper);
	return str.find(pattern, 0);
}


/**
 * Find the join pose of a specific join based on the type parameter 
 *
 * @param str String with from statement.
 * @param type Int representing the join type.
 * @return position of the specific join in str.
 */
static inline int FindJOINS(string str, int type){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	string join = joinType.at(type);
	transform(join.begin(), join.end(),join.begin(), ::toupper);
	return str.find(join, 0);
}


/**
 * Find the first join in the string str
 *
 * @param str String with from statement.
 * @param offset If true include the lenght of the join string.
 * @return position of the first join in str.
 */
static inline int FindFirstJOIN(string str, bool offset = false){	
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	int MIN = INT_MAX;
	for(auto const &ent : joinType) 
	{
		// TO-DO : Sistemare il tipo di join, se la query non contiene esattamente la stringa va in errore!!!	
		string join = ent.second + " join";
		transform(join.begin(), join.end(),join.begin(), ::toupper);
		// if not found (res = -1) return INT_MAX otherwise the join pos in the string
		MIN = min(MIN, (int)(str.find(join, 0) == -1 ? INT_MAX : (offset ? str.find(join, 0) + join.size() : str.find(join, 0))));
	}
	return MIN == INT_MAX ? -1 : MIN;
}


/**
 * Fill the Tables vector with the pair phisic_table, alias
 *
 * @param query String with the SQL query.
 */
void getTables(string query){
	int start = FindFROM(query);
	int end   = FindWHERE(query);
	int end1  = FindGROUPBY(query);
	int end2  = FindORDERBY(query);

	vector<string> v;
	string table;
	string fromString;

	fromString = query.substr(start, (end != -1 ? end : (end1 != -1 ? end1 : (end2 != -1 ? end2 : query.size())))-start);

	// Get first table
	start = FindFirstJOIN(fromString);
	if(start > 0)
		table = fromString.substr(5, start - 5);
	else 
		table = fromString.substr(5, start);

	v = split(table, ' ');
	Tables.push_back(pair<string, string>(v.at(0), v.at(1)));
	
	// update start position and searched string
	start = FindFirstJOIN(fromString, true);
	
	while (start > 0)
	{
		//cout << "start: "  << start << " size: "  << fromString.size()  << " len: "   << fromString.size()-start << endl;
		fromString = fromString.substr(start, fromString.size()-start);
		end = FindPATTERN(fromString, " on ");
		//cout << "start: "  << start  <<  " size: "  << fromString.size() << " len: "   << end-1  << " str: "   << fromString << endl;
		v.empty();
		v = split(fromString.substr(1, end-1), ' ');
		Tables.push_back(pair<string, string>(trim(v.at(0)), trim(v.at(1))));
		start = FindFirstJOIN(fromString, true);
	}
}


/**
 * Find the next comme in a select statement
 *
 * @param selectString String with select statement.
 * @return position of the first comma not included in parenthesis.
 */
int findNextComma(string selectString){
	int count = 0;
	for(string::size_type i = 0; i < selectString.size(); ++i) 
	{
    	if( selectString[i] == ',' && count == 0)		return i;
    	if( selectString[i] == '(' )					count++;
    	if( selectString[i] == ')' )					count--;
	}
	return -1;
}


/**
 * Read the SQL query from file
 *
 * @param file String with the file path.
 * @return query statement red from file.
 */
string readQuery(string name){
	string line;
	string query;
	ifstream file;

	// Convert backslash into slash
	for ( string::iterator it=name.begin(); it!=name.end(); ++it)
		if(*it == '\\')
			name.replace(it, it+1, "/");

	file.open(name);
	while (getline(file, line))
		query += trim(line) + " ";
	file.close();

	return query;
}

/**
 * Look for the physical name of the table with the given alias
 *
 * @param alias String with the table alias.
 * @return table physical name.
 */
string resolveAssociations(string alias){

	for (int i = 0; i < Tables.size(); ++i)
		if(alias.compare(Tables[i].second) == 0 )
			return Tables[i].first;
	return "";
}

/**
 * Fill the Fields vector with the pair field, alias
 *
 * @param query String with the SQL query.
 */
void getFields(string query){
	// remove SELECT statement fron string
	int start = FindSELECT(query) + 7;
	int end = FindFROM(query);

	string stmt;
	string tmp;
	vector<string> v;
	string::size_type i;

	stmt = query.substr(start, end-start);
	int pos = 0;

	while(pos >= 0)
	{
    	pos = findNextComma(stmt.substr(0,-1));
    	tmp = stmt.substr(0, pos);
    	v.push_back(trim(tmp));
    	stmt = stmt.substr(pos+1, -1);
	}

	for (int i = 0; i < v.size(); ++i)
	{
		field currField;
		string _name;
		string _alias;
		string _tableAlias;
		string _type;
		int _lenght; 
		int _dec;

		// !!! Vincolo di separare campo e alias con la stringa as
		int aliasPos = FindPATTERN(v[i], " AS ");
		if(aliasPos == -1)
		{
			//Fields.push_back(pair<string, string>(v[i], ""));
			int pPos = v[i].find(".");
			_alias = "";
			_name = v[i].substr(pPos+1, -1);
			_tableAlias = pPos == -1 ? "" : v[i].substr(0, pPos);
		}
		else
		{
			//Fields.push_back(pair<string, string>(v[i].substr(0, aliasPos), v[i].substr(aliasPos+4, v[i].size()-4)));
			int pPos = v[i].find(".");
			_alias = v[i].substr(aliasPos+4, v[i].size()-4);
			_name = v[i].substr(pPos+1, aliasPos - v[i].find("."));
			_tableAlias = pPos == -1 ? "" : v[i].substr(0, pPos);
		}
		currField.Name = trim(_name);
		currField.Alias = trim(_alias);
		currField.TableAlias = trim(_tableAlias);
		currField.Table = resolveAssociations(trim(_tableAlias));
		Fields.push_back(currField);
	}

}

