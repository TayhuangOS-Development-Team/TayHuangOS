/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * signal.c
 * 
 * 信号量
 * 
 */



#include <task/signal.h>
#include <memory/kheap.h>
#include <logging.h>
#include <printk.h>

typedef struct __signal_node {
    id_t id;

    qword signals;
    qword max_signals;

    struct __signal_node *left;
    struct __signal_node *right;
    struct __signal_node *parent;
    int factor;
} signal_node;

//创建
PRIVATE signal_node *create_signal_node(void) {
    signal_node *node = kmalloc(sizeof(signal_node));

    node->left = node->right = node->parent = NULL;
    node->signals = node->max_signals = node->id = 0;
    node->factor = 0;
    
    return node;
}

//释放
PRIVATE void free_signal_node(signal_node *node) {
    if (node->left != NULL) {
        free_signal_node(node->left);
    }

    if (node->right != NULL) {
        free_signal_node(node->right);
    }

    kfree(node);
}

PRIVATE signal_node *signal_root = NULL;

//修正LL
PRIVATE void __fix_ll(signal_node *node) {
    signal_node *subl   = node->left;
    signal_node *sublr  = subl->right;
    signal_node *parent = node->parent;

    //node的左孩子设为left的右孩子
    node->left = sublr;
    if (sublr != NULL) { 
        sublr->parent = node;
    }

    //node设为left的右孩子
    subl->right = node;
    node->parent = subl;

    subl->parent = parent;

    //node是root
    if (parent == NULL) { 
        signal_root = subl;
    }
    else {
        if (node == parent->left) {
            parent->left = subl;
        }
        else  {
            parent->right = subl;
        }
    }
    //调整因子
    node->factor = subl->factor = 0;
}

//修正RR
PRIVATE void __fix_rr(signal_node *node) {
    signal_node *subr   = node->right;
    signal_node *subrl  = subr->left;
    signal_node *parent = node->parent;

    //node的右孩子设为right的左孩子
    node->right = subrl;
    if (subrl != NULL) { 
        subrl->parent = node;
    }

    //node设为right的左孩子
    subr->left = node;
    node->parent = subr;

    subr->parent = parent;

    //node是root
    if (parent == NULL) { 
        signal_root = subr;
    }
    else {
        if (node == parent->left) {
            parent->left = subr;
        }
        else  {
            parent->right = subr;
        }
    }
    //调整因子
    node->factor = subr->factor = 0;
}

//修正LR
PRIVATE void __fix_lr(signal_node *node) {
    signal_node *subl  = node->left;
    signal_node *sublr = subl->right;

    int sublr_factor = sublr->factor;

    //修正左子树RR
    __fix_rr(node->left);
    //修正LL 
    __fix_ll(node);

    //调整因子
    if (sublr_factor == 1) {
        subl->factor = -1;
    }
    else if (sublr_factor == -1) {
        node->factor = 1;
    }
}

//修正RL
PRIVATE void __fix_rl(signal_node *node) {
    //修正右子树LL
    __fix_ll(node->right);
    //修正RR
    __fix_rr(node);
}

//调整因子
PRIVATE void update_factor(signal_node *node) {
    signal_node *parent = node->parent;

    while (parent != NULL) {
        if (node == parent->left) { //左侧
            parent->factor --;
        }
        else { //右侧
            parent->factor ++;
        }

        if (parent->factor == 0) { //调整完成
            break;
        }

        if ((node->parent->factor == 1) || (node->parent->factor == -1)) {
            node = parent;
            parent = node->parent;
        }
        else {
            if (parent->factor == -2) {
                if (parent->left->factor == -1) {
                    linfo ("Signal", "Fix %d LL", parent->id);
                    __fix_ll(parent);
                }
                else {
                    linfo ("Signal", "Fix %d LR", parent->id);
                    __fix_lr(parent);
                }
            }
            else if (parent->factor == 2) {
                if (parent->right->factor == 1) {
                    linfo ("Signal", "Fix %d RR", parent->id);
                    __fix_rr(parent);
                }
                else {
                    linfo ("Signal", "Fix %d RL", parent->id);
                    __fix_rl(parent);
                }
            }
            break;
        }
    }
}

PRIVATE void __insert_signal(signal_node *parent, signal_node *node) {
    if (node->id == parent->id) {
        lerror("Signal", "Signal ID %d exists!", node->id);
        return;
    }
    else if (node->id > parent->id) {
        if (parent->left == NULL) {
            node->parent = parent;
            parent->left = node;
            update_factor(node);
        }
        else {
            __insert_signal(parent->left, node);
        }
    }
    else if (node->id < parent->id) {
        if (parent->right == NULL) {
            node->parent = parent;
            parent->right = node;
            update_factor(node);
        }
        else {
            __insert_signal(parent->right, node);
        }
    }
}

PRIVATE void insert_signal(signal_node *node) {
    if (signal_root == NULL) {
        signal_root = node;
        return;
    }

    __insert_signal(signal_root, node);
}

PRIVATE id_t cur_id = 0;

PUBLIC id_t create_signal(qword max_signals) {
    id_t id = cur_id ++;

    signal_node *node = create_signal_node();

    node->id = id;
    node->max_signals = max_signals;

    insert_signal(node);

    return id;
}

PUBLIC void __print_tree(int level, signal_node *node) {
    for (int i = 0 ; i < level ; i ++) {
        write_serial_char(' ');
    }

    if (node == NULL) {
        write_serial_str("[]\n");
        return;
    }

    char buffer[256];
    sprintk(buffer, "(%d)[%d/%d]:\n", node->id, node->signals, node->max_signals);
    write_serial_str(buffer);

    __print_tree(level + 1, node->left);
    __print_tree(level + 1, node->right);
}

PUBLIC void print_tree(void) {
    __print_tree(0, signal_root);
}