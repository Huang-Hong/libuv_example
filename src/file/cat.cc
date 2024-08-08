#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

// 编译:  g++ -o cat cat.cc -luv

/*
以下是libuv官方的实现cat命令的代码
    代码的基本逻辑是：
    1.打开目标文件，并在打开成功后在on_open回调函数中读取文件内容。
    2.在读取文件成功后，将读取到的内容写到标准输出。
    3.在写到标准输出成功后继续读取目标文件。
    4.循环进行2、3步，直到读到文件的末尾。
    5.关闭文件。
    说明:在读文件的过程中，程序员无须指定一次uv_fs_read所读取的字符个数，这是因为libuv会根据buffer的大小和文件的大小自行决定应该读取多少字符。
*/

/*

同步 synchronous 形式如果没有指定回调函数则会被自动调用( 并阻塞的), 函数的返回值是libuv error code.
但以上通常只对同步调用有意义. 而异步 asynchronous 形式则会在传入回调函数时被调用, 并且返回 0.

*/

void on_read(uv_fs_t *req);

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[1024];

static uv_buf_t iov;

void on_write(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror((int)req->result));
    } else {
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}

void on_read(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
    } else if (req->result == 0) {
        uv_fs_t close_req;
        // synchronous
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    } else if (req->result > 0) {
        iov.len = req->result;
        uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
    }
}

void on_open(uv_fs_t *req) {
    // The request passed to the callback is the same as the one the call setup
    // function was passed.
    assert(req == &open_req);
    if (req->result >= 0) {
        iov = uv_buf_init(buffer, sizeof(buffer));
        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
    } else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
}

/*
打开文件
uv_fs_open(uv_loop_t* loop,uv_fs_t* req,const char* path,int flags,int mode,uv_fs_cb cb);
参数1：最终被uv_run启动的event-loop,如果只有一个loop的话可以使用libuv提供的默认loop：uv_default_loop();
参数2：与打开文件操作相关联的对象；
参数3：要打开的文件的路径名；
参数4：flag与标准的unix的open函数的flag相同；
参数5：mode与标准的unix的open函数的mode相同，如果忽略此项的话值可为0；
参数6：文件打开成功/失败后所调用的回调函数,函数原型:(uv_fs_cb)(uv_fs_t req);
*/

int main(int argc, char **argv) {
    uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);

    // 如果想使用同步的话，则将回调函数那里置为nullptr就可以了，如下
    // int a = uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, nullptr);
    // if (a < 0) {
    //     perror("1111:");
    // }

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);
    uv_fs_req_cleanup(&write_req);
    return 0;
}
