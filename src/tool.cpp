// network analysis tool
#include "tool.h"
int tree_id = 0;
struct descending_cmp{
	bool operator() (int i,int j) {return (i>j);}
} descending;
vector<int> BFS_alg(Graph & graph,int & s_id){
	graph.init();
	Node * s_node = graph.getNodeById(s_id);
	s_node->color=0;
	s_node->d=0;
	s_node->prev=0;
	queue<int> bfs_q;
	bfs_q.push(s_id);
	// irratation
	while(!bfs_q.empty()){
		//DEQUEUE(bfs_q)
		int u_id = bfs_q.front();
		bfs_q.pop();

		Node * u = graph.getNodeById(u_id);
		vector<int> u_adj = u->neighbors;
		for (int i = 0; i < u_adj.size(); ++i){
			Node * v = graph.getNodeById(u_adj[i]);
			if (v->color == -1){
				v->color = 0;
				v->d = u->d + 1;
				v->prev=u;
				bfs_q.push(v->id);
			}
		}
		u->color=1;
	}
	//result
	vector<int> cluster;
	map<int, Node *>::iterator itN;
	for ( itN = graph.nodesMap.begin() ; itN != graph.nodesMap.end() ; itN++ ){
		Node *u = (*itN).second;
		if (u->prev !=0){
			cluster.push_back(u->id);
		}
	}
	return cluster;
}

void DFS_alg(Graph& input, Graph& output){
	map<int, Node *>::iterator itN;
	int init_time = 0;
	int & time = init_time; // DFS search time step
	// Copy graph to result
	double capacity;
	for (int i = 0; i < input.nodes.size(); ++i){
		for (int j = 0; j < input.nodes.size(); ++j){
			capacity = input.getEdgeById(i,j)->_capacity;
			output.addEdge(i,j, capacity);
		}
	}
	// initialize
	output.build_network();
	for(itN = output.nodesMap.begin();itN != output.nodesMap.end();itN++){
		Node *u = (*itN).second;
		u->color = -1;
		u->prev = 0;
	}
	// irratation
	for ( itN = output.nodesMap.begin() ; itN != output.nodesMap.end() ; itN++ ){
		Node *u = (*itN).second;
		if (u->color == -1){
			DFS_visit(u,output,time);
			cout << "u " << u->id << ": color_" << u->color <<endl;
		}
	}
	//result
	cout << "// The init Graph(that show the scan time): " << endl;
	for( itN = output.nodesMap.begin(); itN != output.nodesMap.end(); itN++){
		Node *u = (*itN).second;
		Node *input_node = input.getNodeById(u->id);
		input_node->d = u->d;
		input_node->f = u->f;
	}
	input.outputFormatFile("DFS");
	cout << "// DFS tree" << endl;
	output.outputFormatFile();
}

void DFS_visit(Node * u,Graph& graph, int & time){
	time++;
	u->d = time;
	u->color = 0;
	vector<int> u_adj = u->next_nodes;
	for (int i = 0; i < u_adj.size(); ++i){
		Node * v = graph.getNodeById(u_adj[i]);
		if (v->color == -1){
			v->prev=u;
			DFS_visit(v,graph,time);
		}
	}
	u->color = 1;
	time++;
	u->f = time;
}

double FordFulkerson(Graph & input, Graph & output,int & s, int & t,\
	 										Graph & s_group, Graph & t_group){
	// cout << "copy input to output..." << endl;
	double capacity;
	double flow = 0.0;
	// copy input to output
	vector<Node*>::iterator itN1,itN2;
	for (itN1 = input.nodes.begin(); itN1 != input.nodes.end(); itN1++) {
		for (itN2 = input.nodes.begin(); itN2 != input.nodes.end(); itN2++) {
			Node * u = (*itN1);
			Node * v = (*itN2);
			// cout << "copy edge(" << u->label << "," << v->label << ")";
			capacity = input.getEdgeById(u->id,v->id)->_capacity;
			// cout << "  |  capacity: " << capacity << endl;
			output.addEdge(u->id,v->id, capacity);
		}
	}
	for (itN1 = output.nodes.begin();itN1 != output.nodes.end();itN1++){
		(*itN1)->label = input.getNodeById((*itN1)->id)->label;
	}
	output.sortEdge();
	cout << "output: " << endl;
	output.outputFormatFile();
	// initail all flow to 0
	vector<Edge*>::iterator itE;
	for(itE = output.edges.begin();itE != output.edges.end();itE++){
		Edge * edge = (*itE);
		edge->_flow = 0;
	}

	// calculate
	// cout << "start to FF alg" << endl;
	vector<int> path;
	while(findPath(output,path,s,t)){
		// cout << "In FF alg while loop..." << endl;
		vector<Edge*> edge_in_path;
		double c_f;
		// Show the path information
		cout << "Print PATH: ";
		for(int i = 0;i < path.size() ; i++){
			Node * node = output.getNodeById(path[i]);
			cout << node->label << " ";
		}
		cout << endl;

		for(int i = 0;i < path.size()-1;i++){
			Edge * edge = output.getEdgeById(path[i],path[i+1]);
			edge_in_path.push_back(edge);
		}
		sortEdge(edge_in_path);
		c_f = edge_in_path.back()->_capacity;
		vector<Edge*>::iterator itE;
		for(itE=edge_in_path.begin();itE != edge_in_path.end();itE++){
			Edge * u_v = (*itE);
			Edge * v_u = output.getEdgeById(u_v->t->id,u_v->s->id);
			u_v->_capacity -= c_f;
			v_u->_capacity += c_f;
			u_v->_flow += c_f;
		}
		cout << endl;
		// cout << "flow: " << c_f << endl;
		flow += c_f;
	}
	cout << "End FF alg..." << endl;
	return flow;
}

void FF_output(Graph & input, Graph & output,double flow, int & s, int & t,\
	 										Graph & s_group, Graph & t_group){
	// print the log of FordFulkerson alg
	// And generate the sub graph s and t
	fstream fout;
	string filename;
	filename = input._name;
	filename += "_FF";
	std::vector<int> tmp_container;

	fout.open(filename.c_str(),ios::out);

  output.cal_neighbors(0);
  vector<int> s_cluster=BFS_alg(output,s);
  sort(s_cluster.begin(),s_cluster.end(),descending);
	Node * sorce = output.getNodeById(s);
	fout << "By Ford Fulkerson algorithms, S cluster is: " << sorce->label;
	fout << " " ;
  for(int i = 0; i < s_cluster.size();i++) {
		Node * node = output.getNodeById(s_cluster[i]);
		fout << node->label << " ";
	}
	s_cluster.push_back(sorce->id);

	for(int i = 0; i < s_cluster.size();i++){
		Node * u = input.getNodeById(s_cluster[i]);
		for(int j = 0; j < s_cluster.size();j++){
			Node * v = input.getNodeById(s_cluster[j]);
			Edge * e = input.getEdgeById(u->id,v->id);
			s_group.addEdge(u->id,v->id,e->_capacity);
		}
	}
	vector<Node*>::iterator itN;
	for(itN = s_group.nodes.begin();itN != s_group.nodes.end();itN++){
		Node * n = (*itN);
		n->label = input.getNodeById(n->id)->label;
		n->site_energy = input.getNodeById(n->id)->site_energy;
	}
	s_group.outputFormatFile();
	s_group.sortNode();
  fout << endl;
	fout << endl;
  output.cal_neighbors(1);
  vector<int> t_cluster=BFS_alg(output,t);
  sort(t_cluster.begin(),t_cluster.end(),descending);
	Node * sink = output.getNodeById(t);
	fout << "By Ford Fulkerson algorithms, T cluster is: " << sink->label;
	fout << " " ;

  for(int i = 0; i < t_cluster.size();i++){
		Node * node = output.getNodeById(t_cluster[i]);
		fout << node->label << " ";
	}
  fout << endl;
	t_cluster.push_back(sink->id);
	for(int i = 0; i < t_cluster.size();i++){
		Node * u = input.getNodeById(t_cluster[i]);
		for(int j = 0; j < t_cluster.size();j++){
			Node * v = input.getNodeById(t_cluster[j]);
			Edge * e = input.getEdgeById(u->id,v->id);
			t_group.addEdge(u->id,v->id,e->_capacity);
		}
	}
	for(itN = t_group.nodes.begin();itN != t_group.nodes.end();itN++){
		Node * n = (*itN);
		n->label = input.getNodeById(n->id)->label;
		n->site_energy = input.getNodeById(n->id)->site_energy;
	}
	t_group.outputFormatFile();
	t_group.sortNode();

	fout << "Sink flow (Max Flow) is " << flow << endl;
	fout.close();
}

bool findPath(Graph& graph,vector<int> & path,int& s,int& t){
//	cout <<"check s and t ..." << endl;
//	cout <<"s id: " << s << ", t id: " << t << endl;
	bool is_path = true;
	bool loop = true;// use for whlie loop
	path.clear();
	int pre = s;
//	cout << "findPath init..." << endl;
	//init
	vector<Node*>::iterator itN;
	for(itN = graph.nodes.begin();itN != graph.nodes.end();itN++){
		Node * node = (*itN);
		node->color = -1;
	}

	// go throw until t
//	cout << "finding ..." << endl;
	graph.build_network();
	while(loop){
		Node * pre_node = graph.getNodeById(pre);
		if(pre_node->color == -1)
			path.push_back(pre);
		pre_node->color = 0;//gray
//		cout << "pre node: " << pre_node->label << endl;;
		// build OutLinks
		vector<Edge*> OutLinks;
		for (int i = 0; i < pre_node->next_nodes.size(); ++i){
			Node * next = graph.getNodeById(pre_node->next_nodes[i]);
			if (next->color == -1){
				Edge * OutLink = graph.getEdgeById(pre,next->id);
				OutLinks.push_back(OutLink);
			}
		}
//		cout << "sorting edge..." << endl;
		sortEdge(OutLinks);
//		cout << "out link nodes sort by edge capacity: " << endl;
//		for(int i = 0; i < OutLinks.size();i++)
//			cout << OutLinks[i]->t->label << " ";
//		if (OutLinks.empty())
//		{
//			cout << "There are no next nodes from node " << pre;
//			cout << endl << ",we should go back to previous node...";
//		}
//		cout << endl;
		if (OutLinks.empty()){
//			cout << "death way..." << endl;
//			cout << "path.back() = " << path.back() << endl;
			if (path.back() == t){
				loop = false;
//				cout << "Oh, sorry. We have got the end." << endl;
//				cout << "We have get a path!!" << endl;
			}
			else{
				if(path.back() == s){
					loop = false;
					is_path = false;
//					cout << "The end is comming XD" << endl;
				}
				else{
//					cout << "test path(before pop) : "<< endl;
//					for(int i = 0;i < path.size();i++){
//						cout << path[i] << " ";
//					}
//					cout << endl;
					path.pop_back();
//					cout << "test path(after pop) : "<< endl;
//					for(int i = 0;i < path.size();i++){
//						cout << path[i] << " ";
//					}
//					cout << endl;
					pre = path.back();
					is_path = true;
//					  cout << "The previous node is: " << pre << endl;
				}
			}
		}
		else
			pre = OutLinks[0]->t->id;
	}
//	cout << "end find path" << endl;;
	return is_path;
}


int st_iteration(Graph & input, tree * root){
	double pre_flow = 0.0;
	double flow = 0.0;

	cout << "st iterate [ "<< root->id << "] start..." << endl;
	for (int i = 0; i < input.nodes.size(); i++) {
		root->members[i]=input.nodes.at(i)->id;
	}
	root->size=input.nodes.size();
	if (input.nodes.size() > 1){
		input.sortNode();

		Node * s = (*input.nodes.begin());
		Node * t = (*(input.nodes.end()-1));
		cout << "           Name	 site energy " << endl;
		cout << "Source: " << s->label << " " << s->site_energy << endl;
		cout << "Target: " << t->label << " " << t->site_energy << endl;
		string t_name, s_name, output_name;
		printf("problem?\n");

		t_name = input._name + "t";
		s_name = input._name + "s";
		output_name = input._name + ".FFlog";

		Graph output =	Graph(output_name.c_str());
		Graph s_group = Graph(s_name.c_str());
		Graph t_group = Graph(t_name.c_str());
		flow = FordFulkerson(input,output,s->id,t->id,s_group,t_group);

		cout << "Flow = " << flow << endl;
		FF_output(input, output, flow,s->id, t->id, s_group, t_group);
		tree_id++;
		s_group.id =  tree_id;
		tree_id++;
		t_group.id = tree_id;
		tree_out(input,s_group,t_group,flow);
		root->max_flw = flow;
		tree * lt = insertnode();
		tree * rt = insertnode();
		root->l_tree = lt;
	  root->r_tree = rt;
	  lt->par = root;
		lt->rank = root->rank + 1;
		lt->id = s_group.id;
	  rt->par = root;
		rt->rank = root->rank + 1;
		rt->id = t_group.id;

		st_iteration(s_group,lt);
		st_iteration(t_group,rt);
	}
	else{
		Node * leaf = (*input.nodes.begin());
		cout << "This is leaf: " << leaf->label << endl;
		cout << "Congrajulation!" << endl;
	}

	return 0;
}
void tree_out(Graph & root, Graph & s_group, Graph & t_group, double flow){
  fstream tout;
	// flow = flow/(double)s_group.nodes.size()/(double)t_group.nodes.size();
	string root_name, s_name, t_name;
  tout.open("Tree.dot",ios::app);
	if(root.nodes.size() > 3){
		root_name = "C";
		root_name.append(my_itos(root.id));
		tout << "\" " << root_name << "\"";
		tout << "[comment=\"" ;
		for (int i = 0; i < root.nodes.size(); i++) {
			tout << root.nodes[i]->label << " ";
		}
		tout << "\"];" << endl;
	}
	else{
		for (int i = 0; i < root.nodes.size(); i++) {
			root_name.append(root.nodes[i]->label);
			root_name.append(" ");
		}
	}
	if(s_group.nodes.size() > 3){
		s_name = "C";
		s_name.append(my_itos(s_group.id));
	}
	else{
		for (int i = 0; i < s_group.nodes.size(); i++) {
			s_name.append(s_group.nodes[i]->label);
			s_name.append(" ");
		}
	}
	if(t_group.nodes.size() > 3){
		t_name = "C";
		t_name.append(my_itos(t_group.id));
	}
	else{
		for (int i = 0; i < t_group.nodes.size(); i++) {
			t_name.append(t_group.nodes[i]->label);
			t_name.append(" ");
		}
	}

	// graphviz style output
	tout << "\" " << root_name << "\"" << " -> " ;
	tout << "\" " << s_name;
	tout << "\" " << "[dir=\"none\", style=\"dashed\"];" << endl;
	tout << "\" " << root_name << "\"" << " -> " ;
	tout << "\" " << t_name;
	tout << "\" " << "[dir=\"none\", style=\"dashed\"];" << endl;
	tout << "\" " << s_name << "\"";
	tout << " -> " ;
	tout << "\" " << t_name << "\"";
	tout << "[constraint=\"false\", style=\"bold\", label=\"" << flow << "\"];";
	tout << endl;

	tout.close();
}

int st_iteration_modified(Graph & input, double pre_flow){
	double flow = 0;
	int i = 1;
	// First Cut
	if (input.nodes.size() > 1){
		input.sortNode();
		Node * s = (*input.nodes.begin());
		Node * t = (*(input.nodes.end()-i));
		cout << "		  Name	 site energy " << endl;
		cout << "Source: " << s->label << " " << s->site_energy << endl;
		cout << "Target: " << t->label << " " << t->site_energy << endl;
		string t_name, s_name, output_name;

		t_name = input._name + "_t";
		s_name = input._name + "_s";
		output_name = input._name + "_FF";

		Graph output =	Graph(output_name.c_str());
		Graph s_group = Graph(s_name.c_str());
		Graph t_group = Graph(t_name.c_str());

		flow = FordFulkerson(input,output,s->id,t->id,s_group,t_group);
		cout << "pre_flow = " << pre_flow << "\n flow = " << flow << " \n ";

		while (flow < pre_flow && (t->id != s->id) ){
			i++;
			Node * t = (*(input.nodes.end()-i));
			cout << "change target : " << t->label << "\n" ;
			flow = FordFulkerson(input,output,s->id,t->id,s_group,t_group);
			cout << "pre_flow = " << pre_flow << "\n flow = " << flow << " \n ";
		}
		pre_flow = flow;

		cout << "Flow = " << flow << endl;
		FF_output(input, output, flow, s->id, t->id, s_group, t_group);
		st_iteration_modified(s_group,pre_flow);
		st_iteration_modified(t_group,pre_flow);
	}
	else{
		Node * leaf = (*input.nodes.begin());
		cout << "This is leaf: " << leaf->label << endl;
		cout << "Congrajulation!" << endl;
	}
	return 0;
}
