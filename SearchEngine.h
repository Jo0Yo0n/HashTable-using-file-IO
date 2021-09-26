#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#define TOKEN " \n\t\"\',.?[]0123456789" //토큰을 기준으로 단어를 분리
#define TABLE_SIZE 26 //알파벳 개수만큼 버켓 생성
#define WORD_SIZE 30//한 단어의 크기
#define FILE_NUM 100 + 1 //파일의 개수 (인덱싱을 1부터 하기 위해 +1)
#define WORD_NUM 50 //한 파일에 존재하는 단어의 개수

typedef struct { //노드에 저장되는 데이터: 단어, 파일 이름, 인덱스, 중복 횟수
	char word[WORD_SIZE]; //단어 저장
	int fileName; //파일 이름 저장
	int count; //해당 단어가 중복된 횟수를 저장
	int index[WORD_NUM]; //단어 중복시 중복되는 단어의 인덱스를 배열에 저장	
} element;
typedef struct list* list_ptr;
typedef struct list { //연결리스트의 노드
	element item;
	list_ptr link;
} list;
list_ptr hashTable[FILE_NUM][TABLE_SIZE]; //해시테이블

extern int indexing_comparision; //색인 과정에서의 비교횟수 저장
extern int searching_comparision; //검색 과정에서의 비교횟수 저장
extern int indexing, searching; //어떤 단계에서 함수를 실행시키는지 확인하기 위한 변수
int words[FILE_NUM]; //각 파일의 전체 단어 수 저장 (중복 포함)
int indexed_words; //전체 파일의 색인된 단어 수 저장 (중복 제외)
char original[FILE_NUM][WORD_NUM][WORD_SIZE]; //원본 텍스트 파일의 단어를 저장할 배열


void storeTexts(char* fname, int fNum); //텍스트 파일을 잘라서 저장

//색인 과정 함수
void build_hash(char* fname, int i); //hash 생성
void hash_insert(element item, int fNum); //item 삽입, 중복이면 false 리턴
int hash(char* word); //hash 함수

//결과 출력 함수
//void print_search(char* search);
//단어가 같은 노드를 찾고 해당 노드의 주소값을 반환
list_ptr chain_search(char* word, int fNum, int flag);
void search_file(char* word);
void print_file(list_ptr file);