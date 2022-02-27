#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <queue>
#include <map>
#include "colors.hpp"
#include <algorithm>
#include <iomanip>
#include <memory>
#include <tuple>

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
	// for(auto &a:cont[i])
	//     sum +=a;
	// std::cout.setf(std::ios::left);  //equivalent: cout << right;
	// if(sum)
	//     std::cout <<i<<"F "<<std::setw(5)<< ": ";
	// else
	//     std::cout <<i<<"S " <<std::setw(5)<< ": ";

	for(auto &b :cont[i] ){
	    if(b != 0)
		std::cout<<std::setw(1) << BOLD(FRED("1 "));
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


template <typename T>
bool matrix_is_symmetrix( vector<vector<T>> matrix)
{
    for(int i=0;i < matrix.size(); i++)
    {
	for(int j =0; j < matrix[i].size(); j++)
	{
	    if(!(matrix[i][j] == matrix[j][i]))
		return false;
	}
    }
    return true;
}

template <typename T>
std::tuple<int,int,int> get_another_matrix_wide(const vector<vector<T>>& list_smej)
{
    unsigned int result = 0;
    int first_index=0;
    int last_vertex = 0;

    for(int i =0; i < list_smej.size(); i++){
	for(auto a: list_smej[i]){
	    if(abs(i - a) > result){
		result = abs(i - a);
		first_index = i;
		last_vertex = a;
	    }
	}
    }

    return std::make_tuple(result, first_index, last_vertex);

}

template <typename T>
int get_matrix_wide(vector<vector<T>> matrix)
{
    int d_size = matrix.size() / 2;
    int result = 0;
    for(int i =0; i < matrix.size(); i++){
	for (int j = 0; j < matrix.size(); j++){
	    if(matrix[i][j]!=0){
		if(abs(j - i) > result)
		    result = abs(j -i);
	    }
	}
    }
    return result;
}

//создаю список смежностей из матрицы
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



class vertex;
//typedef для удобной работы
typedef std::shared_ptr<vertex> vertex_SPtr; //указатель на вершину

template<class T>
using plain_array = std::vector<T>;

//структура для вершины
struct vertex
{
    int degree; //степень
    plain_array<vertex_SPtr> neigbords; //массив указателей на соседей
    plain_array<int> neig_index; // массив индексов соседей
    unsigned index; //начальный индекс
    unsigned new_index; // индекс после работы алгоритма
    plain_array<int> new_neig_index; // новый список соседей

    vertex(int i, plain_array<vertex_SPtr> n,
	   plain_array<int> l, unsigned inddex,
	   unsigned new_index,plain_array<int> new_neig)
	:degree(i),neigbords(n),neig_index(l),
	 index(inddex),new_index(new_index),
	 new_neig_index(new_neig) {}
};


template <typename T>
std::tuple<int,int,int> get_new_matrix_wide(vector<vertex_SPtr>& list_smej)
{
    unsigned int result = 0;
    int first_index=0;
    int last_vertex = 0;

    for(auto vert: list_smej)
    {
	int index = vert->new_index;
	for(auto neig_ind:vert->new_neig_index){
	    if(abs(index - neig_ind) > result){
		result = abs(index - neig_ind);
		first_index = index;
		last_vertex = neig_ind;
	    }
	}
    }
    return std::make_tuple(result, first_index,last_vertex);
}
