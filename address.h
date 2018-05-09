#ifndef ADDRESS_H_
#define ADDRESS_H_

#include <string>

struct Address {
  int node;
  std::string name;
  Address(int _node,std::string _name) : node(_node),name(_name){}
};

#endif