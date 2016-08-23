// 20150510
// minimum cut
// Tseng Wei-Hsung
//  

#include "cluster.h"
#include "../lib/tm_usage.h"

using namespace std;
int help_message(void);
int main(int argc, char * argv[])
{
		if ( argc == 1 || argc > 4 )
		{
				cout << "Number of argument is out of range!!" << endl;
				return help_message();
		}
		if (  strcmp(argv[1],"-h") == 0){ return help_message(); }

		// calc time and memory
		CommonNs::TmUsage tmusg;
		CommonNs::TmStat stat;

		// Read the file
		// Trasfer rate matrix to graph(dot format)
		if( ( strcmp(argv[1],"-f") == 0
								|| strcmp(argv[1],"-b") == 0
								|| strcmp(argv[1],"-g") == 0
								|| strcmp(argv[1],"-d") == 0 
								|| strcmp(argv[1],"-t") == 0)
						&& (argc == 3 || argc == 4)  ){
				if ( ( strcmp(argv[1],"-f") == 0
										|| strcmp(argv[1],"-b") == 0)
								&& argc == 3 ){
						string option = argv[1];
						string output_filename = argv[2];
						double max_flow;
						fstream fin(argv[2]);
						Graph input = Graph(argv[2]);

						option.erase(option.begin());
						output_filename += "_"+option;

						Graph output = Graph(output_filename.c_str());
						int level = 0;
						int size, source_id, target_id ;
						double cut_off;
						double capacity;

						cout << "--------Rate data input---------" << endl;
						fin >> size;
						fin >> source_id >> target_id ;
						// for the calculate process we start at 0
						source_id--;
						target_id--;
						fin >> cut_off;
						cout << "Network size: " << size << endl;
						string name[size];
						for(int i = 0; i < size; i++)
								fin >> name[i];
						cout << "Source node: " << name[source_id] << endl;
						cout << "Target node: " << name[target_id] << endl;
						cout << "Cut off value: " << cut_off << endl;
						/*
							 cout << "The list of node: " << endl;

							 for(int i = 0; i < size; i++)
							 cout << " " << name[i] ;
							 */
						cout << endl;
						// initialize the nodes
						for(int i = 0; i < size; i++)
						{
								input.addNode(i,name[i]);
								fin >> input.nodes[i]->site_energy;
								//	cout << "Creat Node: ";
								//	cout << input.getNodeById(i)->label << endl;
						}

						for (int i = 0; i < size; ++i){
								for (int j = 0; j < size; ++j){
										fin >> capacity;
										capacity = my_abs(capacity);
										capacity = ((capacity >= cut_off) && (i != j)) ? capacity:0; 
										input.addEdge(j,i,capacity);
								}
						}
						cout << "-----------Site energy---------" << endl;
						for(int i = 0; i < size; i++){
								cout << "     " << input.nodes[i]->label << "     |     ";

								cout << input.nodes[i]->site_energy << endl;
						}
						cout << endl;
						cout << "--------------------------------" << endl;
						cout << "------------Result--------------" << endl;
						input.build_network();
						input.outputFormatFile();
						// DFS algorithms
						if(strcmp(argv[1],"-d")==0)
						{
								DFS_alg(input,output);
						}
						// use for s t cut
						int s = source_id;
						int t = target_id;

						if(strcmp(argv[1],"-f")==0)
						{
								Graph s_group = Graph("s group");
								Graph t_group = Graph("t group");

								cout << "Ford Fulkerson method: " << endl;
								max_flow = FordFulkerson(input,output,s,t,s_group,t_group);
								output.outputFormatFile();
						}
                        
						if(strcmp(argv[1],"-b")==0){ exit(0);}
						/*
                        {	

								Graph s_group = Graph("s group");
								Graph t_group = Graph("t group");
								cout << "Brute force method: " << endl;
								max_flow = bruteforceST(input,output,s,t,s_group,t_group
												);
								cout << "Max flow(min cut flow) is " << max_flow << endl;
						}
                        */
				}
			//	else if(strcmp(argv[1],"-t")==0 && (argc == 4))
				else if(strcmp(argv[1],"-t")==0){
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
								//		capacity = -1*capacity;
										capacity = ((capacity >= cut_off) && ( i != j)) ? capacity:0;
										input.addEdge(j,i,capacity);
								}
						}
						input.outputFormatFile();
						cout << "Start to built st cut..." << endl;
                        input.sortNode();
						st_iteration(input);	

				}

				else if(strcmp(argv[1],"-g")==0 && (argc == 3||argc == 4))
				{
						// cout << " test fRand(0,1): " << fRand(0,1) << endl;
						cout << "Flow matrix generator..." << endl;
						string file_name = argv[2];
						file_name += ".data";
						cout << "file " << file_name << endl;
						if(argc == 4)
						{
								double s_flow = strtod(argv[3],NULL)*0.005+0.55;
								generator(6,file_name,s_flow);
						}
						else
								generator(6,file_name);
				}
				else
						return help_message();
		}
		else
				return help_message();
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
