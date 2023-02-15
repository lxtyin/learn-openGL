//
// Created by lx_tyin on 2022/11/24.
//

#include "tool.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

string read_file(const string &path){
    ifstream fin;
    string res;
    //这两行确保ifstream能抛出异常
    fin.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        fin.open(path);
        stringstream stream;
        stream << fin.rdbuf();
        res = stream.str();
        fin.close();
    } catch(std::ifstream::failure e) {
        cerr << "tool: Fail to read " << path << ", " << e.what() << endl;
    }
    return res;
}

