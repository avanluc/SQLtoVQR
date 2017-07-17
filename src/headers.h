#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> 
#include <map>
#include <functional> 
#include <cctype>
#include <climits>
#include <locale>
#include <vector>
#include <D:/Projects/SQLtoVQR/include/SQLAPI.h> // main SQLAPI++ header

using namespace std;


// GLOBAL VARS

struct field 
{
	string Table;
	string TableAlias;
	string Name;
	string Alias;
	string Type;
	int lenght;
	int decimal;
};

struct filter 
{
	string Table;
	string TableAlias;
	string Name;
	string Value;
	string Criteria;
	string ConcatExp;
	bool Not;
	bool Having;
};


map<int,string> joinType;
vector<field> 	Fields;   				// Vector of fields
vector<filter> 	Filters; 				// Vector of filters
vector<string> 	GroupBy;				// Vector of groups
vector<pair<string,string>> Tables;		// Pair Table - Alias
vector<pair<string,string>> OrderBy;	// Pair Field - Direction
//vector<pair<int,int>> Association;  	// Pair Field Index - Table Index
vector<string> Criteria = {"<>", " like ", ">=", "<=", "<", ">", "=", " in ", " is null ", " is not null "};	// Symbols used in filter statements
