#pragma once

#include <cstdio>
#include <cstdarg>
#include <string>
using std::string;

template<typename... T>
inline string str_format(const char* fmt, T... args) {
    char buf[128] = {0};
    sprintf(buf, fmt, args...);
    return buf;
}

string read_file(const string &path);
