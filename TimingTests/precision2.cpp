// Don't run while running tmux =(
// Don't run while running anything really

#include <stdio.h>
#include <time.h>

void gettime(timespec& t) {
    clock_gettime(CLOCK_MONOTONIC, &t);
}

int m3diff(const timespec& s, const timespec& e) {
    return (e.tv_sec - s.tv_sec) * 1000 + (e.tv_nsec - s.tv_nsec) / 1000000;
}

int m4diff(const timespec& s, const timespec& e) {
    return (e.tv_sec - s.tv_sec) * 10000 + (e.tv_nsec - s.tv_nsec) / 100000;
}

int m5diff(const timespec& s, const timespec& e) {
    return (e.tv_sec - s.tv_sec) * 100000 + (e.tv_nsec - s.tv_nsec) / 10000;
}

int m6diff(const timespec& s, const timespec& e) {
    return (e.tv_sec - s.tv_sec) * 1000000 + (e.tv_nsec - s.tv_nsec) / 1000;
}

int main() {
    timespec t,n;
    int diff;
    const int rows = 200;
    const int cols = 50;
    int index = 0;
    int result[4 * rows * cols];

    index = 0;
    gettime(t);
    gettime(n);
    for (int i = 0, l = rows * cols; i < l; i++) {
        while ((diff = m3diff(t, n)) <= 1) gettime(n);
        result[index++] = diff;
        t = n;
    }

    gettime(t);
    gettime(n);
    for (int i = 0, l = rows * cols; i < l; i++) {
        while ((diff = m5diff(t, n)) <= 1) gettime(n);
        result[index++] = diff;
        t = n;
    }

    gettime(t);
    gettime(n);
    for (int i = 0, l = rows * cols; i < l; i++) {
        while ((diff = m5diff(t, n)) <= 1) gettime(n);
        result[index++] = diff;
        t = n;
    }

    gettime(t);
    gettime(n);
    for (int i = 0, l = rows * cols; i < l; i++) {
        while ((diff = m6diff(t, n)) <= 1) gettime(n);
        result[index++] = diff;
        t = n;
    }

    
    index = 0;
    for (int d = 3; d <= 6; d++) {
        printf("m%d\n", d);
        int bad = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (result[index++] != 2) {
                    bad++;
                }
                //printf("%d ", result[index++]);
            }
            //printf("\n");
        }
        printf("Bad: %d\n\n", bad);
    }
}
