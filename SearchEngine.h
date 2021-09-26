#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define TOKEN " \n\t\"\',.?[]0123456789" //��ū�� �������� �ܾ �и�
#define TABLE_SIZE 26 //���ĺ� ������ŭ ���� ����
#define WORD_SIZE 30//�� �ܾ��� ũ��
#define FILE_NUM 100 + 1 //������ ���� (�ε����� 1���� �ϱ� ���� +1)
#define WORD_NUM 50 //�� ���Ͽ� �����ϴ� �ܾ��� ����

typedef struct { //��忡 ����Ǵ� ������: �ܾ�, ���� �̸�, �ε���, �ߺ� Ƚ��
	char word[WORD_SIZE]; //�ܾ� ����
	int fileName; //���� �̸� ����
	int count; //�ش� �ܾ �ߺ��� Ƚ���� ����
	int index[WORD_NUM]; //�ܾ� �ߺ��� �ߺ��Ǵ� �ܾ��� �ε����� �迭�� ����	
} element;
typedef struct list* list_ptr;
typedef struct list { //���Ḯ��Ʈ�� ���
	element item;
	list_ptr link;
} list;
list_ptr hashTable[FILE_NUM][TABLE_SIZE]; //�ؽ����̺�

extern int indexing_comparision; //���� ���������� ��Ƚ�� ����
extern int searching_comparision; //�˻� ���������� ��Ƚ�� ����
extern int indexing, searching; //� �ܰ迡�� �Լ��� �����Ű���� Ȯ���ϱ� ���� ����
int words[FILE_NUM]; //�� ������ ��ü �ܾ� �� ���� (�ߺ� ����)
int indexed_words; //��ü ������ ���ε� �ܾ� �� ���� (�ߺ� ����)
char original[FILE_NUM][WORD_NUM][WORD_SIZE]; //���� �ؽ�Ʈ ������ �ܾ ������ �迭


void storeTexts(char* fname, int fNum); //�ؽ�Ʈ ������ �߶� ����

//���� ���� �Լ�
void build_hash(char* fname, int i); //hash ����
void hash_insert(element item, int fNum); //item ����, �ߺ��̸� false ����
int hash(char* word); //hash �Լ�

//��� ��� �Լ�
//void print_search(char* search);
//�ܾ ���� ��带 ã�� �ش� ����� �ּҰ��� ��ȯ
list_ptr chain_search(char* word, int fNum, int flag);
void search_file(char* word);
void print_file(list_ptr file);