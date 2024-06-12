#include "mmt.h"
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <stdint.h>

// using namespace std;

mmt_node* root;
std::vector<mmt_node*> root_leaf_nodes;

mmt_node* create_node(mmt_node* parent = nullptr, uint8_t* data = nullptr, int size = 0) {
    mmt_node* node = new mmt_node();
    node->data = data;
    node->size = size;
    // compute HMAC
    // node->HMAC = compute_HMAC(data, size);
    node->parent = parent;
    for(int i = 0; i < 8; i++) {
        node->children[i] = nullptr;
    }
    return node;
}

//assume the depth of rootTree is 2, which supports 8 leaf nodes
void create_root_tree() {
    root = new mmt_node();
    root->parent = nullptr;
    for(int i = 0; i < 8; i++) {
        // cout << i << endl;
        mmt_node* child = new mmt_node();
        root->children[i] = child;
        child->parent = root;
        root_leaf_nodes.push_back(child);

    }
    // return root;
}

void create_sub_tree_helper(mmt_node* node, int depth, std::vector<mmt_node*>& leaf_node, int page_num) {
    if(depth == 0) 
        return;
    for(int i = 0; i < 8; i++) {
        node->children[i] = new mmt_node();
        node->children[i]->parent = node;

        if(depth == 1 && page_num > 0) {
            leaf_node.push_back(node->children[i]); 
        }
        create_sub_tree_helper(node->children[i], depth-1, leaf_node, page_num-1);
    }
}

mmt_node* create_sub_tree(int page_num, uint8_t* data, int size, std::vector<mmt_node*>& sub_leaf_nodes) {
    mmt_node* sub_root = new mmt_node();
    int depth = 1;
    while(page_num/8 > 0) {
        depth++;
        page_num = page_num/8;
    }
    //create the tree
    create_sub_tree_helper(sub_root, depth, sub_leaf_nodes, page_num);

    //insert value
    if(size < EPC_PAGE_SIZE) {
        for(int i = 0; i < page_num; i++) {
            if(sub_leaf_nodes[i]->data == nullptr) {
                sub_leaf_nodes[i]->data = data + i*128;
                sub_leaf_nodes[i]->size = size;
                sub_leaf_nodes[i]->HMAC = i;
            }
        }
    }
    
    //update the whole sub tree
    mmt_node* parent_node = sub_leaf_nodes[0]->parent;
    while(parent_node!=nullptr) {
        (parent_node->HMAC)++;
        parent_node = parent_node->parent;
    }

    return sub_root;
}

int add_sub_tree(mmt_node* sub) {
    //replace leaf node
    int i = 0;
    for(; i < root_leaf_nodes.size(); i++) {
        if(root_leaf_nodes[i]->data == nullptr) {
            int node_idx = i % 8;
            sub->parent = root_leaf_nodes[i]->parent;
            root_leaf_nodes[i]->parent->children[node_idx] = sub;
            break;
        }
    }
    if(i == root_leaf_nodes.size()) {
        // std::cout << "fail to insert the node: no available empty leaf node.\n";
        return -1;
    }
    //update HMAC
    mmt_node* update = sub->parent;
    while(update!=nullptr) {
        //calculate new HMAC
        update = update->parent;
    }
    return i;
}

// void update_sub_tree(int idx, uint8_t* data, int size, vector<mmt_node*>& sub_leaf_nodes) {
//     mmt_node* target = sub_leaf_nodes[idx];
//     target->parent->children[idx%8]->data = data;
//     target->parent->children[idx%8]->size = size;  
//     //calculate HMAC
//     mmt_node* update = target->parent;
//     while(update!=nullptr) {
//         //calculate new HMAC
//         update = update->parent;
//     }   
// }

void remove_sub_tree(int idx) {
    mmt_node* target = root_leaf_nodes[idx];
    target->parent->children[idx%8]->data = nullptr;
    target->parent->children[idx%8]->size = 0;
    for(int i = 0; i < 8; i++) {
        target->parent->children[idx%8]->children[i] = nullptr;
    }
    //update HMAC
    mmt_node* update = target->parent;
    while(update!=nullptr) {
        //calculate new HMAC
        update = update->parent;
    } 
    target->parent = nullptr;  
}

void mmt_traverse(mmt_node* node, int depth) {
    if(node == nullptr) { 
        // cout << "null" << " "; return;
    }
    // cout << depth << ": ";
    if(node->data != nullptr) {
        for(int i = 0; i < node->size; i++) {
            // cout << (int)node->data[i] << " ";
        }
    }
    // cout << endl;
    for(int i = 0; i < 8; i++) {
        // cout << depth << " - child " << i << ": ";
        mmt_traverse(node->children[i], depth-1);
        // cout << endl;
    }
}


// int main() {
//     // mmt_node tmp;
//     create_root_tree();

//     uint8_t* sample_data = (uint8_t*)malloc(sizeof(uint8_t)*128);
//     for(int i = 0; i < 128; i++) {
//         sample_data[i] = i%128;
//     }
//     int page_num = 1;

//     vector<mmt_node*> subTreeLeaf;
//     mmt_node* subTree = create_sub_tree(page_num, sample_data, 128, subTreeLeaf);

//     int idx = add_sub_tree(subTree);
//     // std::cout << "index: " << idx << std::endl;

//     // mmt_traverse(root, 3);

//     remove_sub_tree(idx);



//     return 0;

// }