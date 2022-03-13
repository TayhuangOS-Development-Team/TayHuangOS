/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * vmareas.c
 *
 * 虚空间平衡树
 *
 */



#include "vmareas.h"
#include "../kheap.h"

PRIVATE void update_height(vm_area *node) {
	int lheight = node->left ? node->left->height : 0;
	int rheight = node->right ? node->right->height : 0;
	node->height = max(lheight, rheight) + 1;
}

PRIVATE int dis_height(vm_area *node) {
	int lheight = node->left ? node->left->height : 0;
	int rheight = node->right ? node->right->height : 0;
	return lheight - rheight;
}

PUBLIC vm_area *create_vmarea(void) {
	vm_area *node = (vm_area*)malloc(sizeof(vm_area));
	node->height = 1;
	node->left = NULL;
	node->right = NULL;
	return node;
}

PRIVATE vm_area *fix_ll(vm_area *an) {
    vm_area* bn = an->left;
    vm_area* cn = bn->right;
    bn->right = an;
    an->left = cn;

    update_height(an);
    update_height(bn);
    return bn;
}

PRIVATE vm_area *fix_rr(vm_area *an) {
    vm_area *bn = an->right;
    vm_area *cn = bn->left;
    bn->left = an;
    an->right = cn;

    update_height(an);
    update_height(bn);
    return bn;
}

PRIVATE vm_area *fix_lr(vm_area *an) {
    an->left = fix_rr(an->left);
    return fix_ll(an);
}

PRIVATE vm_area *fix_rl(vm_area *an) {
    an->right = fix_ll(an->right);
    return fix_rr(an);
}

PUBLIC vm_area *vmarea_insert(vm_area *node, void *vm_start, void *vm_end,
    byte container_type, word container_info, byte shared_protection, byte global_protection, byte attribute) {
    if (node == NULL) {
        node = create_vmarea();
        node->vm_start = vm_start;
        node->vm_end = vm_end;
        node->container_info.container_type = container_type;
        node->container_info.container_info = container_info;

        node->attribute.present         = (attribute         & VM_ATTR_PRESENT) != 0;
        node->attribute.read            = (attribute         & VM_ATTR_READ   ) != 0;
        node->attribute.write           = (attribute         & VM_ATTR_WRITE  ) != 0;
        node->attribute.execute         = (attribute         & VM_ATTR_EXECUTE) != 0;
        node->attribute.shared          = (attribute         & VM_ATTR_SHARED ) != 0;
        node->attribute.reserved        = 0;

        node->protection.system         = (attribute         & VM_ATTR_SYSTEM ) != 0;
        node->protection.shared_read    = (shared_protection & VM_ATTR_READ   ) != 0;
        node->protection.shared_write   = (shared_protection & VM_ATTR_WRITE  ) != 0;
        node->protection.shared_execute = (shared_protection & VM_ATTR_EXECUTE) != 0;
        node->protection.global_read    = (global_protection & VM_ATTR_READ   ) != 0;
        node->protection.global_write   = (global_protection & VM_ATTR_WRITE  ) != 0;
        node->protection.global_execute = (global_protection & VM_ATTR_EXECUTE) != 0;
        node->protection.reserved       = 0;
    }
    else if(node->vm_end < vm_end) { //大于: right
        node->right = vmarea_insert(node->right, vm_start, vm_end, container_type, container_info, shared_protection, global_protection, attribute);
        int disheight = dis_height(node);
        if (disheight == -2) {
            if(vm_end > node->right->vm_end)
                node = fix_rr(node); //RR
            else
                node = fix_rl(node); //RL
        }
    }
    else if (node->vm_start > vm_start) { //小于: left
        node->left = vmarea_insert(node->left, vm_start, vm_end, container_type, container_info, shared_protection, global_protection, attribute);
        int disheight = dis_height(node);
        if (disheight == 2) {
            if(vm_start < node->left->vm_start)
                node = fix_ll(node); //LL
            else
                node = fix_lr(node); //LR
        }
    }
    update_height(node);
    return node;
}

PUBLIC vm_area *vmarea_delete(vm_area *node, void *vm_start, void *vm_end) {
    if (node == NULL) {
        return NULL;
    }
    else if (node->vm_end < vm_end)  {//大于: right
        node->right = vmarea_delete(node->right, vm_start, vm_end);
    }
    else if (node->vm_start > vm_start) { //小于: left
        node->left = vmarea_delete(node->left, vm_start, vm_end);
    }
    else {
        if (node->left) {
            vm_area *nd = node->left;
            while (nd->right != NULL)
                nd = nd->right;
            node->vm_start = nd->vm_start;
            node->vm_end = nd->vm_end;
            node->container_info = nd->container_info;
            node->attribute = nd->attribute;
            node->protection = nd->protection;
            node->left = vmarea_delete(node->left, nd->vm_start, nd->vm_end);
        }
        else if (node->right) {
            vm_area *nd = node->right;
            while (nd->left != NULL)
                nd = nd->left;
            node->vm_start = nd->vm_start;
            node->vm_end = nd->vm_end;
            node->container_info = nd->container_info;
            node->attribute = nd->attribute;
            node->protection = nd->protection;
            node->right = vmarea_delete(node->right, nd->vm_start, nd->vm_end);
        }
        else {
            free(node);
            return NULL;
        }
    }

    if (dis_height(node) == 2) {
        if(dis_height(node->left) == 1)
            node = fix_ll(node); //LL
        else
            node = fix_lr(node); //LR
    }
    else if (dis_height(node) == -2) {
        if (dis_height(node->right) == -1)
            node = fix_rr(node); //RR
        else
            node = fix_rl(node); //RL
    }

    update_height(node);
    return node;
}