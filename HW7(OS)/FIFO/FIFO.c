#include <stdio.h>
#include <stdbool.h>

#define FRAME_SIZE 3
#define REF_STRING_LENGTH 20

typedef struct {
    int items[FRAME_SIZE];
    int front, rear;
    int size;
} Queue;

// 큐 초기화
void initialize_queue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

// 큐가 비어있는지 확인
bool is_empty(Queue *q) {
    return q->size == 0;
}

// 큐가 가득 찼는지 확인
bool is_full(Queue *q) {
    return q->size == FRAME_SIZE;
}

// 큐에 삽입
void enqueue(Queue *q, int value) {
    if (is_full(q)) {
        q->front = (q->front + 1) % FRAME_SIZE; // 가장 오래된 프레임 제거
    } else {
        q->size++;
    }
    q->rear = (q->rear + 1) % FRAME_SIZE;
    q->items[q->rear] = value;
}

// 큐에 특정 값이 존재하는지 확인
bool contains(Queue *q, int value) {
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % FRAME_SIZE;
        if (q->items[index] == value) {
            return true;
        }
    }
    return false;
}

// 큐의 현재 상태 출력
void print_queue_state(Queue *q, int time) {
    for (int i = 0; i < FRAME_SIZE; i++) {
        if (i < q->size) {
            int index = (q->front + i) % FRAME_SIZE;
            printf("%d ", q->items[index]);
        } else {
            printf("0 ");
        }
    }
    printf(" | 시간: %d\n", time);
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

    return 0;
}

