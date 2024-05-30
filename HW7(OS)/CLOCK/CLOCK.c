#include <stdio.h>
#include <stdlib.h>

#define FRAME_SIZE 3
#define STRING_SIZE 20

int reference_string[STRING_SIZE] = {1, 2, 3, 2, 1, 5, 2, 1, 6, 2, 5, 6, 3, 1, 3, 6, 1, 2, 4, 3};
int frame[FRAME_SIZE];
int use_bit[FRAME_SIZE];
int clock_pointer = 0;
int page_fault_indices[STRING_SIZE];
int page_fault_count = 0;

// 프레임을 초기화하는 함수
void initialize_frames() {
    for (int i = 0; i < FRAME_SIZE; i++) {
        frame[i] = 0;
        use_bit[i] = 0;
    }
}

// 프레임에 페이지가 있는지 확인하는 함수
int search_in_frame(int page) {
    for (int i = 0; i < FRAME_SIZE; i++) {
        if (frame[i] == page) {
            return i;
        }
    }
    return -1;
}

// 페이지를 교체하는 함수
void replace_page(int page) {
    while (use_bit[clock_pointer] == 1) {
        use_bit[clock_pointer] = 0;
        clock_pointer = (clock_pointer + 1) % FRAME_SIZE;
    }
    frame[clock_pointer] = page;
    use_bit[clock_pointer] = 1;
    clock_pointer = (clock_pointer + 1) % FRAME_SIZE;
}

// 특정 시간에 프레임 상태를 출력하는 함수
void print_frame_status(int time) {
    printf("시간 %2d: ", time);
    for (int i = 0; i < FRAME_SIZE; i++) {
        printf("%2d ", frame[i]);
    }
    printf("\n");
}

// Clock 알고리즘을 구현한 함수
void clock_algorithm() {
    initialize_frames();
    for (int i = 0; i < STRING_SIZE; i++) {
        int page = reference_string[i];
        int frame_index = search_in_frame(page);
        if (frame_index == -1) { // 페이지 부재 발생
            page_fault_indices[page_fault_count++] = i;
            replace_page(page);
        } else {
            use_bit[frame_index] = 1;
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
