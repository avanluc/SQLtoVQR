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
	Tables.push_back(pair<string, string>(v[0], (v.size()>1 ? v[1] : "")));
	
	// update start position and searched string
	start = FindFirstJOIN(fromString, true);
	
	while (start > 0)
	{
		fromString = fromString.substr(start, fromString.size()-start);
		end = FindPATTERN(fromString, " on ");
		v.empty();
		v = split(fromString.substr(1, end-1), ' ');
		string tmp = (v.size()>1 ? v[1] : "");
		Tables.push_back(pair<string, string>(trim(v[0]), trim(tmp)));
		start = FindFirstJOIN(fromString, true);
	}
}


/**
 * Find the next comme in a select statement
 *
 * @param selectString String with select statement.
 * @return position of the first comma not included in parenthesis.
 */
int findNextAND_OR(string str){
	int And = FindPATTERN(str, " AND ");
	int Or = FindPATTERN(str, " OR ");
	if (And == -1 && Or == -1)	return -1;
	if (And == -1)				return Or+4;
	if (Or == -1)				return And+4;
	return min(And+5, Or+4);
}


/**
 * Find the next comma in a select statement
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
 * Remove comment from code
 *
 * @param query String with the query.
 * @param mode if 0 remove block comments, else remove line comments.
 * @return query statement clean from comments.
 */
string removeComments(string query, int mode){
	int start = 0;
	int end = 0;

	if (mode == 0)
	{
		while((start = query.find("/*")) >= 0)
		{
			end = query.find("*/", start) + 2;
			query.erase(start, end-start);
		}
	}
	else
	{
		int start = 0;
		if((start =  query.find("--")) >= 0)
			query.erase(start, -1);
	}
	return query;
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
		query += removeComments(trim(line), 1) + " ";
	file.close();

	return removeComments(query, 0);
}

/**
 * Look for the physical name of the table with the given alias
 *
 * @param alias String with the table alias.
 * @return table physical name.
 */
string resolveAssociations(string alias){

	for (int i = 0; i < Tables.size(); ++i)
		if(alias.compare(Tables[i].first) == 0 || alias.compare(Tables[i].second) == 0)
			return Tables[i].first;
	return "";
}

/**
 * Fill the Fields vector with the info in the field struct data
 *
 * @param query String with the SQL query.
 */
void getFields(string query){
	// Remove SELECT statement fron string
	int start = FindSELECT(query) + 7;
	int end = FindFROM(query);

	string stmt;
	string tmp;
	vector<string> v;
	string::size_type i;

	stmt = query.substr(start, end-start);
	int pos = 0;

	// Extract fields
	while(pos >= 0)
	{
    	pos = findNextComma(stmt);
    	tmp = stmt.substr(0, pos);
    	v.push_back(trim(tmp));
    	stmt = stmt.substr(pos >= 0 ? pos+1 : 0, -1);
	}

	// Parse fields to extract alias and table
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
			int pPos = v[i].find(".");
			_alias = "";
			_name = v[i].substr(pPos+1, -1);
			_tableAlias = pPos == -1 ? "" : v[i].substr(0, pPos);
		}
		else
		{
			int pPos = v[i].find(".");
			_alias = v[i].substr(aliasPos+4, v[i].size()-4);
			_name = v[i].substr(pPos+1, aliasPos - v[i].find("."));
			_tableAlias = pPos == -1 ? "" : v[i].substr(0, pPos);
		}
		if (_alias.empty())
			_alias = _name;

		// TO-DO : extract type and lenght from DB

		currField.Name = trim(_name);
		currField.Alias = trim(_alias);
		currField.TableAlias = trim(_tableAlias);
		currField.Table = (trim(_tableAlias).empty()) ? "" : resolveAssociations(trim(_tableAlias));
		Fields.push_back(currField);
	}

}


/**
 * Fill the Filters vector with the info in the filter struct data
 *
 * @param query String with the SQL query.
 */
void getFilters(string query){
	int start = FindWHERE(query);
	int end  = FindGROUPBY(query);
	int end2  = FindORDERBY(query);
	string stmt;
	string tmp;
	vector<string> v;

	// If there is no WHERE then exit
	if(start == -1)
		return;

	// Shift start posizion after WHERE string
	start += 6;

	stmt = query.substr(start, (end != -1 ? end : (end2 != -1 ? end2 : query.size()))-start);
	int pos = 0;

	// Extract filters
	while(pos >= 0)
	{
    	pos = findNextAND_OR(stmt);
    	tmp = stmt.substr(0, pos);
    	v.push_back(trim(tmp));
   		stmt = stmt.substr(pos >= 0 ? pos : 0, -1);
	}

	// Parse filters to extract struct data
	for (int i = 0; i < v.size(); ++i)
	{
		filter currFilter;
		vector<string> v2;
		
		if (FindPATTERN(v[i], "OR") != -1)
		{
			currFilter.ConcatExp = "OR";
			v[i] = v[i].substr(0, FindPATTERN(v[i], "OR"));
		}
		else if (FindPATTERN(v[i], "AND") != -1)
		{
			currFilter.ConcatExp = "AND";
			v[i] = v[i].substr(0, FindPATTERN(v[i], "AND"));
		}

		// Extract criteria
		for (int j = 0; j < Criteria.size(); ++j)
		{
			v2 = split(v[i], Criteria[j]);
			if (v2.size() > 1)
			{
				currFilter.Criteria = Criteria[i];
				break;
			}
		}

		currFilter.Name = v2[0];
		currFilter.Value = ((v2.size() > 1) ? v2[1] : "");
		currFilter.Not = 0;
		currFilter.Having = 0;

		// extract table
		for (int k = 0; k < Tables.size(); ++k)
		{
			if((int)v2[0].find(Tables[k].first + ".") >= 0 || (!Tables[k].second.empty() && (int)v2[0].find(Tables[k].second + ".") >= 0) )
			{
				currFilter.Table = Tables[k].first;
				currFilter.TableAlias = Tables[k].second;
			}
		}

		Filters.push_back(currFilter);
	}
	return;
}


/**
 * Fill the GroupBy vector with the Fields to group by
 *
 * @param query String with the SQL query.
 */
void getGroupBy(string query){
	int start  = FindGROUPBY(query);
	int end  = FindORDERBY(query);
	string stmt;
	string tmp;
	vector<string> v;

	// If there is no GROUP BY then exit
	if(start == -1)
		return;

	// Shift start posizion after GROUP BY string
	start += 9;

	stmt = query.substr(start, (end != -1 ? end : query.size())-start);

	int pos = 0;
	// Extract goups
	while(pos >= 0)
	{
    	pos = findNextComma(stmt);
    	tmp = stmt.substr(0, pos);
    	v.push_back(trim(tmp));
   		stmt = stmt.substr(pos >= 0 ? pos+1 : 0, -1);
	}

	// Insert fields into vector
	for (int i = 0; i < v.size(); ++i)
		GroupBy.push_back(v[i]);

	return;
}


/**
 * Fill the OrderBy vector with the Field - Direction pair
 *
 * @param query String with the SQL query.
 */
void getOrderBy(string query){
	int start  = FindORDERBY(query);
	string stmt;
	string tmp;
	vector<string> v;

	// If there is no ORDER BY then exit
	if(start == -1)
		return;

	// Shift start posizion after ORDER BY string
	start += 9;

	stmt = query.substr(start, query.size()-start);

	int pos = 0;
	// Extract sorting
	while(pos >= 0)
	{
    	pos = findNextComma(stmt);
    	tmp = stmt.substr(0, pos);
    	v.push_back(trim(tmp));
   		stmt = stmt.substr(pos >= 0 ? pos+1 : 0, -1);
	}

	// Parse sorting to extract field and direction
	for (int i = 0; i < v.size(); ++i)
	{
		string field;
		string direction;
		
		int p = v[i].rfind(" ");
		field = v[i].substr(0, p);
		direction = (p >= 0) ? v[i].substr(p, -1) : "asc";

		if( isInteger(field) ) 
			field = Fields[atoi(field.c_str())-1].Alias;

		OrderBy.push_back(pair<string,string>(trim(field), trim(direction)));
	}
	return;
}