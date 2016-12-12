// 2016 09 15
// Tseng Wei-Hsung
// use git to make log
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "tool.h"
#include "generator.h"
#include "ST_tree.h"

#include "../lib/tm_usage.h"

using namespace std;
int system(const char *command);
int help_message(void);
int main(int argc, char * argv[]){

	// set time parameter
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat stat;

	if ( argc == 1 || argc > 4 ){
		cout << "Number of argument is out of range!!" << endl;
		return help_message();
	}
	// time
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	int size;
	double cut_off,capacity;
	fstream fin(argv[2]);
	fin >> size >> cut_off;
	string name[size];
	double site_energy[size];
	Graph input = Graph(argv[3]);
	cout << "size: " << size << endl;
	cout << "cut off: " << cut_off << endl;
	for(int i = 0; i < size; i++) {
		fin >> name[i];
		cout << name[i] << "\t";
	}
	cout  << endl;
	for(int i = 0; i < size; i++) {
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
		system("mv Tree.dot Old_tree.dot");
		system("mv Tree.png Old_tree.png");
		fstream tree_out, tree_nrm_out; // log file
		string tree_log_name = "Tree.dot";
		//string tree_nrm_name = "Tree_nrm.dot";
		tree_out.open(tree_log_name.c_str(),ios::app);
		//tree_nrm_out.open(tree_nrm_name.c_str(),ios::app);
		tree_out << "digraph Network{" << endl;
		tree_out << "edge [comment=\"Wildcard node added automatic in EG.\"];"<< endl;
    tree_out << "node [comment=\"Wildcard node added automatic in EG.\", "<< endl;
    tree_out << "      fontname=\"sans-serif\"," << endl;
    tree_out << "      fontsize=\"16\", " << endl;
    tree_out << "      penwidth=\"1.5\"];" << endl;
		//tree_nrm_out << "digraph Network{" << endl;
		tree_out.close();
		//tree_nrm_out.close();

		cout << "Start to built st cut..." << endl;

		input.id = 0;
		st_iteration(input);

		tree_out.open(tree_log_name.c_str(),ios::app);
		//tree_nrm_out.open(tree_nrm_name.c_str(),ios::app);
		tree_out << "}" << endl;
		//tree_nrm_out << "}" << endl;
		tree_out.close();
		//tree_nrm_out.close();
		system("dot -Tpng Tree.dot -o Tree.png");
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
	else if ((strcmp(argv[1],"--plot-network") == 0)) {
		cout << " input the cut off you want ";
		cout << "[default = cut off in input file]:" << endl;
		if (cin.peek() != '\n') { // check if next character is newline
			cin >> cut_off;
		}
		input.sortNode();
		fstream fout;
		fout.open("graph.dot",ios::out);
		// cout << "fout.is_open(): " << fout.is_open(); // print the file check
		if (fout.is_open()){
			fout << "//" << "Graph by dimsplendid" << endl;
			fout << "//" << tm.tm_year + 1900 << tm.tm_mon + 1 << \
			tm.tm_mday << tm.tm_hour << tm.tm_min << endl;
			fout << "digraph " << "Network" << "{" << endl;
			vector<Edge*>::iterator itN;
			vector<Edge *> edges = input.edges;
			for ( itN = edges.begin() ; itN != edges.end() ; itN++ ){
				Edge *edge = (*itN);
				Node *u = edge->s;
				Node *v = edge->t;
				if( edge->_capacity > cut_off) {
					fout << "\"" << u->label << "\" -> \"" << v->label << "\"";
					fout << "[ label = " << edge->_capacity << " ]" << endl;
				}
			}
			fout << "}" << endl;
			fout.close();
		}
		system("dot -Tpng graph.dot -o graph.png");
	}
	//calc time and memory
	tmusg.getPeriodUsage(stat);
	cout << "The total CPU time: " << (stat.uTime + stat.sTime)/1000.0;
	cout << "ms" << endl;
	cout << "memory: " << stat.vmPeak << "KB" << endl; // print peak memory
	return 0;
}

int help_message() {
	system("cat doc/usage");
	return 0;
}
