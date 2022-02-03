#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <span>
#include <queue>
#include <map>
#include "colors.hpp"
#include <algorithm>
#include <iomanip>

using namespace std;

template <class T>
concept container =
    requires(T a){
    a.begin();
    a.end();
};

template <class T>
concept pair_if =
    requires(T a){
    a.begin()->first;
    a.end()->second;
};

template <pair_if T>
void printer(const T& container)
{
    for(auto&& [a,b] : container){
	std::cout << a << " " << b <<'\n';
    }
}

template<typename T>
void prints(const std::vector<std::vector<T>>& cont)
{

    for(int i =0; i < cont.size(); i++){
	int sum = 0;
	for(auto &a:cont[i])
	    sum +=a;
	std::cout.setf(std::ios::left);  //equivalent: cout << right;
	if(sum)
	    std::cout <<i<<"F "<<std::setw(5)<< ": ";
	else
	    std::cout <<i<<"S " <<std::setw(5)<< ": ";

	for(auto &b :cont[i] ){
	    if(b != 0)
		std::cout<<std::setw(1) << BOLD(FBLU("1 "));
	    else
		std::cout<<std::setw(1)<<"O ";
	}
	//     std::cout << b <<" ";
	 std::cout <<'\n';
    }
}

// template <typename T>
// class my_vect : public std::vector<T>
// {
// public:
//     int degree;
// };

// inline std::map<unsigned,my_vect<int>> create_map_smej(const std::vector<std::vector<int>>& matr)
// {
//     std::map<unsigned,my_vect<int>> res;
//     for(int i=0; i < matr.size(); i++)
//     {
// 	my_vect<int> temp;
// 	for(int j =0; j < matr[i].size(); j++)
// 	{
// 	    if(matr[i][j] == 1){
// 		temp.push_back(j);
// 	    }
// 	}
// 	res[i] = temp;
//     }
//     return res;
// }

template <typename T>
ostream& operator<<(ostream& out, vector<T> const& v)
{
    for(auto &a: v)
	out<< a <<' ';
    return out;
}

using namespace std;
template <typename T>
vector<vector<T>> fileIn( const char* filename )
{
   vector< vector<T> > arr;
   ifstream in( filename );
   if ( in )
   {
      for ( string line; getline( in, line ); )
      {
	  stringstream ss( line );
	  vector<int> row;
	  for ( int i; ss >> i; )
	      row.push_back( i );
	  arr.push_back( row );
      }
   }
   else
   {
      cerr << "Boo! No such file\n";
   }
   return arr;
}

