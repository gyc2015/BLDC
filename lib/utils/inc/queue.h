#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

typedef struct {
    uint8 *buffer;
    int head;
    int tail;
	int size;
} Queue_T;

/*
 * init_queue - ��ʼ������
 *
 * @q: Ŀ�����
 * @buf: ���л���
 * @size: ���г��ȣ�Ӧ����ʵ�ʻ��泤��С1
 */
void init_queue(Queue_T *q, uint8 *buf, int size);
/*
 * get_queue_size - ���г���
 *
 * @q: Ŀ�����
 */
int get_queue_size(Queue_T *q);
/*
 * is_queue_empty - ��ѯ�����Ƿ�Ϊ��
 *
 * @q: Ŀ�����
 * return: TRUE-��
 */
bool is_queue_empty(Queue_T *q);
/*
 * is_queue_full - ��ѯ�����Ƿ�full
 *
 * @q: Ŀ�����
 * return: TRUE-��
 */
bool is_queue_full(Queue_T *q);
/*
 * enqueue - �����ݲ����β
 *
 * @q: Ŀ�����
 * @data: ����
 * return: TRUE-�ɹ��������
 */
bool enqueue(Queue_T *q, uint8 data);
/*
 * dequeue - �������ݳ���
 *
 * @q: Ŀ�����
 * @re[OUT]: �������
 * return: TRUE-�ɹ�����
 */
bool dequeue(Queue_T *q, uint8 *re);

void clear_queue(Queue_T *q);
#endif
