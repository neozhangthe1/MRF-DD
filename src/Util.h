#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif
#include <vector>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h> //I've ommited this line.
#ifndef _WINSOCKAPI_
struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};
#endif
extern int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif



#define SIZE(A) ((int)A.size())

bool same_type_label(int num_labels, int l1, int l2);

class MappingDict
{
public:
    map<string, int>    dict;
    vector<string>      keys;
    
    int GetSize() const { return keys.size(); }
    
    int GetId(const string& key);            // insert if not exist
    int GetIdConst(const string& key) const; // return -1 (if not exist)

    string GetKeyWithId(const int id) const; // return "" (if not exist)

    void SaveMappingDict(const char* file_name);
    void LoadMappingDict(const char* file_name);
};

class CommonUtil
{
public:
    static vector<string> StringTokenize(string line);
    static vector<string> StringSplit(string line, char separator);
};

class MatrixUtil
{
public:
    static double* GetDoubleArr(int size)
    {
        double* arr = new double[size];
        return arr;
    }

    static void DoubleArrFill(double* arr, int size, double v)
    {
        for (int i = 0; i < size; i ++)
            arr[i] = v;
    }
};