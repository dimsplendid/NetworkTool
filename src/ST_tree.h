#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

// Link list to store the member of clusters
typedef struct ns{
    int data;
    struct ns* next;
    struct ps* pre;
    // methods
    void (*del)(struct ns ** self,int data);
    void (*push)(struct ns * self,int data);
    void (*free)(struct ns * self);
} link_lst;
link_lst * init_lst(int);

typedef struct st_tree tree;
struct st_tree {
	int id;
	int rank;
	struct st_tree * par; // parent
	struct st_tree * l_tree;
	struct st_tree * r_tree;
	double max_flw;
	int members[100];
	int size;
  void (*norm)( struct st_tree * self);
  tree * (*find)(tree * self,double data);
  void (*print)(tree * self);
  // link_lst * (*sort_data) (tree * self);
};

tree * insertnode();
void tree_printf(tree *);
int tree_cluster(tree *);
void free_tree(tree *);

void tree_reconstruct(tree * root, int option);
void tree_printf2file( const char * filename, tree * root);

typedef struct cluster_struct cluster;
struct cluster_struct{
  link_lst * total;
  link_lst * member;

  void (*print)(cluster * self);
  void (*free)(cluster * self);
};

cluster * init_cluster ();
