#pragma once

#include <cstdio>
#include <cstdlib>
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

  tree * (*copy)(tree * self);

	int (*size) ( tree * self);
  void (*norm)( tree * self);
  tree * (*find)(tree * self,double data);
  int (*cluster)(tree * self,int option[3]);
  void (*print)(tree * self);
	void (*free)(tree * self);
  // link_lst * (*sort_data) (tree * self);
};

tree * insertnode();
void tree_printf(tree *);

void tree_reconstruct(tree * root, int option);
void tree_printf2file( const char * filename, tree * root);

typedef struct cluster_struct cluster;
struct cluster_struct{
  link_lst * member;
	cluster * next;

  int (*len)(cluster * self);
  void (*del)(cluster * self, link_lst ** data);
	void (*push)(cluster * self, link_lst * new_cluster);
  void (*print)(cluster * self);
  void (*free)(cluster * self);
};

cluster * cluster_init ();
