// 2016 01 05
// version 1.01
// simplier code and add some funcion
// - can set S and T directily
//
// 20150510
// minimum cut
// Tseng Wei-Hsung
//  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "aux.h"
#include "generator.h"
#include "ST_tree.h"

#include "../lib/tm_usage.h"

using namespace std;
int help_message(void);
int main(int argc, char * argv[]){

	// set time parameter
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;

	if ( argc == 1 || argc > 4 ){
		cout << "Number of argument is out of range!!" << endl;
		return help_message();
	}
	int size;
	double cut_off,capacity;
	fstream fin(argv[2]);
	fin >> size >> cut_off;
	string name[size];
	int site_energy[size];
	Graph input = Graph(argv[3]);
	cout << "size: " << size << endl;
	cout << "cut off: " << cut_off << endl;
	for(int i = 0; i < size; i++)
	{
		fin >> name[i];
		cout << name[i] << "\t";
	}
	cout  << endl;
	for(int i = 0; i < size; i++)
	{
		fin >> site_energy[i];
		input.addNode(i,name[i]);
		input.nodes[i]->site_energy = site_energy[i];
		cout << site_energy[i] << "\t";
	}
	cout << endl;

	// build graph
	for(int i = 0; i < size; i++)	{
		for ( int j = 0; j < size; j++){
			fin >> capacity;
//			capacity = -1*capacity;
			capacity = ((capacity >= cut_off) && ( i != j)) ? capacity:0;
			input.addEdge(j,i,capacity);
		}
	}
	input.outputFormatFile();
	if((strcmp(argv[1],"-t") == 0)){
		cout << "Start to built st cut..." << endl;
		input.sortNode();
		st_iteration(input);
	}
	else if((strcmp(argv[1],"-f") == 0)){
		int S,T;
		double Flow;
		const string output_n = "output";
		const string s_group_n= "S group";
		const string t_group_n= "T group";
		Graph output = Graph(output_n);
		Graph s_group= Graph(s_group_n);
		Graph t_group= Graph(t_group_n);
		cout << "Set source: " << endl;
		cin >> S;
		cout << "Set target: " << endl;
		cin >> T;
		Flow = FordFulkerson(input,output,S,T,s_group,t_group);
		cout << "Flow = " << Flow << endl;
		FF_output(input, output, Flow,S,T, s_group, t_group);
	}
	else if((strcmp(argv[1],"-b") == 0)){
		input.sortNode();
		Node * s = (*input.nodes.begin());
		Node * t = (*(input.nodes.end()-1));
		cout << "         Name   site energy " << endl;
		cout << "Source: " << s->label << " " << s->site_energy << endl;
		cout << "Target: " << t->label << " " << t->site_energy << endl;
		Graph output = Graph(argv[3]);
		cout << "Use brute force algorithms: run out all of the combination = 2^N \n";
		bruteforceST(input, output, s->id, t->id);
	}
	//calc time and memory
	tmusg.getPeriodUsage(stat);
	cout << "The total CPU time: " << (stat.uTime + stat.sTime)/1000.0;
	cout << "ms" << endl;
	cout << "memory: " << stat.vmPeak << "KB" << endl; // print peak memory
	return 0;   
}

int help_message()
{
	cout << "NAME " << endl;
	cout << "       MinCut - tool to cut the graph" << endl;
	cout << "SYNOPSIS" << endl;
	cout << "       ./bin/MinCut [-g file] [-f|-b|-d file] [-h]" << endl;
	cout << "DESCRIPTION " << endl;
	cout << "       MinCut is a tool to analysis graph, and especially"<< endl;
	cout << "       for photosynthesis system." << endl;
	cout << "OPTIONS " << endl;
	cout << "       -g file [s flow], --generator" << endl;
	cout << "           Generating a matrix name <file name>.data, which " << endl;
	cout << "           has s flow=[s flow], by default s flow is 2.5." << endl;        
	cout << endl;
	cout << "       -f file, --FordFulkerson " << endl;
	cout << "           Use the Ford Fulkerson algorithms to solve the min" << endl;
	cout << "           -cut problem. This function would produce 2 file, " << endl;
	cout << "           file.log and file.out." << endl;
	cout << endl;
	cout << "       -b file, --BruteForce " << endl;
	cout << "           Use the brute force ethod to check the s-t cut of" << endl;
	cout << "           the graph. Because the minimum cut is the NPC " << endl;
	cout << "           problem, this method can't check the large system" << endl;
	cout << "           It's use to check the smaller system(maybe < 10)." << endl;
	cout << endl;
	cout << "       -d file, --DFS " << endl;
	cout << "           Use the Depth-first Search to build the DFS forest." <<endl;
	cout << endl;
	cout << "       -t file, --BinaryTree " << endl;
	cout << "           Use Ford Fulkerson algorithms to build the Tree. " << endl;
	cout << endl;
	cout << "       -h, --help" << endl;
	cout << "           Print a help message and exit" << endl;
	return 0;
}
