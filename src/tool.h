#ifndef _TOOL_H_
#define _TOOL_H_
#include "aux.h"
#include "Graph.h"
#include "ST_tree.h"

bool findPath(Graph& graph,vector<int> & path,int &,int&);
double FordFulkerson(Graph&, Graph&,int &, int &,Graph &, Graph &);
void FF_output(Graph &, Graph &, double,int &, int &, Graph & , Graph &);
void sortEdge(vector<Edge*> &);
void build_tree(Graph &,int);
double bruteforceST(Graph &, Graph &, int&,int& );
vector<int> BFS_alg(Graph &, int &);
void DFS_alg(Graph&,Graph&);
void DFS_visit(Node*,Graph&,int &);
int st_iteration(Graph &,tree * );
int st_iteration_modified(Graph &, double);
void tree_out(Graph & input, Graph & s, Graph & t,double flow);

#endif
