/*
 * queue.c
 *
 *  Created on: 2018/02/22
 *      Author: USER
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int QueueAlloc(Queue *q, int max){ /*— キュ ー の初期化 —*/
q->front=0;
q->rear=0;
q->num=0;
q->max=max;
	if ((q->que = (unsigned char *)calloc(max, sizeof(unsigned char))) == NULL) {
		q->max = 0;						/* 配列の確保に失敗 */
		return (-1);
	}
	return 0;
}
void QueueFree(Queue *q){ /*— キュ ー の後始末 —*/
	if (q->que != NULL) {
		free(q->que);
		q->max = q->num = q->front = q->rear = 0;
	}
}
int QueueEnque(Queue *q, unsigned char x){ /*— キュ ー にデー タ をエ ン キュー*/
	if(QueueIsFull(q)) return -1;
	*(q->que+q->rear)=x;
	q->rear++;
	q->num++;
	if(q->rear==q->max) q->rear=0;
	return 0;
}
int QueueDeque(Queue *q, unsigned char *x){ /*— キュ ー から デー タをデキュー*/
	if(QueueIsEmpty(q)) return -1;
	*x=*(q->que+q->front);
	q->front++;
	q->num--;
	if(q->front==q->max) q->front=0;
	return 0;
}
int QueueEmpty(Queue *q){ /*キューを空にする*/
	q->front=0;
	q->rear=0;
	q->num=0;
	return 0;
}
int QueueSize(const Queue *q){ /*— キュ ー の大きさ —*/
	return q->max;
}
int QueueNo(const Queue *q){ /*— キュ ー に蓄えら れて いるデータ数*/
	return q->num;
}
int QueueIsEmpty(const Queue *q){ /*— キュ ー は空か —*/
	return (q->num<=0);
}
int QueueIsFull(const Queue *q){ /*— キュ ー は満杯か —*/
	return (q->num>=q->max);
}

