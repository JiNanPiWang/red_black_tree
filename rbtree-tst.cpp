/*
 * =============================================================================
 *
 *       Filename:  rbtree-tst.c
 *
 *    Description:  rbtree testcase.
 *
 *        Created:  09/02/2012 11:39:34 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/time.h>


struct mynode {
  	struct rb_node node;
  	char *string; // string 成员则包含了一个用于比较和排序树中节点的键值
};

struct rb_root mytree = RB_ROOT;

struct mynode * my_search(struct rb_root *root, char *string)
{
  	struct rb_node *node = root->rb_node;

  	while (node) {
  		struct mynode *data = container_of(node, struct mynode, node);
		int result;

		result = strcmp(string, data->string);

		if (result < 0)
  			node = node->rb_left;
		else if (result > 0)
  			node = node->rb_right;
		else
  			return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, struct mynode *data)
{
  	struct rb_node **new = &(root->rb_node), *parent = NULL;

  	/* Figure out where to put new node */
  	while (*new) {
  		struct mynode *this = container_of(*new, struct mynode, node);
  		int result = strcmp(data->string, this->string);

		parent = *new;
  		if (result < 0)
  			new = &((*new)->rb_left);
  		else if (result > 0)
  			new = &((*new)->rb_right);
  		else
  			return 0;
  	}

  	/* Add new node and rebalance tree. */
  	rb_link_node(&data->node, parent, new);
  	rb_insert_color(&data->node, root);

	return 1;
}

void my_free(struct mynode *node)
{
	if (node != NULL) {
		if (node->string != NULL) {
			free(node->string);
			node->string = NULL;
		}
		free(node);
		node = NULL;
	}
}

#define NUM_NODES 50000000

int main()
{
//    struct mynode *mn[NUM_NODES];
    struct mynode **mn = malloc(NUM_NODES * sizeof(struct mynode*));

    struct timeval start, end;
    long seconds, useconds;
    double mtime;

	/* *insert */
	int i = 0;
	printf("insert node from 1 to NUM_NODES(50M): \n");
    long long totalMemoryUsed = 0; // 用于跟踪使用的总内存

    gettimeofday(&start, NULL); // 获取插入操作开始前的时间

	for (; i < NUM_NODES; i++) {
		mn[i] = (struct mynode *)malloc(sizeof(struct mynode));
		mn[i]->string = (char *)malloc(sizeof(char) * 4);
//		sprintf(mn[i]->string, "%d", i);
		my_insert(&mytree, mn[i]);
        totalMemoryUsed += sizeof(struct mynode) + sizeof(char) * 4; // 更新内存使用统计
    }

    gettimeofday(&end, NULL); // 获取插入操作完成后的时间
    // 计算插入操作所花费的时间
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    printf("Total memory used: %f MB\n", (double)totalMemoryUsed / 1024 / 1024);
    printf("Inserted all nodes in: %f milliseconds\n", mtime);

    // 在程序结束前释放所有节点
    for (i = 0; i < NUM_NODES; i++) {
        my_free(mn[i]);
    }
    free(mn);
	return 0;
}
