#include <stdio.h>
#include <stdlib.h>

#define FRAME_SIZE 3
#define STRING_SIZE 20

// 노드 정의
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 연결 리스트 큐 정의
typedef struct Queue {
    Node* front;
    Node* rear;
    int size;
} Queue;

// 큐 초기화
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// 큐가 비어 있는지 확인
int isEmpty(Queue* queue) {
    return queue->rear == NULL;
}

// 큐에 요소 추가
void enqueue(Queue* queue, int value) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = value;
    temp->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = temp;
    } else {
        queue->rear->next = temp;
        queue->rear = temp;
    }
    queue->size++;
}

// 큐에서 요소 제거
void dequeue(Queue* queue) {
    if (isEmpty(queue)) return;
    Node* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    free(temp);
    queue->size--;
}

// 큐에서 특정 값을 찾고 위치를 반환
Node* findAndMoveToFront(Queue* queue, int value) {
    Node* temp = queue->front;
    Node* prev = NULL;
    while (temp != NULL) {
        if (temp->data == value) {
            if (prev != NULL) {
                prev->next = temp->next;
                temp->next = queue->front;
                queue->front = temp;
                if (temp == queue->rear) queue->rear = prev;
            }
            return temp;
        }
        prev = temp;
        temp = temp->next;
    }
    return NULL;
}

// 프레임 상태 출력
void printFrames(int frames[FRAME_SIZE][STRING_SIZE], int time) {
    for (int i = 0; i < FRAME_SIZE; i++) {
        for (int j = 0; j < time; j++) {
            if (frames[i][j] == -1)
                printf("0 ");
            else
                printf("%d ", frames[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int referenceString[STRING_SIZE] = {1, 2, 3, 3, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3, 6, 1, 2, 4, 3};
    int frames[FRAME_SIZE][STRING_SIZE];
    int pageFaults = 0;

    // 프레임 초기화
    for (int i = 0; i < FRAME_SIZE; i++) {
        for (int j = 0; j < STRING_SIZE; j++) {
            frames[i][j] = -1;
        }
    }

    Queue* queue = createQueue();

    for (int i = 0; i < STRING_SIZE; i++) {
        int page = referenceString[i];

        // 큐에서 페이지를 찾고 맨 앞으로 이동
        Node* foundNode = findAndMoveToFront(queue, page);

        // 페이지가 큐에 없는 경우 페이지 부재 발생
        if (foundNode == NULL) {
            pageFaults++;
            if (queue->size == FRAME_SIZE) {
                dequeue(queue);
            }
            enqueue(queue, page);
            printf("페이지 부재 발생 인덱스: %d\n", i);
        }

        // 각 시간 단계에서 프레임 상태 업데이트
        Node* temp = queue->front;
        for (int j = 0; j < FRAME_SIZE; j++) {
            if (temp != NULL) {
                frames[j][i] = temp->data;
                temp = temp->next;
            } else {
                frames[j][i] = -1;
            }
        }
    }

    // 최종 프레임 행렬 출력
    printf("각 시간 단계에서의 프레임 상태:\n");
    printFrames(frames, STRING_SIZE);

    return 0;
}
