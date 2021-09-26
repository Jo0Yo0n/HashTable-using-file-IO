#include "SearchEngine.h"

//� �ܰ迡�� �Լ��� �����Ű���� Ȯ���ϱ� ���� ����
int indexing = 1, searching = -1; 

//���� �ؽ�Ʈ ����
void storeTexts(char* fname, int fNum) { 
	char line[1000];
	char* token;
	int i = 0;
	FILE* fp;

	if ((fp = fopen(fname, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}

	while (fgets(line, 1000, fp) != NULL) { //���Ϸ� ���� �� ���徿 �о����
		token = strtok(line, " \n");
		while (token != NULL) {
			strcpy(original[fNum][i++], token);
			token = strtok(NULL, " \n");
		}
	}
	fclose(fp);
}

//���� �̸�, ��ȣ�� �Ű������� �ް� �� ���Ͽ� ����ִ� �ܾ���� �о� hashTable�� ����
void build_hash(char* fname, int fNum) {
	int   index_word = 0; // �ܾ��� �ε���(�ߺ� ����)
	char* token; //�ڸ� ���ڿ� ����
	char line[1000]; //�� ������ �����ϴ� ����
	FILE* fp;
	element item;

	if ((fp = fopen(fname, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}

	while (fgets(line, 1000, fp) != NULL) { //���Ϸκ��� �� ���徿 �о����
		token = strtok(line, TOKEN);
		while (token != NULL) {
			/*item ��� �ʱ�ȭ(ī��Ʈ, �ε���, ���� ��ȣ, �ܾ�)*/
			words[fNum]++; //�ܾ� �ڸ� ������ ���� ���� ��ü �ܾ� ī��Ʈ ����
			item.index[0] = index_word++; //�ش� ���Ͽ����� �ε��� ����
			item.count = 1; //default of count
			item.fileName = fNum;

			for (unsigned int i = 0; i < strlen(token); i++) { //�ҹ��ڷ� ��ȯ
				token[i] = tolower(token[i]);
			}
			//word ����
			strcpy(item.word, token);
			hash_insert(item, fNum); //hashTable�� ����
			token = strtok(NULL, TOKEN);
		}
	}
	fclose(fp);
	return;
}

//���� element�� ���� ��ȣ�� �´� hashTable�� ������
void hash_insert(element item, int fNum) { //linked list�� insert ����ð��� ����
	int h = hash(item.word); //�ܾ��� ù ���ĺ��� �������� ���� ����
	list_ptr temp, lead, trail = NULL;

	//�ߺ��� �ܾ� Ȯ��
	for (lead = hashTable[fNum][h]; lead; trail = lead, lead = lead->link) {
		indexing_comparision++; //��Ʈ�� �� Ƚ�� ����(���� ����)
		if (strcmp(lead->item.word, item.word) == 0) { //�ܾ� �ߺ�
			lead->item.count++; //�ߺ��� �ܾ��� count ���� ����
			lead->item.index[lead->item.count - 1] = item.index[0]; //�ߺ��� �ܾ��� �ε����� ����� �迭�� ����
			return;
		}
	}
	//�ش� ���Ͽ��� �ߺ��� �ܾ ���� ���
	if ((temp = (list_ptr)malloc(sizeof(list))) == NULL) {
		printf("Malloc fail\n");
		exit(1);
	}
	//���� ��ü���� �ߺ��Ǵ� �ܾ �ִ��� Ȯ��
	for (int i = 1; i <= fNum; i++) {
		list_ptr ptr;
		ptr = chain_search(item.word, i, indexing);
		if (ptr != NULL) { //���� ��ü���� �ش� �ܾ �ִ� ���
			goto EXIT; //�ٷ� Ž���� ����
		}
		//���� ��ü�� �ش� �ܾ ���� ���
		else if (ptr == NULL && i == fNum) {
			indexed_words++;
		}
	}

EXIT:
	//�ؽ� ���̺� ���ο� �ܾ� �߰�
	temp->item = item;
	temp->link = NULL;
	if (trail) trail->link = temp; //���� ����Ʈ�� �������� ��� ����
	else hashTable[fNum][h] = temp; //hashTable[fNum][h]�� ����ִ� ���
	return;
}

int hash(char* word) { //0~25 �� ��ȯ
	return word[0] - 'a';
}

//Chaining�� �̿��� ��带 ã�� �ּҸ� ����
list_ptr chain_search(char* word, int fNum, int flag) { //hashing�� search ���� �ð��� ����
	int i = hash(word);
	list_ptr ptr;

	for (ptr = hashTable[fNum][i]; ptr; ptr = ptr->link) {
		//�񱳿��� Ƚ�� ����
		if (flag == 1) indexing_comparision++;
		else if (flag == -1) searching_comparision++;

		if (strcmp(ptr->item.word, word) == 0) { //���̺� ã���� �ϴ� �ܾ� �߰�
			return ptr;
		}
	}
	return NULL;
}

//��ü �ؽ�Ʈ ���Ͽ��� �Ű������� ���� �ܾ ���� ��带 ã�� �迭�� �ְ� ���� (insertion sort)
void search_file(char* word) {
	list_ptr find[FILE_NUM] = { NULL, }; //�� ���ϸ��� ã�� �ܾ��� �ּҸ� �����ϴ� �迭
	list_ptr node[FILE_NUM] = { NULL, }; //ã�� ����� �ּҸ� ��� �����ϱ� ���� �迭
	list_ptr next;
	int i, j, k = 0;
	
	//��ü ������ ���� ã�� �ܾ �ִ� ����� �ּҸ� �迭�� ����
	for (int i = 1; i < FILE_NUM; i++) { 
		find[i] = chain_search(word, i, searching);
		if (find[i] != NULL) {
			node[k++] = find[i]; //�ش� ���Ͽ� ã�� �ܾ��� ��尡 ������ node �迭�� �ű�
		}
	}

	for (i = 1; i < k; i++) { //count�� ũ�⿡ ���� �������� ����(insertion sort)
		next = node[i];
		if (node[i]) {
			for (j = i - 1; j >= 0 && (next->item.count > node[j]->item.count); j--) {
				node[j + 1] = node[j];
			}
			node[j + 1] = next;
		}
	}

	//���ĵ� ��� �迭�� ���
	for (int k = 0; node[k]; k++) {
		print_file(node[k]);
	}

}

//�Ű������� ���� ��� �迭�� ����� �����͸� ���ǿ� �°� ���
void print_file(list_ptr node) {
	int numCount = node->item.count; //�ش� ���Ͽ��� ã�� �ܾ ���Ե� ����
	int fNum = node->item.fileName; //���� �ѹ�
	char* word = node->item.word; //�ܾ�
	
	//���ϸ� ���
	printf("doc");
	if (fNum > 0 && fNum < 10) printf("00");
	else if (fNum > 9 && fNum < 100) printf("0");
	printf("%d.txt(%s: %d)\n", fNum, word, numCount);

	//�� �ڷ� 3���� �ܾ� ���
	for (int i = 0; i < numCount; i++) {
		int j = node->item.index[i] - 3;
		for (j; j < 0; j++);
		int k = node->item.index[i] + 3;
		for (k; k >= words[fNum]; k--);

		if (j != 0) printf("...");
		for (j; j <= k; j++) {
			printf("%s ", original[fNum][j]);
		}
		if (j != words[fNum] - 1) printf("...\n");
	}
	printf("\n");
}