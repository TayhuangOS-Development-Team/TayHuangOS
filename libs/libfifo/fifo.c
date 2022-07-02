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
 * fifo.c
 *
 * fifo
 *
 */



#include <fifo.h>

#include <string.h>

void create_fifo(fifo_struct *fifo, void *buffer, int size) {
    fifo->rd_pos = fifo->wr_pos = 0;
    fifo->len = 0;
    fifo->buffer = buffer;
    fifo->size = size;
}

bool fifo_empty(fifo_struct *fifo) {
    return fifo->len == 0;
}

bool fifo_full(fifo_struct *fifo) {
    return fifo->len == fifo->size;
}

static inline void __read_fifo(fifo_struct *fifo, void *data, int len) {
    if (fifo->rd_pos + len > fifo->size) {
        int size1 = fifo->size - fifo->rd_pos;
        memcpy(data, fifo->buffer + fifo->rd_pos, size1);
        memcpy(data + size1, fifo->buffer, len - size1);
    }
    else {
        memcpy(data, fifo->buffer + fifo->rd_pos, len);
    }
}

bool read_fifo(fifo_struct *fifo, void *data, int len) {
    if (fifo_empty(fifo) || (fifo->len < len)) {
        return false;
    }
    if (len == -1) {
        len = fifo->len;
    }

    __read_fifo(fifo, data, len);

    fifo->rd_pos = (fifo->rd_pos + len) % fifo->size;
    fifo->len -= len;
    return true;
}

static inline void __write_fifo(fifo_struct *fifo, void *data, int size) {
    if (fifo->wr_pos + size > fifo->size) {
        int size1 = fifo->size - fifo->wr_pos;
        memcpy(fifo->buffer + fifo->wr_pos, data, size1);
        memcpy(fifo->buffer, data + size1, size - size1);
    }
    else {
        memcpy(fifo->buffer + fifo->wr_pos, data, size);
    }
}

bool write_fifo(fifo_struct *fifo, void *data, int size) {
    if (fifo_full(fifo) || ((fifo->size - fifo->len) < size)) {
        return false;
    }

    __write_fifo(fifo, data, size);

    fifo->wr_pos = (fifo->wr_pos + size) % fifo->size;
    fifo->len += size;
    return true;
}

int get_fifo_size(fifo_struct *fifo) {
    return fifo->size;
}

int get_fifo_len(fifo_struct *fifo) {
    return fifo->len;
}

// Using&Test Example
//
//int data[] = {0, 1, 2, 3, 4, 5, 6, 7};
//
//int main(int argc, const char **argv) {
//    void *buffer = malloc(250);
//    fifo_struct fifo;
//    create_fifo(&fifo, buffer, 250);
//	
//    for (int j = 0 ; j < 16; j ++) {
//        std::cout << "-----------------------------\n"; 
//        write_fifo(&fifo, data, sizeof(data));
//        for (int i = 0 ; i < 8 ; i ++) {
//            int num = 0;
//            bool status = read_fifo(&fifo, &num, sizeof(int));
//            std::cout << (status ? "(Success:" : "(Failed:") << num << ") \n";
//        }
//
//        for (int i = 0 ; i < 8 ; i ++) {
//            data[i] /= (j + 1);
//            data[i] *= (j + 2);
//        }
//    }
//
//    free(buffer);
//    return 0;
//}