#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

// Link list to store the member of clusters
typedef struct ns{
    int data;
    struct ns* next;
} link_lst;

struct st_tree {
	int id;
	int rank;
	struct st_tree * par; // parent
	struct st_tree * l_tree;
	struct st_tree * r_tree;
	double max_flw;
	int members[100];
	link_lst * members_modified;
	int size;
};
typedef struct st_tree tree;
tree * insertnode();
void tree_printf(tree *);
int tree_cluster(tree *);

void tree_printf2file( const char * filename, tree * root);
