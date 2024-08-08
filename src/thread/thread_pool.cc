#include <uv.h>

#include <cstdio>

// https://developer.baidu.com/article/details/3294827

/*在调用uv_queue_work之后，libuv会将任务放入其内部的工作队列中，并立即返回。
然后，libuv会在其内部的线程池中取出一个线程来执行任务。当任务完成后，libuv会调用之前注册的回调函数来通知主线程。
*/

/*libuv的工作队列非常适合执行那些需要阻塞线程或者需要较长时间才能完成的任务，
例如文件I/O、网络I/O等。通过将这些任务放在工作队列中执行，我们可以避免阻塞主线程，从而提高应用的性能。*/

//可通过 UV_THREADPOOL_SIZE 调整线程池的大小

// 编译:  g++ -o thread_pool thread_pool.cc -luv

uv_work_t req;
uv_loop_t *loop = uv_default_loop();

void thread_work(uv_work_t *req) {
    // 这里执行阻塞或耗时的任务
    int i = 0;
    while (i < 5) {
        printf("do work.....\n");
        uv_sleep(1000);
        i++;
    }
}
void on_work_complete(uv_work_t *req, int status) {
    // 任务完成后调用此函数
    printf("work done\n");
}

int main() {
    uv_queue_work(loop, &req, thread_work, on_work_complete);
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}