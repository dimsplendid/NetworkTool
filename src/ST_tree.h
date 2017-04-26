#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

// Link list to store the member of clusters
typedef struct ns{
    int data;
		double fdata;
    struct ns* next;
    struct ps* pre;
    // methods
		int (*len) (struct ns *);
		void (*print)(struct ns *);
    void (*del)(struct ns ** self,int data);
    void (*push)(struct ns * self,int data);
		void (*sort)(struct ns ** self,int option);
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
	int members[1];

	int (*size) ( tree * self);
  void (*norm)( tree * self);
  tree * (*find)(tree * self,double data);
  int (*cluster)(tree * self);
  void (*print)(tree * self);
	void (*free)(tree * self);
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
  link_lst * member;
	cluster * next;

	void (*push)(cluster * self, link_lst * new_cluster);
  void (*print)(cluster * self,int num);
  void (*free)(cluster * self);
};

cluster * cluster_init ();
