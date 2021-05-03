#ifndef OSPF_H
#define OSPF_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Router.h"

typedef std::vector< std::vector<int> > ForwardingTable;
std::vector<Router> synchronize_routers(std::vector< std::vector<int> >);
extern std::stringstream error_message;

#endif
