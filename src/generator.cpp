#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "aux.h"
#include "generator.h"
#include "ST_tree.h"

#include "../lib/tm_usage.h"

double fRand(double fMin, double fMax)
{	
	srand(static_cast<unsigned>(rand()));
	double f = fMin + static_cast<double>(rand())/(static_cast<double>(RAND_MAX/(fMax-fMin))); 
	return f;
}
void generator(int size,string file_name,double s_flow)
{
	// s = 0 ,t = size-1
	double element;
	fstream fout;
	fout.open(file_name.c_str(),ios::out);
	cout << size << endl;
    fout << size << endl;
	for(int i = 0;i < size;i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(i == j || i == 0 || (j == 0 && i == size-1))
				element = 0.0;
			// s-t flow
			else if(j == 0)
				element = s_flow;
			else
			{
				if ( fRand(0.0,1.0) > 0.5)
					element = fRand(0.0,1.0);
				else
					element = 0;
			}
			cout << element << " ";
			fout << element << " ";
		}
		cout << endl;
		fout << endl;
	}
	fout.close();
}
