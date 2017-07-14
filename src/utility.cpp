#include "headers.h"


// Trim from start
static inline string &ltrim(string &s) 
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

// Trim from end
static inline string &rtrim(string &s) 
{
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

// Trim from both ends
static inline string &trim(string &s) {
    return ltrim(rtrim(s));
}

// String split on delimiter
template<typename Out>
void split(const string &s, char delim, Out result) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

vector<string> split(const string &s, const string &del){
    vector<string> elems;
    string s2 = s;
    string del2 = del;
    int pos = 0;
    int start = 0;

    transform(s2.begin(), s2.end(),s2.begin(), ::toupper);
    transform(del2.begin(), del2.end(),del2.begin(), ::toupper);

    while(pos >= 0)
    {
        pos = s2.find(del2, start);
        elems.push_back(s.substr(start, pos-start));
        start = pos + del2.size();
    }
    return elems;
}