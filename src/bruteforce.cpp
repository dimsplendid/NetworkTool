#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "aux.h"
#include "generator.h"
#include "ST_tree.h"

#include "../lib/tm_usage.h"

void comb(int N,int K,vector<vector<int> >& group)
{
	group.clear();
	string bitmask(K,1); // K leading 1's
	bitmask.resize(N,0); // N-K traniling 0's

	// print int and permute bitmask
	do{
		vector<int> elem;
		for(int i = 0; i < N; i++) // int 0..N-1
		{
			if(bitmask[i])
			{
				cout << " " << i;
				elem.push_back(i);
			}
		}
		cout << endl;
		group.push_back(elem);
	} while(prev_permutation(bitmask.begin(),bitmask.end()));
}

double bruteforceST(Graph& input, Graph& output,int& s, int& t)
{
	int size = input.nodes.size();
	vector<vector<int> > combination;
	vector<int> result;
	double flow=DOUBLE_INF;//double limit
	double tmp_flw=0;
	double capacity;
	vector<int> group;
	// copy input to output
    for (int i = 0; i < input.nodes.size(); ++i)
    {
        for (int j = 0; j < input.nodes.size(); ++j)
        {
            capacity = input.getEdgeById(i,j)->_capacity;
            output.addEdge(i,j, capacity);
        }
    }
	output.sortEdge();
	output.build_network();
	vector<int>::iterator itI;
	for(int i = 1; i < size;i++)
	{
		comb(size,i,combination);
		for(int j = 0; j < combination.size(); j++)
		{
			tmp_flw = 0;
			vector<int> elem = combination[j];
			if(find(elem.begin(),elem.end(),s) != elem.end())
			{	
				if(find(elem.begin(),elem.end(),t) != elem.end())
				{}	// do nothing
				else
				{
					for(itI = elem.begin();itI != elem.end();itI++)
					{
						Node* node = output.getNodeById((*itI));
						for(int k = 0; k < node->next_nodes.size();k++)
						{
							int next_id = node->next_nodes[k];
							if (find(elem.begin(),elem.end(),next_id) != elem.end())
							{}	//do nothing
							else
							{
								Edge * edge = output.getEdgeById(node->id,next_id);
								tmp_flw += edge->_capacity;
							}
						}
					}
					cout << "tmp_flow: " << tmp_flw << endl;
					if (tmp_flw < flow)
					{
						flow = tmp_flw;
						result = elem;
					}
				}
			}
		}
	}
	cout << "By bruteforce methods, ";
	cout << "S cluster is : " << endl;
	for(int i = 0; i < result.size();i++)
		cout << " " << result[i];
	cout << endl;
	cout << "Flow: " << flow << endl;
	return flow;
}
