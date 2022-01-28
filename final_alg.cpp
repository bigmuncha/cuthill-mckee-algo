#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "helper.hpp"
#include <memory>
#include <algorithm>
#include <ranges>

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

    vertex(int i, plain_array<vertex_SPtr> n, plain_array<int> l, unsigned inddex,unsigned new_index,plain_array<int> new_neig)
	:degree(i),neigbords(n),neig_index(l),index(inddex),new_index(new_index),new_neig_index(new_neig) {}
};


//создаю список указателей на вершины из списка смежностей 
plain_array<vertex_SPtr> create_vertex_array (const plain_array<plain_array<int>> &list_smej)
{
    plain_array<vertex_SPtr> map_vert;
    for(int i =0;i < list_smej.size(); i++)
    {
	vertex omar(list_smej[i].size(),plain_array<vertex_SPtr>(),list_smej[i],i,0,plain_array<int>());
	map_vert.push_back(std::make_shared<vertex>(omar));
    }
    for(int i = 0; i< list_smej.size(); i++){
	map_vert[i]->neigbords = [&map_vert](plain_array<int> vect_ind) ->plain_array<vertex_SPtr>{
		plain_array<vertex_SPtr> ret;
		for(auto &a:vect_ind){ ret.push_back(map_vert[a]);}
		return ret;}(map_vert[i]->neig_index);
    }
    return map_vert;
}

// достать индекс вершины с минимальной степенью
int get_min_degree(const plain_array<vertex_SPtr>& vert_list)
{
    int current = 100000;  //предположу, что так много вершин не будет
    int index = 0;
    for(auto& a:vert_list){
	if(a->degree < current){
	    current = a->degree;
	    index = a->index;
	}
    }
    return  index;
}

// достать вершину по индексу O(n)- долго при большом количестве вершин, можно использовать другой контейнер
vertex_SPtr get_vertex_by_index(const int index,const plain_array<vertex_SPtr> vert_list){
    for(auto &a:vert_list)
	if(a->index == index)
	    return a;
    return nullptr;
}

//сортировка массива с вершинами по степени, для более удобного перебора
plain_array<vertex_SPtr> sort_vert_list_by_degree(plain_array<vertex_SPtr> vert_list){
	std::sort(vert_list.begin(),vert_list.end(),
		  [&vert_list]
		  (vertex_SPtr i, vertex_SPtr j){
		      return i->degree < j->degree;
		  });
	return  vert_list;
}

//основной алгоритм
void cuthill_mckee_algo(plain_array<vertex_SPtr>& vert_list){
    int current_ind = get_min_degree(vert_list); // берем индекс минимальной вершины
    std::cout <<"in cuthill mccke"<< current_ind <<"\n";
    std::set<int> pomech ={current_ind}; // кладем ее в набор помеченных вершин(мы там были)
    std::queue<vertex_SPtr> inqueue; // очередь с вершинами, которые мы должны посетить
    auto current_vert = get_vertex_by_index(current_ind,vert_list); // достаем вершину с минимальной степенью
    if(current_vert == nullptr){
	std::cerr <<"Bad vertex\n";
	return ;
    }
    inqueue.push(current_vert); // кладем ее в очередь
    current_vert->new_index = 0; // устанавлимаем ее индекс в 0;
    int i = 1;
    while(!inqueue.empty())
    {
	current_vert = inqueue.front(); //достаем вершину из очереди
	inqueue.pop(); 
	auto sort_vertex_list = sort_vert_list_by_degree(current_vert->neigbords); // сортируем список соседей в текущей вершине
	for(auto& a: sort_vertex_list){ //идем по всем соседям
	    if(pomech.contains(a->index)){
		continue; //пропускаем помеченные
	    }else{
		inqueue.push(a); // кладем в очередь не помеченные 
		pomech.insert(a->index); 
		a->new_index = i++; // увеличиваем текущий индекс
	    }
	}
    }

    for(int i=0; i < vert_list.size(); i++){
	plain_array<int> new_neig; // создаем для каждой вершины новый список соседей
	for(auto &a:vert_list[i]->neig_index){
	    auto cur = get_vertex_by_index(a, vert_list);
	    vert_list[i]->new_neig_index.push_back(cur->new_index);
	}
    }
}

// создаем матрицу из списка смежностей вершин
plain_array<plain_array<int>> get_matrix(const plain_array<vertex_SPtr>& vert_list)
{
    plain_array<plain_array<int> > ret_val(vert_list.size(),plain_array<int>(vert_list.size(),0));
    for(auto&a:vert_list){
	for(auto&s:a->new_neig_index){
	    ret_val[a->new_index][s] = 1;
	}
    }
    return ret_val;
}

void main_algo(plain_array<plain_array<int>> matrix)
{
    prints(matrix); //вывод изначальной матрицы
    auto list_smej = create_list_smej(matrix); //создаю из нее список смежностей
    auto vertex_array = create_vertex_array(list_smej); // создаю из списка смежностей массив с вершинами
    cuthill_mckee_algo(vertex_array); 
    auto new_matr = get_matrix(vertex_array); 
    prints(new_matr); // вывожу результат
}



int main(int argc, char **argv)
{
    if(argc != 2){
	std::cerr<<" bad argc \n" << argc;
	return 0;
    }
    auto matr = fileIn<int>(argv[1]);
    main_algo(matr);
    return 0;
}
