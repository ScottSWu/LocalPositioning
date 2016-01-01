#include <stdio.h>
#include <time.h>

void gettime(timespec& t) {
    clock_gettime(CLOCK_MONOTONIC, &t);
}

int m3diff(const timespec& s, const timespec& e) {
    return (e.tv_sec - s.tv_sec) * 1000 + (e.tv_nsec - s.tv_nsec) / 1000000;
}

int main() {
    timespec t,n;
    gettime(t);
    gettime(n);
    int diff;
    const int rows = 20;
    const int cols = 20;
    int index = 0;
    int result[rows * cols];
    for (int i = 0, l = rows * cols; i < l; i++) {
        while ((diff = m3diff(t, n)) <= 0) gettime(n);
        result[index++] = diff;
        t = n;
    }

    index = 0;
    for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        printf("%d ", result[index++]);
    }
    printf("\n");
    }
}
