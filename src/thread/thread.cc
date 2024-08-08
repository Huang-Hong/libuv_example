#include <stdio.h>
#include <unistd.h>
#include <uv.h>

// 编译:  g++ -o thread thread.cc -luv
void fun1(void *arg) {
    int track_len = *((int *)arg);
    while (track_len) {
        track_len--;
        sleep(1);
        printf("fun1 runing \n");
    }
    printf("fun1 done running!\n");
}

void fun2(void *arg) {
    int track_len = *((int *)arg);
    while (track_len) {
        track_len--;
        printf("fun2 running\n");
        sleep(3);
    }
    printf("fun2 done running!\n");
}

int main() {
    int track_len = 10;
    uv_thread_t hare;
    uv_thread_t tortoise;
    uv_thread_create(&hare, fun1, &track_len);
    uv_thread_create(&tortoise, fun2, &track_len);

    uv_thread_join(&hare);
    uv_thread_join(&tortoise);
    return 0;
}
