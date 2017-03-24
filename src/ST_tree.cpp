#include "ST_tree.h"
link_lst * creat_node(int data){
	// dynamic allocate memory
  printf("creat node...\n");
  printf("data is: %d\n",data);

	link_lst * n = (link_lst*)malloc(sizeof(link_lst));
	n->data = data;
  n->next = NULL;
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

void add_member(link_lst * lst, int data){
  link_lst * now = lst;
  while (now->next != NULL){
    now = now->next;
  }
  link_lst * new_member = creat_node(data);
  now->next = new_member;
}

void free_link_lst(link_lst * lst) {
    if (lst->next != NULL) {
        free_link_lst(lst->next);
    }
    free(lst);
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
  // newnode->members_modified = creat_node(0);
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

// print tree to file
void tree_get_member(tree * root, link_lst * members){
  link_lst * n = members;
  if (root != NULL){
    if ((root->l_tree == NULL) && (root->r_tree == NULL)){
      if(n->data == -1){
        printf("set n->data = member\n" );
        n->data = root->members[0];
      }
      else{
        printf("call add_member()\n" );
        add_member(n,root->members[0]);
      }
      printf("%d ",root->members[0]);
    }
    else{
      tree_get_member(root->l_tree,n);
      tree_get_member(root->r_tree,n);
    }
  }
}

void tree_fprintf( const char * filename, tree * root){
  if(root != NULL) {
    link_lst * root_members = creat_node(-1);
    // link_lst * lt_members = NULL;
    // link_lst * rt_members = NULL;

    tree_get_member(root, root_members);
    link_lst_fprintf(filename,root_members);
    link_lst_printf(root_members);

    free_link_lst(root_members);

    tree_fprintf(filename, root->l_tree);
    tree_fprintf(filename, root->r_tree);

  }
}

void tree_printf2file( const char * filename, tree * root){
  FILE * f = fopen(filename,"w");
  if (f == NULL) { perror("Error opening file!\n"); }
  else {fclose(f);}

  f = fopen(filename,"a");
  if (f == NULL) { perror("Error opening file!\n"); }

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
  rt->par = root;
  lt->members[0]=1;
  rt->members[0]=2;

  // tree_printf(root);
  const char * filename = "mT.dot";
  tree_printf2file(filename,root);

  return 0;
}
#endif
