/*
 * queue.h
 *
 *  Created on: 2018/02/22
 *      Author: USER
 */

#ifndef QUEUE_H_
#define QUEUE_H_


/* キューのデータ構造と関数プロトタイプ */

///*--- キューを実現する構造体 ---*/
//
typedef struct {
  int max;   /* キューのサイズ */
  int num;   /* 現在の要素数 */
  int front; /* 先頭要素カーソル */
  int rear;  /* 末尾要素カーソル */
  unsigned char *que;  /* キュー（の先頭要素へのポインタ） */
} Queue;
///* キューの操作を実現する関数*/
//
int QueueAlloc(Queue *q, int max);  /*--- キューの初期化 ---*/
void QueueFree(Queue *q);           /*--- キューの後始末 ---*/
int QueueEnque(Queue *q, unsigned char x);    /*--- キューにデータをエンキュー ---*/
int QueueDeque(Queue *q, unsigned char *x);   /*--- キューからデータをデキュー ---*/
int QueueEmpty(Queue *q);			/*--- キューを空にする ---*/
int QueueSize(const Queue *q);      /*--- キューの大きさ ---*/
int QueueNo(const Queue *q);        /*--- キューに蓄えられているデータ数 ---*/
int QueueIsEmpty(const Queue *q);   /*--- キューは空か ---*/
int QueueIsFull(const Queue *q);    /*--- キューは満杯か ---*/

#endif /* QUEUE_H_ */
