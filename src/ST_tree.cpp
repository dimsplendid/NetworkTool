#include "ST_tree.h"
#include <cmath>
#define EPSILON 0.00001

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
  printf("%% data = [ ");
  while (n != NULL) {
    printf("%f ", n->fdata);
    n = n->next;
  }
  printf("] \n");

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

static tree * tree_copy_impl(tree * self){
  tree * copy_tree = NULL;
  if(self != NULL){
    copy_tree = insertnode();
    copy_tree->id = self->id;
    copy_tree->rank = self->rank;
    copy_tree->members[0] = self->members[0];
    copy_tree->l_tree = tree_copy_impl(self->l_tree);
    copy_tree->r_tree = tree_copy_impl(self->r_tree);
    if (copy_tree->l_tree != NULL){ copy_tree->l_tree->par = copy_tree;}
    if (copy_tree->r_tree != NULL){ copy_tree->r_tree->par = copy_tree;}
    copy_tree->max_flw = self->max_flw;
  }
  return copy_tree;
}


static int tree_size_impl(tree * self);
static int tree_cluster_impl(tree * self,int opt[3]);

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
  newnode->copy = tree_copy_impl;
	newnode->size = tree_size_impl;
  newnode->norm = tree_norm_impl;
  newnode->find = tree_find_impl;
	newnode->cluster = tree_cluster_impl;
  newnode->print = tree_printf;
	newnode->free = free_tree;
  // newnode->sort_data = tree_sort_data_impl;
  // newnode->members_modified = init_lst(0);
  return newnode;
}
static int get_max_flow(tree * root,double result[]);

static int tree_cluster_method_simple_cut(tree * self);
static int tree_cluster_method_simple_ratio(tree * self);
static int tree_cluster_method_elimination(tree * self);

static int tree_cluster_impl(tree * self,int option[3]){
  // option: {reconstruct ,normalized , methods}
  enum methods {simple_cut,simple_ratio,elimination};
  int reconstruct = option[0];
  int normalized = option[1];
  methods method = (methods)option[2];

  tree * copy_tree = self->copy(self);
  if (reconstruct == 1) {tree_reconstruct(copy_tree,0);}
  if (normalized == 1) {copy_tree->norm(copy_tree);}
  printf(" G_%d%d%d = {",option[0],option[1],option[2]);
  switch (method){
    case simple_cut: tree_cluster_method_simple_cut(copy_tree); break;
    case simple_ratio: tree_cluster_method_simple_ratio(copy_tree); break;
    case elimination: tree_cluster_method_elimination(copy_tree); break;
    default: printf("method out of range\n");
  }
  printf("};\n");
  copy_tree->free(copy_tree);
	return 0;
}

static int tree_cluster_method_elimination(tree * self){
  int size = self->size(self);
	double max_flw_acc[size-1];
	get_max_flow(self,max_flw_acc);

	for(int cluster_num = 2; cluster_num <= size; cluster_num++){
		cluster * G = cluster_init();
		link_lst * l = init_lst(1);
		for (int i = 2; i <= size; i++) {
			l->push(l,i);
		}

		G->member = l;
		for(int i = 0; i < (cluster_num-1); i++){
			tree * tmpT = self->find(self,max_flw_acc[i]);
			if (tmpT->l_tree->max_flw <= tmpT->r_tree->max_flw){
				tmpT = tmpT->r_tree;
			}
			else{
				tmpT = tmpT->l_tree;
			}
			link_lst * nl = init_lst(-1);
			tree_get_member(tmpT, nl);
      G->del(G,&nl);
			G->push(G,nl);
		}
		G->print(G);
		G->free(G);
	}
  return 0;
}

static int tree_cluster_simple_cut_aux(tree * root, double cut_off, cluster * G){
  if((root->max_flw > cut_off) || root->size(root) == 1){
    link_lst * nl = init_lst(-1);
    tree_get_member(root,nl);
    G->push(G,nl);
  }
  else{
    tree_cluster_simple_cut_aux(root->l_tree, cut_off, G);
    tree_cluster_simple_cut_aux(root->r_tree, cut_off, G);
  }
  return 0;
}

static int tree_cluster_method_simple_cut(tree * self){
  // 1. get acc cut off list
  int size = self->size(self);
	double max_flw_acc[size-1];
	get_max_flow(self,max_flw_acc);

  for(int i = 0; i < size-1; i++){
    cluster * G = cluster_init();
    link_lst * l = init_lst(-1);
    G->member = l;
    double cut_off = max_flw_acc[i];
    tree_cluster_simple_cut_aux(self, cut_off, G);
    G->print(G->next);
    G->free(G);
  }
  return 0;
}

static int tree_cluster_simple_ratio_aux(tree * root, double ratio, cluster * G){
  if(root->l_tree != NULL){
    if(((root->l_tree->max_flw/root->max_flw) > ratio) || root->l_tree->size(root->r_tree) == 1){
      link_lst * nl = init_lst(-1);
      tree_get_member(root->l_tree,nl);
      G->push(G,nl);
    }
    else{
      tree_cluster_simple_ratio_aux(root->l_tree, ratio, G);
    }
  }
  if(root->r_tree != NULL){
    if(((root->r_tree->max_flw/root->max_flw) > ratio) || root->r_tree->size(root->r_tree) == 1){
      link_lst * nl = init_lst(-1);
      tree_get_member(root->r_tree,nl);
      G->push(G,nl);
   }
    else{
      tree_cluster_simple_ratio_aux(root->r_tree, ratio, G);
   }
  }
  return 0;
}

static int tree_cluster_method_simple_ratio(tree * self){
  // 1. get acc cut off list
  int size = self->size(self);
  double base = 2.0;
	double max_flw_acc[size-1];
  max_flw_acc[size-2] = 1.0;
  for (int i = 1; i < size-1; i++){
    max_flw_acc[size-2-i] = max_flw_acc[size-1-i]*base;
  }
  printf("%% data = [ ");
  for (int i = 0; i < size-1; i++){
    printf("%.2f ",max_flw_acc[i]);
  }
  printf("];\n");
	
  for(int i = 0; i < size-1; i++){
    cluster * G = cluster_init();
    link_lst * l = init_lst(-1);
    G->member = l;
    double cut_off = max_flw_acc[i];
    tree_cluster_simple_ratio_aux(self, cut_off, G);
    
    G->print(G->next);
    G->free(G);
  }
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

static int tree_size_impl(tree * self){
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
  if (f == NULL) { fprintf(stderr,"Error opening file \"%s\"!\n",filename);}
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
static void cluster_print_impl(cluster *);
static void cluster_free_impl(cluster *);
static void cluster_del_impl(cluster * self, link_lst ** data);
static int cluster_length_impl(cluster * self);
cluster * cluster_init(void){
	cluster * c = (cluster*)malloc(sizeof(cluster));
	c->member = NULL;
	c->next = NULL;

  c->len = cluster_length_impl;
  c->del = cluster_del_impl;
	c->push = cluster_push_impl;
	c->print = cluster_print_impl;
	c->free = cluster_free_impl;
	return c;
}

static int cluster_length_impl(cluster * self){
  cluster * curr = self;
  int count = 1;
  while(curr->next != NULL){
    curr = curr->next;
    count++;
  }
  return count;
}

static void cluster_del_impl(cluster * self, link_lst ** data){
	cluster * curr = self;
	link_lst * nl = NULL;
	while((curr->next != NULL) && ((*data) != NULL)){
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
	cluster * curr = self;
	if(data != NULL){
		while(curr->next != NULL){
			curr = curr->next;
		}
		curr->next = cluster_init();
		curr->next->member = data;
	}
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
static void cluster_print_impl(cluster * self){
	cluster * curr = self;
  // printf("G&02d: {",num)
  printf("{");
	while(curr != NULL){
		curr->member->print = cluster_member_print_impl;// another print style
		curr->member->print(curr->member);
		// index++;
		curr = curr->next;
	}
	printf("} \n");
}
static void cluster_free_impl(cluster * self){
	if(self->next != NULL){
		cluster_free_impl(self->next);
	}
	free(self->member);
	free(self);
}

static void get_tree_value(tree * root,link_lst * l){
	if(root->l_tree && root->r_tree){
		link_lst * n = init_lst(-1);
		n->fdata = root->max_flw;
		n->next = l->next;
		l->next = n;
		get_tree_value(root->l_tree,l);
		get_tree_value(root->r_tree,l);
	}
}

static int get_max_flow(tree * root,double result[]){
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

  int opt[3] = {0,0,0};
  printf("Original tree: \n");
  opt[0] = 0;
  printf("Method 1. simple cut\n");
  opt[2] = 0;
  t[0]->cluster(t[0],opt);
  printf("Method 2. simple ratio\n");
  opt[2] = 1;
  t[0]->cluster(t[0],opt);
  printf("Method 3. elimination\n");
  opt[2] = 2;
  t[0]->cluster(t[0],opt);
  
  /*-----------------------------------------*/
	printf("Normalized tree: \n");
  opt[1] = 1;
  printf("Method 1. simple cut\n");
  opt[2] = 0;
  t[0]->cluster(t[0],opt);
  printf("Method 2. simple ratio\n");
  opt[2] = 1;
  t[0]->cluster(t[0],opt);
  printf("Method 3. elimination\n");
  opt[2] = 2;
  t[0]->cluster(t[0],opt);

	t[0]->free(t[0]);

  return 0;
}
#endif
