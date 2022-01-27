#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <span>
#include <queue>
#include <map>

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

template <typename T>
ostream& operator<<(ostream& out, vector<T> const& v)
{
    for(auto &a: v)
	out<< a <<' ';
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, vector<vector<T>> const& v)
{
    for(auto &am: v){
	for(auto &em: am)
	    std::cout <<em << " ";
	std::cout <<"\n";
    }
    return out;
}

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
	std::cout <<i<<": ";
	for(auto &b :cont[i] )
	    std::cout << b <<" ";
	std::cout <<'\n';
    }
}

inline std::vector<std::vector<int>> create_list_smej(const std::vector<std::vector<int>>& matr)
{
    std::vector<std::vector<int>> res;
    for(int i=0; i < matr.size(); i++)
    {
	std::vector<int> temp;
	for(int j =0; j < matr[i].size(); j++)
	{
	    if(matr[i][j] == 1){
		temp.push_back(j);
	    }
	}
	res.push_back(temp);
    }
    return res;
}

template <typename T>
class my_vect : public std::vector<T>
{
public:
    int degree;
};

inline std::map<unsigned,my_vect<int>> create_map_smej(const std::vector<std::vector<int>>& matr)
{
    std::map<unsigned,my_vect<int>> res;
    for(int i=0; i < matr.size(); i++)
    {
	my_vect<int> temp;
	for(int j =0; j < matr[i].size(); j++)
	{
	    if(matr[i][j] == 1){
		temp.push_back(j);
	    }
	}
	res[i] = temp;
    }
    return res;
}
