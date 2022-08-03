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
#include <task/thread.h>
#include <task/task_scheduler.h>
#include <task/task_manager.h>
#include <memory/kheap.h>
#include <logging.h>
#include <printk.h>

//----------------------------

//等待进程队列
typedef struct __wtnode_t {
    thread_info_struct *thread;
    struct __wtnode_t *next;
} wtnode_t;

PRIVATE wtnode_t *wthead, *wttail;

PRIVATE void enqueue_wt(thread_info_struct *thread) {
    wtnode_t *node = (wtnode_t*)kmalloc(sizeof(wtnode_t));

    node->thread = thread;
    node->next = NULL;

    //队列为空
    if (wttail == NULL) {
        wthead = wttail = node;
    }
    else { //加入队尾
        wttail->next = node;
        wttail = node;
    }
}

PRIVATE thread_info_struct *dequeue_wt(void) {
    if (wthead == NULL) {
        return NULL;
    }

    wtnode_t *head = wthead;
    wthead = head->next;

    if (wthead == NULL) { //队列为空
        wttail = NULL;
    }

    thread_info_struct *thread = head->thread;
    kfree(head);

    return thread;
}

//----------------------------

typedef struct {
    id_t id;

    int signals;
    int max_signals;

    bool soft; //当soft = 1时，不在up和down操作中对进程进行休眠

    
} signal_t;

typedef struct __signal_node {
    signal_t signal;

    struct __signal_node *left;
    struct __signal_node *right;
    struct __signal_node *parent;
    int factor;
} signal_node;

//创建
PRIVATE signal_node *create_signal_node(void) {
    signal_node *node = kmalloc(sizeof(signal_node));

    node->left = node->right = node->parent = NULL;
    node->signal.signals = node->signal.max_signals = node->signal.id = node->signal.soft = 0;
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

PRIVATE void __insert_signal(signal_node *parent, signal_node *node) {
    while (parent != NULL) {
        if (node->signal.id == parent->signal.id) {
            lerror("Signal", "Signal ID %d exists!", node->signal.id);
            return;
        }
        else if (node->signal.id > parent->signal.id) { //左侧
            if (parent->left == NULL) { //仍然有孩子
                node->parent = parent;
                parent->left = node;
                break;
            }
            else {
                parent = parent->left;
            }
        }
        else if (node->signal.id < parent->signal.id) { //右侧
            if (parent->right == NULL) { //仍然有孩子
                node->parent = parent;
                parent->right = node;
                break;
            }
            else {
                parent = parent->right;
            }
        }
    }

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
                    __fix_ll(parent);
                }
                else {
                    __fix_lr(parent);
                }
            }
            else if (parent->factor == 2) {
                if (parent->right->factor == 1) {
                    __fix_rr(parent);
                }
                else {
                    __fix_rl(parent);
                }
            }
            break;
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

PRIVATE signal_t *get_signal(id_t id) {
    signal_node *node = signal_root;

    while (node != NULL) {
        if (node->signal.id == id) {
            return &node->signal;
        }
        else if (id > node->signal.id) {
            node = node->left;
        }
        else if (id < node->signal.id) {
            node = node->right;
        }
    }

    return NULL;
}

//---------------------------------

PRIVATE id_t cur_id = 0;

PUBLIC id_t __create_signal(int max_signals, int value, bool soft) {
    id_t id = cur_id;
    cur_id ++;

    signal_node *node = create_signal_node();

    node->signal.id = id;
    node->signal.signals = value;
    node->signal.max_signals = max_signals;
    node->signal.soft = soft;

    insert_signal(node);

    return node->signal.id;
}

PUBLIC void __up_signal(id_t id) {
    signal_t *signal = get_signal(id);

    if (signal->signals < signal->max_signals) {
        signal->signals ++;
    }

    if (! signal->soft) {
        if (signal->signals <= 0) {
            thread_info_struct *thread = dequeue_wt();
            thread->state = READY;

            enqueue_thread(thread);
        }
    }
}

PUBLIC void __down_signal(id_t id) {
    signal_t *signal = get_signal(id);

    signal->signals --;
    
    if (! signal->soft) {
        if (signal->signals < 0){ //等待
            enqueue_wt(current_thread);
            current_thread->state = WAITING;
        }
    }
}

PUBLIC int __get_signals(id_t id) {
    return get_signal(id)->signals;
}

PUBLIC bool __is_soft_signal(id_t id) {
    return get_signal(id)->soft;
}