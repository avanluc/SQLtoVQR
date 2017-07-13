#include <iostream>
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


map<int,string> joinType;
vector<pair<string,string>> Tables;	// Pair Table - Alias
//vector<pair<string,string>> Fields; // Pair Field - Alias
vector<field> Fields; // Pair Field - Alias
vector<pair<int,int>> Association; // Pair Field Index - Table Index

