#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "helper.hpp"
#include <memory>
#include <algorithm>
#include <ranges>
#include <assert.h>





int main(int argc, char **argv)
{
    if(argc != 2){
	std::cerr<<" bad argc \n" << argc;
	return 0;
    }
    auto matr = fileIn<int>(argv[1]);
    std::cout <<matr.size() <<" " << matr[0].size() <<'\n';

    
    return 0;
}


