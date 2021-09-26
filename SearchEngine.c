#include "SearchEngine.h"

//어떤 단계에서 함수를 실행시키는지 확인하기 위한 변수
int indexing = 1, searching = -1; 

//원본 텍스트 저장
void storeTexts(char* fname, int fNum) { 
	char line[1000];
	char* token;
	int i = 0;
	FILE* fp;

	if ((fp = fopen(fname, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}

	while (fgets(line, 1000, fp) != NULL) { //파일로 부터 한 문장씩 읽어들임
		token = strtok(line, " \n");
		while (token != NULL) {
			strcpy(original[fNum][i++], token);
			token = strtok(NULL, " \n");
		}
	}
	fclose(fp);
}

//파일 이름, 번호를 매개변수로 받고 한 파일에 들어있는 단어들을 읽어 hashTable에 삽입
void build_hash(char* fname, int fNum) {
	int   index_word = 0; // 단어의 인덱스(중복 무관)
	char* token; //자른 문자열 저장
	char line[1000]; //한 문장을 저장하는 버퍼
	FILE* fp;
	element item;

	if ((fp = fopen(fname, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}

	while (fgets(line, 1000, fp) != NULL) { //파일로부터 한 문장씩 읽어들임
		token = strtok(line, TOKEN);
		while (token != NULL) {
			/*item 멤버 초기화(카운트, 인덱스, 파일 번호, 단어)*/
			words[fNum]++; //단어 자를 때마다 파일 내의 전체 단어 카운트 증가
			item.index[0] = index_word++; //해당 파일에서의 인덱스 대입
			item.count = 1; //default of count
			item.fileName = fNum;

			for (unsigned int i = 0; i < strlen(token); i++) { //소문자로 변환
				token[i] = tolower(token[i]);
			}
			//word 저장
			strcpy(item.word, token);
			hash_insert(item, fNum); //hashTable에 삽입
			token = strtok(NULL, TOKEN);
		}
	}
	fclose(fp);
	return;
}

//들어온 element를 파일 번호에 맞는 hashTable에 저장함
void hash_insert(element item, int fNum) { //linked list의 insert 실행시간을 따름
	int h = hash(item.word); //단어의 첫 알파벳을 기준으로 버켓 나눔
	list_ptr temp, lead, trail = NULL;

	//중복된 단어 확인
	for (lead = hashTable[fNum][h]; lead; trail = lead, lead = lead->link) {
		indexing_comparision++; //스트링 비교 횟수 증가(색인 과정)
		if (strcmp(lead->item.word, item.word) == 0) { //단어 중복
			lead->item.count++; //중복된 단어의 count 변수 증가
			lead->item.index[lead->item.count - 1] = item.index[0]; //중복된 단어의 인덱스를 노드의 배열에 저장
			return;
		}
	}
	//해당 파일에서 중복된 단어가 없는 경우
	if ((temp = (list_ptr)malloc(sizeof(list))) == NULL) {
		printf("Malloc fail\n");
		exit(1);
	}
	//파일 전체에서 중복되는 단어가 있는지 확인
	for (int i = 1; i <= fNum; i++) {
		list_ptr ptr;
		ptr = chain_search(item.word, i, indexing);
		if (ptr != NULL) { //파일 전체에서 해당 단어가 있는 경우
			goto EXIT; //바로 탐색을 멈춤
		}
		//파일 전체에 해당 단어가 없는 경우
		else if (ptr == NULL && i == fNum) {
			indexed_words++;
		}
	}

EXIT:
	//해쉬 테이블에 새로운 단어 추가
	temp->item = item;
	temp->link = NULL;
	if (trail) trail->link = temp; //연결 리스트의 마지막에 노드 삽입
	else hashTable[fNum][h] = temp; //hashTable[fNum][h]이 비어있는 경우
	return;
}

int hash(char* word) { //0~25 값 반환
	return word[0] - 'a';
}

//Chaining을 이용해 노드를 찾아 주소를 리턴
list_ptr chain_search(char* word, int fNum, int flag) { //hashing의 search 실행 시간을 따름
	int i = hash(word);
	list_ptr ptr;

	for (ptr = hashTable[fNum][i]; ptr; ptr = ptr->link) {
		//비교연산 횟수 증가
		if (flag == 1) indexing_comparision++;
		else if (flag == -1) searching_comparision++;

		if (strcmp(ptr->item.word, word) == 0) { //테이블에 찾고자 하는 단어 발견
			return ptr;
		}
	}
	return NULL;
}

//전체 텍스트 파일에서 매개변수로 들어온 단어를 갖는 노드를 찾아 배열에 넣고 정렬 (insertion sort)
void search_file(char* word) {
	list_ptr find[FILE_NUM] = { NULL, }; //각 파일마다 찾는 단어의 주소를 저장하는 배열
	list_ptr node[FILE_NUM] = { NULL, }; //찾은 노드의 주소를 담아 정렬하기 위한 배열
	list_ptr next;
	int i, j, k = 0;
	
	//전체 파일을 돌며 찾는 단어가 있는 노드의 주소를 배열에 저장
	for (int i = 1; i < FILE_NUM; i++) { 
		find[i] = chain_search(word, i, searching);
		if (find[i] != NULL) {
			node[k++] = find[i]; //해당 파일에 찾는 단어의 노드가 있으면 node 배열로 옮김
		}
	}

	for (i = 1; i < k; i++) { //count의 크기에 따라 내림차순 정렬(insertion sort)
		next = node[i];
		if (node[i]) {
			for (j = i - 1; j >= 0 && (next->item.count > node[j]->item.count); j--) {
				node[j + 1] = node[j];
			}
			node[j + 1] = next;
		}
	}

	//정렬된 노드 배열을 출력
	for (int k = 0; node[k]; k++) {
		print_file(node[k]);
	}

}

//매개변수로 들어온 노드 배열에 저장된 데이터를 조건에 맞게 출력
void print_file(list_ptr node) {
	int numCount = node->item.count; //해당 파일에서 찾는 단어가 포함된 개수
	int fNum = node->item.fileName; //파일 넘버
	char* word = node->item.word; //단어
	
	//파일명 출력
	printf("doc");
	if (fNum > 0 && fNum < 10) printf("00");
	else if (fNum > 9 && fNum < 100) printf("0");
	printf("%d.txt(%s: %d)\n", fNum, word, numCount);

	//앞 뒤로 3개의 단어 출력
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