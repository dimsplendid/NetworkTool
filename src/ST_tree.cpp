#include "ST_tree.h"
#include <math.h>
#define EPSILON 0.001

void tree_get_member(tree * root, link_lst * members);
void tree_printf(tree * root);
void free_tree(tree * root);

void link_lst_printf (link_lst * lst);

static link_lst * merge(link_lst * l1, link_lst * l2){
	link_lst * tmp = init_lst(0);
	link_lst * q = tmp;
	while( l1 && l2){
		if ( l1->fdata < l2->fdata ){
			tmp->next = l1;
			tmp = tmp->next;
			l1 = l1->next;
		}
		else{
			tmp->next = l2;
			tmp = tmp->next;
			l2 = l2->next;
		}
	}
	
	if(l1){tmp->next = l1;}
	if(l2){tmp->next = l2;}

	link_lst * head = q->next;
	q->next = NULL;
	q->free(q);
	return head;
}
static link_lst * merge_sort_list(link_lst * head){
	// only one node
	if (!head || !head->next){return head;}

	link_lst * slow = head;
	link_lst * fast = head->next;

	// split list
	while(fast && fast->next){
		slow = slow->next;
		fast = fast->next->next;
	}
	fast = slow->next;
	slow->next = NULL;

	// sort each list
	link_lst * l1 = merge_sort_list(head);
	link_lst * l2 = merge_sort_list(fast);

	// merge l1 and l2
	return merge(l1,l2);
}

static void lst_float_print_impl(link_lst * self){
	link_lst * n = self;
  printf("data: ");
  while (n != NULL) {
    printf("%f ", n->fdata);
    n = n->next;
  }
  printf("\n");

}

static void lst_sort_impl(link_lst ** self,int option){
	(*self) = merge_sort_list((*self));
	lst_float_print_impl((*self));
}


static void lst_del_impl(link_lst ** self,int data){
  link_lst * curr = (*self),
           * prev = NULL;

  while (curr != NULL && curr->data != data) {
    prev = curr;
    curr = curr->next;
  }

  if(curr == 0){
    // printf("no %d in the list\n", data);
  }
  else if (curr == (*self)) {
    (*self) = (*self)->next;
    free(curr);
    curr = NULL;
  }
  else {
    prev->next = curr->next;
    free(curr);
    curr = NULL;
  }
}
static void lst_push_impl(link_lst * self,int data){
  // the same with add_member()
  // need to removing the redundant once
  link_lst * curr = self;
  while (curr->next != NULL){
    curr = curr->next;
  }
  link_lst * new_member = init_lst(data);
  curr->next = new_member;
}

static void lst_free_impl(link_lst * self) {
    if (self->next != NULL) {
        lst_free_impl(self->next);
    }
    free(self);
}
static int lst_len_impl(link_lst *self){
	link_lst * curr = self;
	int len = 1;
	while(curr->next != NULL){
		curr = curr->next;
		len++;
	}
	return len;
}

link_lst * init_lst(int data){
	// dynamic allocate memory

	link_lst * n = (link_lst*)malloc(sizeof(link_lst));
	n->data = data;
	n->fdata = 0.0;
  n->next = NULL;
	n->len = lst_len_impl;
  n->del = lst_del_impl;
  n->push = lst_push_impl;
  n->free = lst_free_impl;
	n->print = link_lst_printf;
	n->sort = lst_sort_impl;

  return n;
}

void insert_node(link_lst * n1, link_lst * n2) {
  n2->next = n1->next;
  n1->next = n2;
}

void remove_node(link_lst * n1) {
  n1->next = n1->next->next;
}

void link_lst_printf (link_lst * lst) {
  link_lst * n = lst;
  printf("members: ");
  while (n != NULL) {
    printf("%d ", n->data);
    n = n->next;
  }
  printf("\n");
}
void link_lst_fprintf(const char * filename, link_lst * lst){
  link_lst * n = lst;
  FILE * f = fopen(filename,"a");
  if (f == NULL){ perror("Error opening file!\n"); }
  else{
    while (n != NULL) {
      fprintf (f, "%d ",n->data);
      n = n->next;
    }
    fprintf(f, "\n");
    fclose(f);
  }
}

int link_lst_len(link_lst * lst){
  int l = 0;
  link_lst * n = lst;
  while (n != NULL){
    l++;
    n = n->next;
  }
  return l;
}

void add_member(link_lst * lst, int data){
  link_lst * now = lst;
  while (now->next != NULL){
    now = now->next;
  }
  link_lst * new_member = init_lst(data);
  now->next = new_member;
}

void free_link_lst(link_lst * lst) {
    if (lst->next != NULL) {
        free_link_lst(lst->next);
    }
    free(lst);
}

static void tree_norm_impl(tree * self){
  if ((self->l_tree != NULL) && (self->r_tree != NULL)) {
    int n1,n2;
    link_lst * lst1 = init_lst(-1);
    link_lst * lst2 = init_lst(-1);
    tree_get_member(self->l_tree,lst1);
    tree_get_member(self->r_tree,lst2);
    n1 = lst1->len(lst1);
    n2 = lst2->len(lst2);
    self->max_flw = self->max_flw/double(n1)/double(n2);
    tree_norm_impl(self->l_tree);
    tree_norm_impl(self->r_tree);
    free_link_lst(lst1);
    free_link_lst(lst2);
  }
}

static tree * tree_find_impl(tree * self,double data){
	if(self != NULL){
		double compare = fabs(self->max_flw - data);
  	if ( compare < EPSILON){
    	return self;
  	}
  	else{
			tree * foundSubTree = tree_find_impl(self->l_tree,data);	
   		if(foundSubTree == NULL){
				foundSubTree = tree_find_impl(self->r_tree,data);
			}
			return foundSubTree;
 		}
	}
	else{
		return NULL;
	}
}

int tree_size_impl(tree * self);

tree * insertnode(){
  // initialized the node of tree
  // printf("insertnode... \n");
  tree * newnode;
  newnode = (tree *)malloc(sizeof(tree));
  newnode->id = 0;
  newnode->rank = 0;
	newnode->members[0]=-1;
  newnode->par = NULL;
  newnode->l_tree = NULL;
  newnode->r_tree = NULL;
  newnode->max_flw = 1.0/0.0;
  
  // methods
	newnode->size = tree_size_impl;
  newnode->norm = tree_norm_impl;
  newnode->find = tree_find_impl;
  newnode->print = tree_printf;
	newnode->free = free_tree;
  // newnode->sort_data = tree_sort_data_impl;
  // newnode->members_modified = init_lst(0);
  return newnode;
}

int tree_cluster(tree * root){
  return 0;
}
// print tree on terminal
void print_member(tree * root){
  if (root != NULL){
    if ((root->l_tree == NULL) && (root->r_tree == NULL)){
      printf (" %d",root->members[0]);
    }
    else{
      print_member(root->l_tree);
      print_member(root->r_tree);
    }
  }
}
void tree_printf(tree * root){
  if(root != NULL) {
    printf("Rank: %d Id: %d\n",root->rank,root->id);
    printf("maximum flow: %.2f\n",root->max_flw);
    printf("members: ");
    print_member(root);
    printf("\n");
    tree_printf(root->l_tree);
    tree_printf(root->r_tree);
  }
}
void free_tree(tree * root) {
    if (root->l_tree != NULL) {
        free_tree(root->l_tree);
    }
    if (root->r_tree != NULL) {
        free_tree(root->r_tree);
    }
    free(root);
}
// print tree to file
void tree_get_member(tree * root, link_lst * members){
  link_lst * n = members;
  if (root != NULL){
    if ((root->l_tree == NULL) && (root->r_tree == NULL)){
      if(n->data == -1){
        n->data = root->members[0];
        // n->data = root->members[0]+1; // +1 for real label
      }
      else{
        add_member(n,root->members[0]);
        // add_member(n,root->members[0]+1); // +1 for real label
      }
      // printf("%d ",root->members[0]);
    }
    else{
      tree_get_member(root->l_tree,n);
      tree_get_member(root->r_tree,n);
    }
  }
}
int tree_size_impl(tree * self){
	link_lst * n = init_lst(-1);
	tree_get_member(self,n);
	return n->len(n);
}


// set a subtree format
void subtree_fprintf(const char * file, link_lst * subtree[3],tree * root){
  FILE * f = fopen(file,"a");
  link_lst * root_lst = subtree[0];
  link_lst * lt_lst = subtree[1];
  link_lst * rt_lst = subtree[2];
  link_lst * n = NULL;
  if (f == NULL){ perror("Error opening file!\n"); }
  else{
    if (link_lst_len(root_lst) > 3){
      fprintf(f, "\t\"C%d\"", root->id);
      fprintf(f, "[comment=\"");
      n = root_lst;
      while (n != NULL) {
        fprintf(f, "%d ",n->data );
        n = n->next;
      }
      fprintf(f, "\"];\n");
    }
    else{
      n = root_lst;
      fprintf(f, "\t\"");
      while (n != NULL) {
        fprintf (f, "%d ",n->data);
        n = n->next;
      }
      fprintf(f, "\"\n");
    }
    if((root->r_tree != NULL) && (root->l_tree != NULL)){
      if (link_lst_len(lt_lst) > 3){
        fprintf(f,"\t\"C%d\" -> ", root->l_tree->id);
      }
      else{
        n = lt_lst;
        fprintf(f, "\t\"");
        while (n != NULL) {
          fprintf (f, "%d ",n->data);
          n = n->next;
        }
        fprintf(f, "\" -> ");
      }

      if (link_lst_len(rt_lst) > 3){
        fprintf(f,"\t\"C%d\"", root->r_tree->id);
      }
      else{
        n = rt_lst;
        fprintf(f, "\t\"");
        while (n != NULL) {
          fprintf (f, "%d ",n->data);
          n = n->next;
        }
        fprintf(f, "\"");
      }
      fprintf(f, "[constraint=\"false\",");
      fprintf(f, " style=\"bold\",");
      fprintf(f, " label=\"%.2f\"];\n", root->max_flw );

      if (link_lst_len(root_lst) > 3){
        fprintf(f, "\t\"C%d\" -> ", root->id);
      }
      else{
        n = root_lst;
        fprintf(f, "\t\"");
        while (n != NULL) {
          fprintf (f, "%d ",n->data);
          n = n->next;
        }
        fprintf(f, "\" -> ");
      }
      if (link_lst_len(lt_lst) > 3){
        fprintf(f,"\t\"C%d\"", root->l_tree->id);
      }
      else{
        n = lt_lst;
        fprintf(f, "\t\"");
        while (n != NULL) {
          fprintf (f, "%d ",n->data);
          n = n->next;
        }
        fprintf(f, "\"");
      }
      fprintf(f, "[dir=\"none\", style=\"dashed\"];\n");
      if (link_lst_len(root_lst) > 3){
        fprintf(f, "\t\"C%d\" -> ", root->id);
      }
      else{
        n = root_lst;
        fprintf(f, "\t\"");
        while (n != NULL) {
          fprintf (f, "%d ",n->data);
          n = n->next;
        }
        fprintf(f, "\" -> ");
      }
      if (link_lst_len(rt_lst) > 3){
        fprintf(f,"\t\"C%d\"", root->r_tree->id);
      }
      else{
        n = rt_lst;
        fprintf(f, "\t\"");
        while (n != NULL) {
          fprintf (f, "%d ",n->data);
          n = n->next;
        }
        fprintf(f, "\"");
      }
      fprintf(f, "[dir=\"none\", style=\"dashed\"];\n");
    }
    fclose(f);
  }
}

void tree_fprintf( const char * filename, tree * root){
  if(root != NULL) {

    link_lst * subtree[3];
    for(int i = 0; i < 3; i++){
      subtree[i] = init_lst(-1);
    }

    tree_get_member(root, subtree[0]);
    tree_get_member(root->l_tree, subtree[1]);
    tree_get_member(root->r_tree, subtree[2]);

    subtree_fprintf(filename,subtree,root);

    for(int i = 0; i < 3; i++){
      free_link_lst(subtree[i]);
    }

    tree_fprintf(filename, root->l_tree);
    tree_fprintf(filename, root->r_tree);
  }
}

void tree_printf2file( const char * filename, tree * root){
  FILE * f = fopen(filename,"w");
  if (f == NULL) { perror("Error opening file!\n"); }
  else {fclose(f);}

  f = fopen(filename,"a");

  FILE * tmp1 = fopen("Tree_first.dot","r");
  char sentence[256] ;
  if (tmp1 == NULL){ perror("Error opening file!\n");}
  else{
    while (fgets(sentence,256,tmp1) != NULL){
        fputs(sentence,f);
    }
    fclose (tmp1);
  }
  fclose(f);

  tree_fprintf(filename,root);

  f = fopen(filename,"a");
  fprintf(f, "}\n");
  fclose(f);
}

void tree_reconstruct_0(tree * root){
  if((root->l_tree != NULL) && (root->r_tree != NULL)){
    tree * lt = root->l_tree;
    tree * rt = root->r_tree;
    tree_reconstruct_0(root->l_tree);
    tree_reconstruct_0(root->r_tree);
    while ((root->max_flw > lt->max_flw) || root->max_flw > rt->max_flw) {
      if (rt->max_flw > lt->max_flw) {

        double tmp = root->max_flw;
        tree * tmp_tr = rt;
        root->max_flw = lt->max_flw;
        root->r_tree = lt->r_tree;
        root->r_tree->par = root;
        lt->max_flw = tmp;
        lt->r_tree = tmp_tr;
        lt->r_tree->par = lt;
      }
      else{
        double tmp = root->max_flw;
        tree * tmp_tr = lt;
        root->max_flw = rt->max_flw;
        root->l_tree = rt->l_tree;
        root->l_tree->par = root;
        rt->max_flw = tmp;
        rt->l_tree = tmp_tr;
        rt->l_tree->par = rt;
      }
      tree_reconstruct_0(root->l_tree);
      tree_reconstruct_0(root->r_tree);
    }
  }
}


void tree_reconstruct(tree * root, int option){
    if(root != NULL){
      switch (option) {
        case 0:
          tree_reconstruct_0(root);
          break;

        default:
          printf("option %d hasn't been implement!",option);
      }
    }
}

// cluster
// list of list
static void cluster_push_impl(cluster *,link_lst *);
static void cluster_print_impl(cluster *,int);
static void cluster_free_impl(cluster *);
cluster * cluster_init(void){
	cluster * c = (cluster*)malloc(sizeof(cluster));
	c->member = NULL;
	c->next = NULL;

	c->push = cluster_push_impl;
	c->print = cluster_print_impl;
	c->free = cluster_free_impl;
	return c;
}

static void cluster_del_impl(cluster * self, link_lst ** data){
	cluster * curr = self;
	link_lst * nl = NULL;
	while(curr->next != NULL){
		curr = curr->next;
		if (curr->member->len(curr->member) > (*data)->len((*data))){
			nl = (*data);
			while(nl != NULL){
				curr->member->del(&(curr->member),nl->data);
				nl = nl->next;
			}
		}
		else{
			nl = curr->member;
			while(nl != NULL){
				(*data)->del(data,nl->data);
				nl = nl->next;
			}
		}
	}
	nl = (*data);
	while(nl != NULL) {
		self->member->del(&(self->member),nl->data);
		nl = nl->next;
	}
}

static void cluster_push_impl(cluster * self, link_lst * data){
	cluster_del_impl(self,&data);
	cluster * curr = self;
	while(curr->next != NULL){
		curr = curr->next;
	}
	curr->next = cluster_init();
	curr->next->member = data;
}
static void cluster_member_print_impl(link_lst * cluster_member){
 // fit matlab style
 link_lst * n = cluster_member;
 printf("[");
 while(n != NULL){
 	printf("%d ",n->data);
	n = n->next;
 }
 printf("] ");
}
static void cluster_print_impl(cluster * self,int num){
	cluster * curr = self;
	// int index = 1;
	printf("G%d = {",num);
	while(curr != NULL){
		curr->member->print = cluster_member_print_impl;// another print style
		curr->member->print(curr->member);
		// index++;
		curr = curr->next;
	}
	printf("};\n");
}
static void cluster_free_impl(cluster * self){
	if(self->next != NULL){
		cluster_free_impl(self->next);
	}
	free(self->member);
	free(self);
}
	
void get_tree_value(tree * root,link_lst * l){
	if(root->l_tree && root->r_tree){
		link_lst * n = init_lst(-1);
		n->fdata = root->max_flw;
		n->next = l->next;
		l->next = n;
		get_tree_value(root->l_tree,l);
		get_tree_value(root->r_tree,l);
	}
}

int get_max_flow(tree * root,double result[]){
	link_lst * pseudo_head = init_lst(-1);
	get_tree_value(root,pseudo_head);
	link_lst * tmp = pseudo_head;
	tmp = tmp->next;
	tmp->sort(&tmp,0);
	for(int i = 0; tmp != NULL;i++){
		result[i] = tmp->fdata;
		tmp = tmp->next;
	}
	pseudo_head->free(pseudo_head);
	return 0;
}

#ifdef TEST_LINKLST
int main(void){
	link_lst * a = init_lst(0);
	link_lst * b = init_lst(1);
	link_lst * c = init_lst(2);
	a->next = b;
	b->next = c;
	a->fdata = 1.0;
	b->fdata = -10.2;
	c->fdata = 3.2;

	a->sort(&a,0);
	a->print(a);

	return 0;
}

#endif

#ifdef MAIN
int main(int argc, char * argv[]){
	// LHCII 
	// made by hand XDDD
	int size = 27;
  tree * t[size];
	double max_flw[size] = {0.77, 100, 1.40, 0.65, 0.10,
	                      1.84, 100, 1.04, 10.49, 3.24,
												100, 100, 100, 100, 10.34, 0.86, 
												100, 100, 100, 0.90, 100, 2.88,
												100, 100, 2.65, 100, 100};


	for(int i = 0; i < size; i++){
		t[i] = insertnode();
		t[i]->id = i;
		t[i]->max_flw = max_flw[i];
	}
	t[0]->l_tree = t[1];
	t[0]->r_tree = t[2];
	t[1]->members[0] = 14;
	t[2]->l_tree = t[3];
	t[2]->r_tree = t[4];
	t[3]->l_tree = t[5];
	t[3]->r_tree = t[6];
	t[4]->l_tree = t[7];
	t[4]->r_tree = t[8];
	t[5]->l_tree = t[9];
	t[5]->r_tree = t[10];
	t[6]->members[0] = 3;
	t[7]->l_tree = t[11];
	t[7]->r_tree = t[12];
	t[8]->l_tree = t[13];
	t[8]->r_tree = t[14];
	t[9]->l_tree = t[15];
	t[9]->r_tree = t[16];
	t[10]->members[0] = 6;
	t[11]->members[0] = 7;
	t[12]->members[0] = 4;
	t[13]->members[0] = 5;
	t[14]->l_tree = t[17];
	t[14]->r_tree = t[18];
	t[15]->l_tree = t[19];
	t[15]->r_tree = t[20];
	t[16]->members[0] = 8;
	t[17]->members[0] = 2;
	t[18]->members[0] = 1;
	t[19]->l_tree = t[21];
	t[19]->r_tree = t[22];
	t[20]->members[0] = 9;
	t[21]->l_tree = t[23];
	t[21]->r_tree = t[24];
	t[22]->members[0] = 10;
	t[23]->members[0] = 13;
	t[24]->l_tree = t[25];
	t[24]->r_tree = t[26];
	t[25]->members[0] = 12;
	t[26]->members[0] = 11;
	
	// const char * filename = "LHCII_MON.dot";
	// tree_printf2file(filename,t[0]);
	
	// Elimination method
	double max_flw_acc[] = {0.1, 0.65, 0.77, 0.86, 0.90, 1.04, 1.4,
	                     1.84, 2.65, 2.88, 3.24, 10.34, 10.49};
	/*
	double norm_max_flw_acc[] = {0.02, 0.03, 0.06, 0.09, 0.21, 0.3,
	                     0.31, 0.65, 1.04, 1.44, 2.65, 5.24, 10.34};
	*/
	printf("Original tree: \n");
	for(int cluster_num = 2; cluster_num <= 14;cluster_num++){
		cluster * G = cluster_init();
		link_lst * member = init_lst(1);
	
		for(int i = 2; i <= 14; i++){
			member->push(member,i);
		}
		G->member = member;
		for(int i = 0; i < cluster_num-1; i++){
			tree * tmpT = t[0]->find(t[0],max_flw_acc[i]);
			if (tmpT->l_tree->size(tmpT->l_tree) >= tmpT->r_tree->size(tmpT->r_tree)){
				tmpT = tmpT->r_tree;
			}	
			else{
				tmpT = tmpT->l_tree;
			}
		
			link_lst * nl = init_lst(-1);
			tree_get_member(tmpT, nl);
			G->push(G,nl);
		}
		G->print(G,cluster_num);
		G->free(G);
	}
	
	printf("Normalized tree: \n");
	t[0]->norm(t[0]);
	double norm_max_flw_acc[13];
	get_max_flow(t[0],norm_max_flw_acc);
	
	for(int cluster_num = 2; cluster_num <= 14;cluster_num++){
		cluster * G = cluster_init();
		link_lst * member = init_lst(1);
	
		for(int i = 2; i <= 14; i++){
			member->push(member,i);
		}
		G->member = member;
		for(int i = 0; i < cluster_num-1; i++){
			tree * tmpT = t[0]->find(t[0],norm_max_flw_acc[i]);
			if (tmpT->l_tree->size(tmpT->l_tree) >= tmpT->r_tree->size(tmpT->r_tree)){
				tmpT = tmpT->r_tree;
			}	
			else{
				tmpT = tmpT->l_tree;
			}
		
			link_lst * nl = init_lst(-1);
			tree_get_member(tmpT, nl);
			G->push(G,nl);
		}
		G->print(G,cluster_num);
		G->free(G);
	}


	t[0]->free(t[0]);
	
  return 0;
}
#endif
