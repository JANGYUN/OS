#include <stdio.h>
#include <stdlib.h>

#define FRAME_SIZE 3
#define STRING_SIZE 20

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
    int stack[FRAME_SIZE];
    int time = 0, pageFaults = 0;

    // 프레임과 스택 초기화
    for (int i = 0; i < FRAME_SIZE; i++) {
        for (int j = 0; j < STRING_SIZE; j++) {
            frames[i][j] = -1;
        }
        stack[i] = -1;
    }

    for (int i = 0; i < STRING_SIZE; i++) {
        int page = referenceString[i];
        int found = 0;

        // 스택에 페이지가 있는지 확인
        for (int j = 0; j < FRAME_SIZE; j++) {
            if (stack[j] == page) {
                found = 1;
                // 페이지를 스택의 맨 위로 이동
                for (int k = j; k > 0; k--) {
                    stack[k] = stack[k - 1];
                }
                stack[0] = page;
                break;
            }
        }

        // 페이지가 스택에 없는 경우 페이지 부재 발생
        if (!found) {
            pageFaults++;
            // 스택의 모든 페이지를 한 칸씩 아래로 이동
            for (int j = FRAME_SIZE - 1; j > 0; j--) {
                stack[j] = stack[j - 1];
            }
            stack[0] = page;

            // 페이지 부재 인덱스 기록
            printf("페이지 부재 발생 인덱스: %d\n", i);
        }

        // 각 시간 단계에서 프레임 상태 업데이트
        for (int j = 0; j < FRAME_SIZE; j++) {
            frames[j][time] = stack[j];
        }

        time++;
    }

    // 최종 프레임 행렬 출력
    printf("각 시간 단계에서의 프레임 상태:\n");
    printFrames(frames, time);

    return 0;
}
