#include <stdio.h>
#include <stdbool.h>

#define FRAME_SIZE 3
#define REFERENCE_LENGTH 20

// 페이지가 프레임에 있는지 확인하는 함수
bool isInFrames(int frames[], int page) {
    for (int i = 0; i < FRAME_SIZE; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

// 교체할 최적의 페이지를 찾는 함수
int findOptimalIndex(int frames[], int refStr[], int currentIndex) {
    int farthestIndex = currentIndex;
    int indexToReplace = -1;
    for (int i = 0; i < FRAME_SIZE; i++) {
        int j;
        for (j = currentIndex; j < REFERENCE_LENGTH; j++) {
            if (frames[i] == refStr[j]) {
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
    int frames[FRAME_SIZE] = {0, 0, 0};
    int frameStates[REFERENCE_LENGTH][FRAME_SIZE] = {0};
    int pageFaultIndices[REFERENCE_LENGTH] = {0};
    int pageFaults = 0;

    for (int i = 0; i < REFERENCE_LENGTH; i++) {
        if (!isInFrames(frames, refStr[i])) {
            int indexToReplace = findOptimalIndex(frames, refStr, i + 1);
            frames[indexToReplace] = refStr[i];
            pageFaultIndices[pageFaults++] = i;
        }
        for (int j = 0; j < FRAME_SIZE; j++) {
            frameStates[i][j] = frames[j];
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
