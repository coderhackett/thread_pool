#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	20

struct task
{
	void *(*task)(void *arg);	/* 任务需要执行的函数 */
	void *arg;					/* 执行函数的参数 */
	struct task *next;			/* 下一个任务的地址 */
};

typedef struct thread_pool
{
	pthread_mutex_t lock;
	pthread_cond_t  cond;
	struct task *task_list;	/*链表结构，线程池中所有等待任务*/
	pthread_t *tids;		/*存放线程id的指针*/
	unsigned waiting_tasks; /*当前等待的任务数*/
	unsigned active_threads;/*线程池中线程数目*/
	bool shutdown;			/*是否销毁线程池*/
}thread_pool;

/*
 * @description: 初始化线程池
 * @param {thread_pool*} pool:线程池结构体指针 {unsigned int} max_thread_num: 创建几个线程
 * @return: false 失败 true 成功
 */
bool init_pool(thread_pool *pool,
          unsigned int threads_number);
/*
 * @description: 向线程池添加任务
 * @param {thread_pool*} pool:线程池结构体指针 {void *(void *arg)} (*task): 线程的回调函数 {void *} arg: 传入的参数
 * @return: false 失败 true 成功
 */
bool add_task(thread_pool *pool,
         void *(*task)(void *arg),
         void *arg);
/*
 * @description: 向线程池添加线程
 * @param {thread_pool*} pool:线程池结构体指针 {unsigned int} additional_threads: 添加的线程数
 * @return: 返回成功的线程数
 */
int add_thread(thread_pool *pool,
           unsigned int additional_threads_number);
/*
 * @description: 线程池里取消线程
 * @param {thread_pool*} pool:线程池结构体指针 {nsigned int} removing_threads: 取消的线程数
 * @return: 失败返回-1
 */
int remove_thread(thread_pool *pool,
              unsigned int removing_threads_number);

/*
 * @description: 销毁线程池
 * @param {thread_pool*} pool:线程池结构体指针
 * @return: 成功返回true
 */
bool destroy_pool(thread_pool *pool);
/*
 * @description: 线程的回调处理函数
 * @param  {void *} arg: 传入的参数
 * @return: 无
 */
void *routine(void *arg);

#endif