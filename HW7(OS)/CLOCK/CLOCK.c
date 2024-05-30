#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 20
#define FRAME_SIZE 3

int reference_string[STRING_SIZE] = {1, 2, 3, 2, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3, 6, 1, 2, 4, 3};
int page_fault_indices[STRING_SIZE];
int page_fault_count = 0;

// 노드 구조체 정의
typedef struct Node {
    int page;
    int use_bit;
    struct Node* next;
} Node;

Node* head = NULL;
Node* tail = NULL;
Node* clock_pointer = NULL;

// 노드 초기화
Node* create_node(int page) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->page = page;
    new_node->use_bit = 1;
    new_node->next = NULL;
    return new_node;
}

// 프레임 초기화
void initialize_frames() {
    head = create_node(0);
    tail = head;
    for (int i = 1; i < FRAME_SIZE; i++) {
        tail->next = create_node(0);
        tail = tail->next;
    }
    tail->next = head;
    clock_pointer = head;
}

// 프레임에 페이지가 있는지 확인
int search_in_frame(int page) {
    Node* current = head;
    do {
        if (current->page == page) {
            return 1;
        }
        current = current->next;
    } while (current != head);
    return 0;
}

// 페이지 교체
void replace_page(int page) {
    while (clock_pointer->use_bit == 1) {
        clock_pointer->use_bit = 0;
        clock_pointer = clock_pointer->next;
    }
    clock_pointer->page = page;
    clock_pointer->use_bit = 1;
    clock_pointer = clock_pointer->next;
}

// 현재 프레임 상태 출력
void print_frame_status(int time) {
    Node* current = head;
    printf("시간 %2d: ", time);
    do {
        printf("%2d ", current->page);
        current = current->next;
    } while (current != head);
    printf("\n");
}

// Clock 알고리즘 구현
void clock_algorithm() {
    initialize_frames();
    for (int i = 0; i < STRING_SIZE; i++) {
        int page = reference_string[i];
        if (!search_in_frame(page)) { // 페이지 부재 발생
            page_fault_indices[page_fault_count++] = i;
            replace_page(page);
        } else {
            Node* current = head;
            do {
                if (current->page == page) {
                    current->use_bit = 1;
                    break;
                }
                current = current->next;
            } while (current != head);
        }
        print_frame_status(i);
    }
}

// 페이지 부재가 발생한 인덱스를 출력하는 함수
void print_page_fault_indices() {
    printf("페이지 부재가 발생한 인덱스: ");
    for (int i = 0; i < page_fault_count; i++) {
        printf("%d ", page_fault_indices[i]);
    }
    printf("\n");
}

int main() {
    clock_algorithm();
    print_page_fault_indices();
    return 0;
}
