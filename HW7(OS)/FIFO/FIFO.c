#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAME_SIZE 3
#define REF_STRING_LENGTH 20

// 노드 구조체 정의
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 큐 구조체 정의
typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

// 큐 초기화
void initialize_queue(Queue* q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

// 큐가 비어있는지 확인
bool is_empty(Queue* q) {
    return q->size == 0;
}

// 큐가 가득 찼는지 확인
bool is_full(Queue* q) {
    return q->size == FRAME_SIZE;
}

// 큐에 삽입
void enqueue(Queue* q, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;
    if (is_empty(q)) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    if (is_full(q)) {
        Node* temp = q->front;
        q->front = q->front->next;
        free(temp);
    } else {
        q->size++;
    }
}

// 큐에 특정 값이 존재하는지 확인
bool contains(Queue* q, int value) {
    Node* current = q->front;
    while (current != NULL) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// 큐의 현재 상태 출력
void print_queue_state(Queue* q, int time) {
    Node* current = q->front;
    for (int i = 0; i < FRAME_SIZE; i++) {
        if (current != NULL) {
            printf("%d ", current->data);
            current = current->next;
        } else {
            printf("0 ");
        }
    }
    printf(" | 시간: %d\n", time);
}

// 큐 메모리 해제
void free_queue(Queue* q) {
    Node* current = q->front;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    q->front = q->rear = NULL;
    q->size = 0;
}

int main() {
    int reference_string[REF_STRING_LENGTH] = {1, 2, 3, 2, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3, 6, 1, 2, 4, 3};
    Queue frames;
    initialize_queue(&frames);
    int page_faults[REF_STRING_LENGTH];
    int page_fault_count = 0;

    for (int i = 0; i < REF_STRING_LENGTH; i++) {
        int current_page = reference_string[i];

        if (!contains(&frames, current_page)) {
            // 페이지 부재 발생
            page_faults[page_fault_count++] = i;
            enqueue(&frames, current_page);
        }

        print_queue_state(&frames, i);
    }

    printf("\n페이지 부재가 발생한 인덱스: ");
    for (int i = 0; i < page_fault_count; i++) {
        printf("%d ", page_faults[i]);
    }
    printf("\n");

    // 큐 메모리 해제
    free_queue(&frames);

    return 0;
}
