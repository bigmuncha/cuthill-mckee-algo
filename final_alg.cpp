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

std::vector<std::shared_ptr<vertex>> create_vertex_map (std::vector<std::vector<int>> &list_smej)
{
    std::vector<std::shared_ptr<vertex>> map_vert;
    for(int i =0;i < list_smej.size(); i++)
    {
	vertex omar(list_smej[i].size(),std::vector<std::shared_ptr<vertex>>(),list_smej[i],i,0,std::vector<int>());
	map_vert.push_back(std::make_shared<vertex>(omar));
    }
    for(int i = 0; i< list_smej.size(); i++){
	map_vert[i]->neigbords = [&map_vert](std::vector<int> vect_ind) ->std::vector<std::shared_ptr<vertex>>{
		std::vector<std::shared_ptr<vertex>> ret;
		for(auto &a:vect_ind){ ret.push_back(map_vert[a]);}
		return ret;}(map_vert[i]->neig_index);
    }
    return map_vert;
}


class Graph {

public:
    Graph(std::vector<std::vector<int>> &list_smej)
    {
	    m_vertex_list = create_vertex_map(list_smej);
    }

    int get_degree()
    {
	int current = 1000;
	int index = 0;
	for(auto& a:m_vertex_list){
	    if(a->degree < current){
		current = a->degree;
		index = a->index;
	    }
	}
	return  index;
    }

    std::shared_ptr<vertex> get_vertex(int index,std::vector<std::shared_ptr<vertex>> vert_list){
	for(auto &a:vert_list)
	    if(a->index == index)
		return a;
	return nullptr;
    }

    std::vector<shared_ptr<vertex>> sort_vert(std::vector<shared_ptr<vertex>> vert_list){

	std::sort(vert_list.begin(),vert_list.end(),[&vert_list](shared_ptr<vertex> i, shared_ptr<vertex> j){return i->degree < j->degree;});

	return  vert_list;
    }

    void cuthill_mckee(){
	int current_ind = get_degree();
	std::cout <<"in cuthill mccke"<< current_ind <<"\n";
	std::set<int> pomech ={current_ind};
	std::queue<std::shared_ptr<vertex>> inqueue;
	auto current_vert = get_vertex(current_ind,m_vertex_list);
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
	    auto sort_vertex_list = sort_vert(current_vert->neigbords);
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
    }

    void  create_new_smej_list(std::vector<shared_ptr<vertex>> vert_list)
	{
	    for(int i=0; i < vert_list.size(); i++){
		std::vector<int> new_neig;
		for(auto &a:vert_list[i]->neig_index){
		    auto cur = get_vertex(a, vert_list);
		    vert_list[i]->new_neig_index.push_back(cur->new_index);
		}
	    }
	    for(auto&a: vert_list){
		std::cout <<"new ind " << a->new_index <<" new neigh " << a->new_neig_index <<'\n';
	    }
	}

    void result(){
//	printer();
	create_new_smej_list(m_vertex_list);

	// for(auto a:s){
	//     std::cout <<"index: " << a->index <<" neighbords: "<< a->neig_index <<'\n';
//	}
    }
    void printer()
	{
	    for(auto &a:m_vertex_list)
	    {
		std::cout <<"index: " << a->index <<" neighbords: "<< a->neig_index << " degree: " << a->degree <<" new index: " << a->new_index <<"\n";
	    }
	}

    std::vector<std::vector<int>> get_matrix(std::vector<std::shared_ptr<vertex>> vert_list)
	{
	    std::vector<std::vector<int> > ret_val(vert_list.size(),std::vector<int>(vert_list.size(),0));
	    for(auto&a:vert_list){
		for(auto&s:a->new_neig_index){
		    ret_val[a->new_index][s] = 1;
		}
	    }
	    return ret_val;
	}
    void print_new_matrix(){
	auto matr = get_matrix(m_vertex_list);
	prints(matr);
    }

private:
    std::vector<std::shared_ptr<vertex>> m_vertex_list;
    std::vector<std::pair<int,int>> degree_list;
};
std::vector<std::vector<int>> get_matrix(std::vector<std::shared_ptr<vertex>> vert_list)
{
    std::vector<std::vector<int> > ret_val(vert_list.size(),std::vector<int>(vert_list.size(),0));
    for(auto&a:vert_list){
	for(auto&s:a->new_neig_index){
	    ret_val[a->new_index][s] = 1;
	}
    }
    return ret_val;
}

int main()
{
    auto matr = fileIn<int>("input.txt");
    prints(matr);
    auto list_smej = create_list_smej(matr);
    auto map_smej  = create_map_smej(matr);

    Graph omar(list_smej);
    omar.printer();
    std::cout <<"\n";
    omar.cuthill_mckee();
    omar.printer();
    omar.result();
    omar.print_new_matrix();
    return 0;
}
