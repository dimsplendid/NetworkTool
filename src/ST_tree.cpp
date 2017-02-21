#include "ST_tree.h"
tree * insertnode(){
  // initialized the node of tree
  printf("insertnode... \n");
  tree * newnode;
  newnode = (tree *)malloc(sizeof(tree));
  newnode->id = 0;
  newnode->rank = 0;
  newnode->par = NULL;
  newnode->l_tree = NULL;
  newnode->r_tree = NULL;
  newnode->max_flw = 0.0;
  newnode->size=0;
  return newnode;
}
void tree_printf(tree * root){
  // print the tree under the root
  if (root != NULL) {
    tree * lt = root->l_tree;
    tree * rt = root->r_tree;
    /*
    printf("Node: ");
    for (int i = 0; i < root->members.size();i++){
      printf("%s ",root->members[i].c_str());
    }
    */
    // printf("test members: %s\n",root->members.at(0).c_str());
    printf("Rank: %d Id: %d\n",root->rank,root->id);
    printf("%.2f\n",root->max_flw);
    printf("members: \n");
    for (int i = 0; i < root->size; i++) {
      printf("%d ",root->members[i]);
    }
    printf("\n");
    tree_printf(lt);
    tree_printf(rt);
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
  rt->par = root;

  //root->members.push_back("root");
  //lt->members.push_back("lt");
  //rt->members.push_back("rt");
  tree_printf(root);
  return 0;
}
#endif
