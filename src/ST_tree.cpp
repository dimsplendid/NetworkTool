#include "ST_tree.h"

void tree_get_member(tree * root, link_lst * members);
void tree_printf(tree * root);

static void lst_del_impl(link_lst ** self,int data){
  link_lst * curr = (*self),
           * prev = NULL;

  while (curr != NULL && curr->data != data) {
    prev = curr;
    curr = curr->next;
  }

  if(curr == 0){
    printf("no %d in the list\n", data);
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

link_lst * init_lst(int data){
	// dynamic allocate memory

	link_lst * n = (link_lst*)malloc(sizeof(link_lst));
	n->data = data;
  n->next = NULL;
  n->del = lst_del_impl;
  n->push = lst_push_impl;
  n->free = lst_free_impl;
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
    n1 = link_lst_len(lst1);
    n2 = link_lst_len(lst2);
    self->max_flw = self->max_flw/double(n1)/double(n2);
    tree_norm_impl(self->l_tree);
    tree_norm_impl(self->r_tree);
    free_link_lst(lst1);
    free_link_lst(lst2);
  }
}

static tree * tree_find_impl(tree * self,double data){
	if(self != NULL){
  	if (self->max_flw == data){
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

tree * insertnode(){
  // initialized the node of tree
  // printf("insertnode... \n");
  tree * newnode;
  newnode = (tree *)malloc(sizeof(tree));
  newnode->id = 0;
  newnode->rank = 0;
  newnode->par = NULL;
  newnode->l_tree = NULL;
  newnode->r_tree = NULL;
  newnode->max_flw = 0.0;
  newnode->size=0;
  // methods
  newnode->norm = tree_norm_impl;
  newnode->find = tree_find_impl;
  newnode->print = tree_printf;
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
        // n->data = root->members[0];
        n->data = root->members[0]+1; // +1 for real label
      }
      else{
        // add_member(n,root->members[0]);
        add_member(n,root->members[0]+1); // +1 for real label
      }
      // printf("%d ",root->members[0]);
    }
    else{
      tree_get_member(root->l_tree,n);
      tree_get_member(root->r_tree,n);
    }
  }
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

#ifdef MAIN
int main(){
  tree * root = insertnode();
  tree * lt = insertnode();
  tree * rt = insertnode();
  root->max_flw = 1.0;
  lt->max_flw = 2.0;
  rt->max_flw = 3.0;
  root->l_tree = lt;
  root->r_tree = rt;
  lt->par = root;
	lt->rank = 1;
  rt->par = root;
	rt->rank = 1;
  lt->members[0]=1;
  rt->members[0]=2;

  tree * n = root->find(root,1.0);
  n->print(n);

  return 0;
}
#endif
