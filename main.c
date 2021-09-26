#include "SearchEngine.h"

int indexing_comparision = 0; //색인 과정에서의 비교횟수 저장
int searching_comparision = 0; //검색 과정에서의 비교횟수 저장


int main() {
	char fileName[15], temp[10], num[4];
	int sum = 0;

	//각 파일의 단어들을 hash 자료구조로 저장 (파일 개수만큼 반복)
	for (int fNum = 1; fNum < FILE_NUM; fNum++) {
		strcpy(fileName, "doc");
		_itoa(fNum, num, 10);
		if (fNum > 0 && fNum < 10) { //한 자리 수
			strcpy(temp, "00");
			strcat(temp, num); //파일 인덱스 앞에 00 붙임
		}
		else if (fNum > 9 && fNum < 100) { // 두 자리 수
			strcpy(temp, "0");
			strcat(temp, num); //파일 인덱스 앞에 0 붙임
		}
		else strcpy(temp, num); //100번째 파일(doc100.txt)
		strcat(temp, ".txt"); //파일 인덱스 뒤에 .txt 붙임
		strcat(fileName, temp); //doc000.txt
		storeTexts(fileName, fNum); //원본 텍스트 파일 저장
		build_hash(fileName, fNum); //hash 생성
	}

	for (int i = 1; i < FILE_NUM; i++) { //각 파일에 있는 총 단어의 수의 합
		sum += words[i];
	}

	printf("색인을 마쳤습니다.\n");
	printf("Total number of documents: %d\n", FILE_NUM - 1);
	printf("Total number of indexed words: %d\n", indexed_words);
	printf("Total number of comparison: %d\n", indexing_comparision);
	printf("Total number of words: %d\n", sum); //각 파일에 있는 총 단어의 수의 합

	char userInput[WORD_NUM];
	printf("\n검색할 단어를 입력하세요>> ");
	scanf("%s", userInput);

	search_file(userInput);

	printf("Total number of comparision: %d\n", searching_comparision);

	return;
}