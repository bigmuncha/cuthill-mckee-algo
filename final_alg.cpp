#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "helper.hpp"
#include <memory>
#include <algorithm>
#include <assert.h>



//создаю список указателей на вершины из списка смежностей 
plain_array<vertex_SPtr> create_vertex_array (
    const plain_array<plain_array<int>> &list_smej,
    const plain_array<plain_array<int>>& source_matrix)
{
    plain_array<vertex_SPtr> map_vert;
    for(int i =0;i < list_smej.size(); i++)
    {
	int subm = 0;
	 //степень складывается из недиагональных единиц
	if(source_matrix[i][i] != 0) subm = 1; 
	vertex omar(list_smej[i].size() -1 ,
		    plain_array<vertex_SPtr>(),list_smej[i],
		    i,0,plain_array<int>());
	map_vert.push_back(std::make_shared<vertex>(omar));
    }
    for(int i = 0; i< list_smej.size(); i++){
	    auto func = [&map_vert]
	    (plain_array<int> vect_ind) {
		plain_array<vertex_SPtr> ret;
		for(auto &a:vect_ind){ ret.push_back(map_vert[a]);}
		return ret;};
	    map_vert[i]->neigbords = func(map_vert[i]->neig_index);
    }
    return map_vert;
}

// достать индекс вершины с минимальной степенью
int get_min_degree(const plain_array<vertex_SPtr>& vert_list)
{
    int current = 100000;  //предположу, что так много вершин не будет
    int index = 0;
    for(auto& a:vert_list){
	if(a->degree < current && a->degree != 0){
	    current = a->degree;
	    index = a->index;
	}
    }
    return  index;
}

// достать вершину по индексу O(n)- долго при большом количестве вершин, можно использовать другой контейнер
vertex_SPtr get_vertex_by_index(
    const int index,
    const plain_array<vertex_SPtr> vert_list){
    for(auto &a:vert_list)
	if(a->index == index)
	    return a;
    return nullptr;
}

// достать не помеченную вершину
vertex_SPtr get_not_mark_vertex(
    const plain_array<vertex_SPtr>& vert_list,
    const std::set<int>& pomech){

    for(const auto&a: vert_list)
	if(pomech.find(a->index) == pomech.end())
	    return a;
    return nullptr;
}


//сортировка массива с вершинами по степени, для более удобного перебора
plain_array<vertex_SPtr>
sort_vert_list_by_degree(plain_array<vertex_SPtr> vert_list)
    {
	std::sort(vert_list.begin(),vert_list.end(),
		  [&vert_list]
		  (vertex_SPtr i, vertex_SPtr j){
		      return i->degree < j->degree;
		  });
	return  vert_list;
}


//основной алгоритм
void cuthill_mckee_algo(plain_array<vertex_SPtr>& vert_list){

    // берем индекс минимальной вершины
    int current_ind = get_min_degree(vert_list);
    // кладем ее в набор помеченных вершин(мы там были)
    std::set<int> pomech ={current_ind};
    // очередь с вершинами, которые мы должны посетить
    std::queue<vertex_SPtr> inqueue;
    // достаем вершину с минимальной степенью
    auto current_vert = get_vertex_by_index(current_ind,vert_list);
    if(current_vert == nullptr){
	std::cerr <<"Bad vertex\n";
	return ;
    }
    // кладем ее в очередь
    inqueue.push(current_vert);
    // устанавлимаем ее индекс в 0;
    current_vert->new_index = 0;
    int i = 1;

    while(!inqueue.empty() )
    {
	//достаем вершину из очереди
	current_vert = inqueue.front();
        inqueue.pop();
	// сортируем список соседей в текущей вершине
        auto sort_vertex_list
	    = sort_vert_list_by_degree(current_vert->neigbords);
        for(auto& a: sort_vertex_list){ //идем по всем соседям
	    if(pomech.find(a->index) != pomech.end()){
		//пропускаем помеченные
		continue;
	    }else{
		// кладем в очередь не помеченные
		inqueue.push(a);
		pomech.insert(a->index);
		// увеличиваем текущий индекс
		a->new_index = i++;
	    }
	}
	// проверка при  несвязных подграфах в основном графе
	if(inqueue.empty()){
	    // достаем первую попавшуюся не помеченную вершину
	    auto temp = get_not_mark_vertex(vert_list, pomech);
	    if(temp){
		pomech.insert(temp->index);
		temp->new_index = i++;
		inqueue.push(temp);
	    }
	}
    }

    for(int i=0; i < vert_list.size(); i++){
	plain_array<int> new_neig; // создаем для каждой вершины новый список соседей
	for(auto &a:vert_list[i]->neig_index){
	    auto cur = get_vertex_by_index(a, vert_list);
	    if(cur)
		vert_list[i]->new_neig_index.push_back(cur->new_index);
	    else
		std::cerr <<" error in get vert\n";
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

void print_max_lenght(plain_array<vertex_SPtr>& array)
{
    for(auto& res_vert: array){
	std::cout<< "Result " <<res_vert->new_index << "  " << res_vert->new_neig_index << "\n ";
    }
}
void print_bad_lenght(plain_array<vertex_SPtr>& array)
{
    for(auto& res_vert: array){
	std::cout<< "Result " <<res_vert->index << "  " << res_vert->neig_index << "\n ";
    }
}

void main_algo(plain_array<plain_array<int>> matrix)
{
    prints(matrix); //вывод изначальной матрицы
    auto list_smej = create_list_smej(matrix); //создаю из нее список смежностей
    auto vertex_array = create_vertex_array(list_smej,matrix); // создаю из списка смежностей массив с вершинами
    print_bad_lenght(vertex_array);
    std::cout <<"SEPARATOR\n";
    cuthill_mckee_algo(vertex_array);
    auto new_matr = get_matrix(vertex_array);
    prints(new_matr); // вывожу результат
    print_max_lenght(vertex_array);
    matrix_is_symmetrix(matrix)? std::cout <<"Symetric" : std::cout <<"none"; std::cout<<'\n';
    auto [a,b,c] = get_another_matrix_wide(list_smej);
    std::cout <<"Ширина ленты искомой матрицы: " <<a <<"Вершины, которые образуют ширину "<< b <<" " <<c <<"\n";
    auto [d,e,f] = get_new_matrix_wide<int>(vertex_array);
    std::cout <<"Ширина ленты преобразованной матрицы: " <<d <<" Вершины, которые образуют ширину "<< e <<" " <<f <<"\n";
}

int main(int argc, char **argv)
{
    if(argc != 2){
	std::cerr<<" bad argc \n" << argc;
	return 0;
    }
    auto matr = fileIn<int>(argv[1]);
    if(!matrix_is_symmetrix(matr)){
	throw std::exception();
    }
    else
	std::cout <<"Symetric";
    std::cout <<matr.size() <<" " << matr[0].size() <<'\n';
    main_algo(matr);
    return 0;
}
