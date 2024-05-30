#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAME_SIZE 3
#define REFERENCE_LENGTH 20

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

// Queue 초기화 함수
void initQueue(Queue* q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

// Queue가 비어 있는지 확인하는 함수
bool isEmpty(Queue* q) {
    return q->size == 0;
}

// Queue에 요소를 추가하는 함수
void enqueue(Queue* q, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

// Queue에서 요소를 제거하는 함수
int dequeue(Queue* q) {
    if (isEmpty(q)) return -1;
    int data = q->front->data;
    Node* temp = q->front;
    q->front = q->front->next;
    free(temp);
    q->size--;
    return data;
}

// Queue에 특정 요소가 있는지 확인하는 함수
bool contains(Queue* q, int data) {
    Node* current = q->front;
    while (current) {
        if (current->data == data) return true;
        current = current->next;
    }
    return false;
}

// Queue의 특정 요소를 특정 위치에 삽입하는 함수
void insertAt(Queue* q, int data, int index) {
    if (index >= q->size) {
        enqueue(q, data);
        return;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    if (index == 0) {
        newNode->next = q->front;
        q->front = newNode;
    } else {
        Node* current = q->front;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    q->size++;
}

// 교체할 최적의 페이지를 찾는 함수
int findOptimalIndex(Queue* q, int refStr[], int currentIndex) {
    int farthestIndex = currentIndex;
    int indexToReplace = -1;
    Node* frames[FRAME_SIZE];
    Node* current = q->front;
    for (int i = 0; i < FRAME_SIZE; i++) {
        frames[i] = current;
        current = current->next;
    }
    for (int i = 0; i < FRAME_SIZE; i++) {
        int j;
        for (j = currentIndex; j < REFERENCE_LENGTH; j++) {
            if (frames[i]->data == refStr[j]) {
                if (j > farthestIndex) {
                    farthestIndex = j;
                    indexToReplace = i;
                }
                break;
            }
        }
        // 페이지가 이후에 없으면 이 페이지를 교체
        if (j == REFERENCE_LENGTH) return i;
    }
    return (indexToReplace == -1) ? 0 : indexToReplace;
}

int main() {
    int refStr[REFERENCE_LENGTH] = {1, 2, 3, 2, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3, 6, 1, 2, 4, 3};
    Queue frames;
    initQueue(&frames);
    int frameStates[REFERENCE_LENGTH][FRAME_SIZE] = {0};
    int pageFaultIndices[REFERENCE_LENGTH] = {0};
    int pageFaults = 0;

    for (int i = 0; i < REFERENCE_LENGTH; i++) {
        if (!contains(&frames, refStr[i])) {
            if (frames.size < FRAME_SIZE) {
                enqueue(&frames, refStr[i]);
            } else {
                int indexToReplace = findOptimalIndex(&frames, refStr, i + 1);
                Node* temp = frames.front;
                for (int j = 0; j < indexToReplace; j++) {
                    temp = temp->next;
                }
                temp->data = refStr[i];
            }
            pageFaultIndices[pageFaults++] = i;
        }
        Node* current = frames.front;
        for (int j = 0; j < FRAME_SIZE; j++) {
            frameStates[i][j] = current ? current->data : 0;
            if (current) current = current->next;
        }
    }

    // 프레임 상태 출력
    printf("프레임 상태:\n");
    for (int i = 0; i < REFERENCE_LENGTH; i++) {
        for (int j = 0; j < FRAME_SIZE; j++) {
            printf("%d ", frameStates[i][j]);
        }
        printf("\n");
    }

    // 페이지 부재 인덱스 출력
    printf("페이지 부재 인덱스:\n");
    for (int i = 0; i < pageFaults; i++) {
        printf("%d ", pageFaultIndices[i]);
    }
    printf("\n");

    return 0;
}
