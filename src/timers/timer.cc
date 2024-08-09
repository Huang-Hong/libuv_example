#include <uv.h>

// 编译:  g++ -o timer timer.cc -luv

static uv_timer_t testTimer;

uv_loop_t* g_loop = uv_default_loop();

void Test(uv_timer_t* handle) {
    static int count = 0;
    count++;
    printf("update shopActive %d\n", count);
    if (count == 5) {
        printf("save shopData %d\n", count);
        count = 0;
    }
}

int main(int argc, char* argv[]) {
    uv_timer_init(g_loop, &testTimer);
    uv_timer_start(&testTimer, Test, 5000, 0);
    uv_run(g_loop, UV_RUN_DEFAULT);

    printf("main loop stop\n");
    return 0;
}
