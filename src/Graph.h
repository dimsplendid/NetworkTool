#ifndef GRAPH_H
#define GRAPH_H

//some c function, should be modified
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <limits>

//for rand function
#include <cstdlib>
#include <ctime>
#include "aux.h"

#include "generator.h"
#include "ST_tree.h"

#include "../lib/tm_usage.h"

#define DOUBLE_INF std::numeric_limits<double>::max()
#define DIS_INF 2147483647

using namespace std;
class Node{
	public:
		Node(const int& i);
		Node(string );
		int id;
		string label;

		int color;
		//use for dfs
		int d;
		int f;
		int site_energy;// cm^-1, used for pigments

		Node *prev;// use for BFS
		vector<int> pre_nodes;
		vector<int> next_nodes;
		vector<int> neighbors;
};

class Edge{
	public:
		Edge(Node *, Node *,double&);
		Node* s; // start node
		Node* t; // end node
		double _capacity;
		double _flow;
};

class Graph{

	public:
		Graph(const string& n);
		~Graph();
		int id;

		void addNode(const int& id,string name);
		void addEdge(const int& v1, const int& v2,double& flow);

		Node * getNodeById(const int& id);
		Edge * getEdgeById(const int&, const int&);

		//aux function
		void init();
		void outputFormatFile(string ="none");
		void sortEdge();
		void sortNode(); // sort node by site energy
		void cal_neighbors(int);
		void build_network();
		void read(char* );

		//use for PSII network
		void build_st(double&,double&);

		double cutoff;
		map<int, Node *> nodesMap;
		map<int,map<int,Edge *> > edgesMap;
    vector<Node *> nodes;
		vector<Edge *> edges;
		string _name;
};

class Tree{
public:
	Tree(int &);
	~Tree();
	vector<int> cluster;
	int level;
	Graph *  network;
	Tree * s_sub;
	Tree * t_sub;
	void build_tree();
};

#endif
