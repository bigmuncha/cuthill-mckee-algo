#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "helper.hpp"
#include <memory>
#include <algorithm>
#include <ranges>

class vertex
{
    public:
    int degree;
    std::vector<std::shared_ptr<vertex>> neigbords;
    std::vector<int> neig_index;
    unsigned index;
    unsigned new_index;
    std::vector<int> new_neig_index;
    vertex(int i, std::vector<std::shared_ptr<vertex>> n, std::vector<int> l, unsigned inddex,unsigned new_index,std::vector<int> new_neig)
	:degree(i),neigbords(n),neig_index(l),index(inddex),new_index(new_index),new_neig_index(new_neig) {}
};

typedef std::shared_ptr<vertex> vertex_sp;

std::vector<vertex_sp> create_vertex_array (const std::vector<std::vector<int>> &list_smej)
{
    std::vector<vertex_sp> map_vert;
    for(int i =0;i < list_smej.size(); i++)
    {
	vertex omar(list_smej[i].size(),std::vector<vertex_sp>(),list_smej[i],i,0,std::vector<int>());
	map_vert.push_back(std::make_shared<vertex>(omar));
    }
    for(int i = 0; i< list_smej.size(); i++){
	map_vert[i]->neigbords = [&map_vert](std::vector<int> vect_ind) ->std::vector<vertex_sp>{
		std::vector<vertex_sp> ret;
		for(auto &a:vect_ind){ ret.push_back(map_vert[a]);}
		return ret;}(map_vert[i]->neig_index);
    }
    return map_vert;
}

int get_min_degree(const std::vector<vertex_sp>& vert_list)
{
    int current = 1000;
    int index = 0;
    for(auto& a:vert_list){
	if(a->degree < current){
	    current = a->degree;
	    index = a->index;
	}
    }
    return  index;
}

vertex_sp get_vertex_by_index(const int index,const std::vector<vertex_sp> vert_list){
    for(auto &a:vert_list)
	if(a->index == index)
	    return a;
    return nullptr;
}

std::vector<vertex_sp> sort_vert_list_by_degree(std::vector<vertex_sp> vert_list){
	std::sort(vert_list.begin(),vert_list.end(),
		  [&vert_list]
		  (vertex_sp i, vertex_sp j){
		      return i->degree < j->degree;
		  });
	return  vert_list;
}

void cuthill_mckee_algo(std::vector<vertex_sp>& vert_list){
    int current_ind = get_min_degree(vert_list);
    std::cout <<"in cuthill mccke"<< current_ind <<"\n";
    std::set<int> pomech ={current_ind};
    std::queue<vertex_sp> inqueue;
    auto current_vert = get_vertex_by_index(current_ind,vert_list);
    if(current_vert == nullptr){
	std::cerr <<"Bad vertex\n";
	return ;
    }
    inqueue.push(current_vert);
    current_vert->new_index = 0;
    int i = 1;
    while(!inqueue.empty())
    {
	current_vert = inqueue.front();
	inqueue.pop();
	auto sort_vertex_list = sort_vert_list_by_degree(current_vert->neigbords);
	for(auto& a: sort_vertex_list){
	    if(pomech.contains(a->index)){
		continue;
	    }else{
		inqueue.push(a);
		pomech.insert(a->index);
		a->new_index = i++;
	    }
	}
    }

    for(int i=0; i < vert_list.size(); i++){
	std::vector<int> new_neig;
	for(auto &a:vert_list[i]->neig_index){
	    auto cur = get_vertex_by_index(a, vert_list);
	    vert_list[i]->new_neig_index.push_back(cur->new_index);
	}
    }
}

std::vector<std::vector<int>> get_matrix(const std::vector<vertex_sp>& vert_list)
{
    std::vector<std::vector<int> > ret_val(vert_list.size(),std::vector<int>(vert_list.size(),0));
    for(auto&a:vert_list){
	for(auto&s:a->new_neig_index){
	    ret_val[a->new_index][s] = 1;
	}
    }
    return ret_val;
}

void main_algo(std::vector<std::vector<int>> matrix)
{
    prints(matrix);
    auto list_smej = create_list_smej(matrix);
    auto vertex_array = create_vertex_array(list_smej);
    cuthill_mckee_algo(vertex_array);
    auto new_matr = get_matrix(vertex_array);
    prints(new_matr);
}



int main()
{
    auto matr = fileIn<int>("input.txt");
    main_algo(matr);
    return 0;
}
