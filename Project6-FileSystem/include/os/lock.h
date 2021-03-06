/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                                   Thread Lock
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_LOCK_H_
#define INCLUDE_LOCK_H_

#include <os/list.h>

#define BINSEM_OP_LOCK 0
#define BINSEM_OP_UNLOCK 1
typedef enum {
    UNLOCKED,
    LOCKED,
} lock_status_t;

typedef struct spin_lock
{
    volatile lock_status_t status;
} spin_lock_t;

typedef struct mutex_lock
{
    spin_lock_t lock;
    list_head block_queue;
} mutex_lock_t;

#define BIN_LOCKED 1
#define BIN_UNLOCKED 0

typedef struct sem {
    int id;
    int key;
    int status;
    list_head block_queue;
} sem_t;

typedef sem_t binsem_t;
typedef sem_t cond_t;
typedef sem_t barrier_t;

/* init lock */
void spin_lock_init(spin_lock_t *lock);
int spin_lock_try_acquire(spin_lock_t *lock);
void spin_lock_acquire(spin_lock_t *lock);
void spin_lock_release(spin_lock_t *lock);

void do_mutex_lock_init(mutex_lock_t *lock);
void do_mutex_lock_acquire(mutex_lock_t *lock);
void do_mutex_lock_release(mutex_lock_t *lock);
int kernel_semget(int key);
int kernel_binsemop(int binsem_id, int op);

int kernel_sem_wait(int sem_id, int lock_id);
int kernel_sem_signal(int sem_id);
int kernel_sem_broadcast(int sem_id);
int kernel_barrier_wait(int id, int count);
typedef int mailbox_t;
mailbox_t kernel_mbox_open(char *name);
void kernel_mbox_close(mailbox_t mailbox);
void kernel_mbox_send(mailbox_t mailbox, void *msg, int msg_length);
void kernel_mbox_recv(mailbox_t mailbox, void *msg, int msg_length);

//#include <mthread.h>
#define MSG_MAX_SIZE 512
#define MAX_MBOX_LENGTH (64)
#define MAX_MBOX 256
// TODO: please define mailbox_t;
// mailbox_t is just an id of kernel's mail box.
typedef struct mailbox
{ 
    int  id;
    char name[64];
    char buffer[MSG_MAX_SIZE];
    int  head, tail;
    int  cited;
    int  used_size;
    int  full_id;
    int  empty_id;
    int  lock_id;
}mbox_t;

#endif
