#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

struct st_tree {
	int id;
	int rank;
	struct st_tree * par; // parent
	struct st_tree * l_tree;
	struct st_tree * r_tree;
	double max_flw;
	int members[100];
	int size;
};
typedef struct st_tree tree;
tree * insertnode();
void tree_printf(tree *);
int tree_cluster(tree *);
