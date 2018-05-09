#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <string>
#include <sstream>     
#include "address.h"
#include <iostream>
#include <fstream>


std::vector<std::string> split(const std::string &str, char sep)
{
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
    }
    return v;
}

std::vector<Address> readAddress(std::string filename){
    std::vector<Address> v = {};
    std::ifstream ifs(filename);
    std::string s;
    while(std::getline(ifs,s)){
        auto vec = split(s,',');
        v.push_back(Address(std::stoi(vec.at(0)),vec.at(1)));
    }
    return v;
}

#endif