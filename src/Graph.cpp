// Tseng Wei-Hsiang
#include "Graph.h"
#include "tool.h"

Edge::Edge(Node *a, Node *b,double & capacity){
	// 	cout << "construct edge "<< a->id << " -> ";
	// 	cout << b->id << " ..." << endl;
	s = a;
	t = b;
	_capacity = capacity;
	_flow = 0;
}
Node::Node(string str){
	id = -1;
	color = -1;
	d = DIS_INF;
	prev = 0;
	label = str;
}
Node::Node(const int& i){
	// cout << "construct node "<< i ;
	// cout << " ..." << endl;
	id = i;
	color = -1;
	d = DIS_INF;
	prev = 0;
	label =  my_itos(i);
}

Graph::Graph(const string& n){
	// cout << "construct graph " << n << " ..." << endl;
	_name = n;
	cutoff = 1.0e-15; // 0 in double
}
void Graph::init(){
	id = 0;
	vector<Node*>::iterator itN;
	for ( itN = nodes.begin() ; itN != nodes.end() ; itN++ ){
		Node *node = (*itN);
		node->d = DIS_INF;
		node->prev = 0;
		node->color = -1;
	}
}
Graph::~Graph(){
	// cout << "destruct graph " << _name << " ..." << endl;
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ ){
		delete (*itE);
		(*itE) = 0;
	}

	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ ){
		delete (*itN).second;
		(*itN).second = 0;
	}

	vector<Node *>::iterator itN2;
	for ( itN2 = nodes.begin() ; itN2 != nodes.end() ; itN2++ ){
		(*itN2) = 0;
	}
}

void Graph::addNode(const int& id,string name){
	Node *n;
	map<int, Node *>::iterator it = nodesMap.find(id);
	if (it != nodesMap.end()){
		n = (*it).second;
		// cout << "E: This id is duplicate!!" << endl;
	}
	else{
		n = new Node(id);
		nodesMap[id] = n;
		nodes.push_back(n);
		n->label = name;
	}
}

void Graph::addEdge(const int& v1, const int& v2, double & capacity){
	Node *a, *b;
	map<int, Node *>::iterator it = nodesMap.find(v1);
	if ( it != nodesMap.end() )
		a = (*it).second;
	else{
		a = new Node(v1);
		nodesMap[v1] = a;
		nodes.push_back(a);
	}

	it = nodesMap.find(v2);
	if ( it != nodesMap.end() ){
		b = (*it).second;
	}
	else{
		b = new Node(v2);
		nodesMap[v2] = b;
		nodes.push_back(b);
	}
	if (capacity <= cutoff){
		capacity = 0;
	}
	Edge *e = new Edge(a, b, capacity);
	edges.push_back(e);
	edgesMap[v1][v2]=e;

}
void Graph::build_network(){
	// init
	vector<Node*>::iterator itN;
	for(itN = nodes.begin(); itN != nodes.end();itN++){
		Node * node = (*itN);
		node->pre_nodes.clear();
		node->next_nodes.clear();
	}
	// cal
	sortEdge();
	vector<Edge*>::iterator itE;
	for(itE = edges.begin();itE != edges.end();itE++){
		Node * s = (*itE)->s;
		Node * t = (*itE)->t;
		if((*itE)->_capacity > cutoff){
			s->next_nodes.push_back(t->id);
			t->pre_nodes.push_back(s->id);
		}
	}
}
void Graph::cal_neighbors(int option){
	// option 0 : s
	// optoon 1 : t
	if(option == 0){
		vector<Node*>::iterator itN;
		for(itN = nodes.begin();itN != nodes.end();itN++){
			Node * node = (*itN);
			node->neighbors = node->next_nodes;
		}
	}
	else if(option == 1){
		vector<Node*>::iterator itN;
		for(itN = nodes.begin();itN != nodes.end();itN++){
			Node * node = (*itN);
			node->neighbors = node->pre_nodes;
		}
	}
}

void Graph::build_st(double & s_capacity,double & t_capacity) {
	Node * s = new Node(-1);
	Node * t = new Node(-2);
	s->label = "s";
	t->label = "t";
	vector<Node*>::iterator itN;
	for(itN = nodes.begin();itN != nodes.end();itN++){
		Node * node = (*itN);
		Edge * edge_s = new Edge(s,node,s_capacity);
		Edge * edge_t = new Edge(node,t,t_capacity);
		edgesMap[s->id][node->id]=edge_s;
		edgesMap[node->id][t->id]=edge_t;
	}
	nodesMap[s->id] = s;
	nodesMap[t->id] = t;
	nodes.push_back(s);
	nodes.push_back(t);
}
// compare operator
struct myEdgeCmp{
	bool operator() (Edge* i, Edge* j)
	{	return (i->_capacity > j->_capacity);}
}myEdge; // sort edge by the rate
struct myNodeCmp{
	bool operator() (Node *i, Node *j)
	{	return (i->site_energy > j->site_energy);}
}myNode; // sort node by nodes' energy(exciton energy)

void sortEdge(vector<Edge*> & edges){
	sort(edges.begin(),edges.end(),myEdge);
}
void Graph::sortEdge(){
	sort(edges.begin(),edges.end(),myEdge);
}
void Graph::sortNode(){
	sort(nodes.begin(),nodes.end(),myNode);
}

Node * Graph::getNodeById(const int& id){
	return nodesMap[id];
}

Edge* Graph::getEdgeById(const int & i, const int & j){
	return edgesMap[i][j];
}

// output the format file
void Graph::outputFormatFile(string option){
	// option: 0: dfs; 1: bfs; 2: color
	// fstream fout;
	// int option = 1;
	// string tmp;
	// if (option == 0)
	// {
	//     tmp="dfs";
	// }
	// else if ( option == 1)
	// {
	//     tmp="bfs";
	// }
	// else if ( option == 2)
	// {
	//     tmp="color";
	// }
	/*
	   fout.open(filename,ios::out);
	   cout << "fout.is_open(): " << fout.is_open();
	   if (fout.is_open()){
	   fout << "//" << name << endl;
	   fout << "graph " << name << "_" << tmp << "{" << endl;
	   vector<Edge*>::iterator itN;
	   for ( itN = edges.begin() ; itN != edges.end() ; itN++ ){
	   Edge *edge = (*itN);
	   Node *u = edge->node[0];
	   Node *v = edge->node[1];
	   fout << "v" << u->id << " -- v" << v->id << ";" << endl;
	   }
	   fout << "}" << endl;
	   if (option==2)
	   {
	   map<int, Node *>::iterator itN;
	   for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	   {
	   Node *node = (*itN).second;
	   fout << "v" << node->id <<" [label = \"" << node->color ;
	   fout << "\"];"<<endl;
	   }
	   }
	   fout.close();
	   }*/
	cout << "//" << _name << endl;
	cout << "digraph " << _name << "{" << endl;
	vector<Edge*>::iterator itN;
	for ( itN = edges.begin() ; itN != edges.end() ; itN++ ){
		Edge *edge = (*itN);
		Node *u = edge->s;
		Node *v = edge->t;
		if ( edge->_capacity > cutoff) {
			// use for DFS search, need modifying...
			if (strcmp(option.c_str(),"DFS") == 0){
				cout << "v" << u->id << "_" << u->d << "/" << u->f;
				cout << " -> " ;
				cout << "v" << v->id << "_" << v->d << "/" << u->f;
			}
			// Actual work Now
			else
				cout << u->label << " -> " << v->label;
			cout << " [label =\" " << edge->_capacity << " \"]";
			cout << ";" << endl;
		}
	}
	cout << "}" << endl;
}
