#ifndef _AUX_H_
#define _AUX_H_
using namespace std;
double my_abs(double);
int my_stoi(string);
string my_itos(const int &i);
bool findPath(Graph& graph,vector<int> & path,int &,int&);
double FordFulkerson(Graph&, Graph&,int &, int &,Graph &, Graph &);
void FF_output(Graph &, Graph &, double,int &, int &, Graph & , Graph &);
void sortEdge(vector<Edge*> &);
void build_tree(Graph &,int);
void comb(int,int,vector<vector<int> >&);
double bruteforceST(Graph &, Graph &, int&,int& );
vector<int> BFS_alg(Graph &, int &);
void DFS_alg(Graph&,Graph&);
void DFS_visit(Node*,Graph&,int &);
int st_iteration(Graph &);
int st_iteration_modified(Graph &, double);
#endif
