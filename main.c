#include "SearchEngine.h"

int indexing_comparision = 0; //���� ���������� ��Ƚ�� ����
int searching_comparision = 0; //�˻� ���������� ��Ƚ�� ����


int main() {
	char fileName[15], temp[10], num[4];
	int sum = 0;

	//�� ������ �ܾ���� hash �ڷᱸ���� ���� (���� ������ŭ �ݺ�)
	for (int fNum = 1; fNum < FILE_NUM; fNum++) {
		strcpy(fileName, "doc");
		_itoa(fNum, num, 10);
		if (fNum > 0 && fNum < 10) { //�� �ڸ� ��
			strcpy(temp, "00");
			strcat(temp, num); //���� �ε��� �տ� 00 ����
		}
		else if (fNum > 9 && fNum < 100) { // �� �ڸ� ��
			strcpy(temp, "0");
			strcat(temp, num); //���� �ε��� �տ� 0 ����
		}
		else strcpy(temp, num); //100��° ����(doc100.txt)
		strcat(temp, ".txt"); //���� �ε��� �ڿ� .txt ����
		strcat(fileName, temp); //doc000.txt
		storeTexts(fileName, fNum); //���� �ؽ�Ʈ ���� ����
		build_hash(fileName, fNum); //hash ����
	}

	for (int i = 1; i < FILE_NUM; i++) { //�� ���Ͽ� �ִ� �� �ܾ��� ���� ��
		sum += words[i];
	}

	printf("������ ���ƽ��ϴ�.\n");
	printf("Total number of documents: %d\n", FILE_NUM - 1);
	printf("Total number of indexed words: %d\n", indexed_words);
	printf("Total number of comparison: %d\n", indexing_comparision);
	printf("Total number of words: %d\n", sum); //�� ���Ͽ� �ִ� �� �ܾ��� ���� ��

	char userInput[WORD_NUM];
	printf("\n�˻��� �ܾ �Է��ϼ���>> ");
	scanf("%s", userInput);

	search_file(userInput);

	printf("Total number of comparision: %d\n", searching_comparision);

	return;
}