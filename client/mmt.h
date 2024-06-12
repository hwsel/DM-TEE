#pragma once

#define EPC_PAGE_SIZE 1024*4

struct mmt_node{
    uint64_t HMAC = 0;
    uint8_t* data = nullptr;
    int size = 0;
    mmt_node* parent = nullptr;
    mmt_node* children[8];
} _mmt_node_t;

mmt_node* create_node(mmt_node* parent = nullptr, uint8_t* data = nullptr, int size = 0);
void create_root_tree();
void create_sub_tree_helper(mmt_node* node, int depth, std::vector<mmt_node*>& leaf_node, int page_num);
mmt_node* create_sub_tree(int page_num, uint8_t* data, int size, std::vector<mmt_node*>& sub_leaf_nodes);
int add_sub_tree(mmt_node* sub);
void remove_sub_tree(int idx);
void mmt_traverse(mmt_node* node, int depth);