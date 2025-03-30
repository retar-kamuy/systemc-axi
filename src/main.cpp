#include <sstream>
// #include <string>
#include <iostream>
#include <thread>
#include "include/thread_pool_executor.hpp"


class HelloWorld {
public:
    std::string say_hello(int number) {
        std::cout << "[say_hello (" << std::this_thread::get_id() << ")] >>> Start" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // 何か重い処理
        std::ostringstream oss;
        oss << "[say_hello (" << std::this_thread::get_id() << ")] >>> Hello! number: " << number;
        std::cout << "[say_hello (" << std::this_thread::get_id() << ")] >>> End" << std::endl;
        return oss.str();
    }
};

std::string say_ok(int number) {
    std::cout << "[say_ok (" << std::this_thread::get_id() << ")] >>> Start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // 何か重い処理
    std::ostringstream oss;
    oss << "[say_ok (" << std::this_thread::get_id() << ")] >>> OK! number: " << number;
    std::cout << "[say_ok (" << std::this_thread::get_id() << ")] >>> End" << std::endl;
    return oss.str();
}

int main() {
    // スレッドプールの作成
    ThreadPoolExecutor executor;

    // グローバル関数の非同期実行
    auto ok_future = executor.submit(say_ok, 100);

    // メンバ関数の非同期実行
    HelloWorld hello;
    auto hello_future = executor.submit([&](auto number){ return hello.say_hello(number); }, 999);

    // 結果の取得
    executor.done();
    std::cout << ok_future.get() << std::endl;
    executor.done();
    std::cout << hello_future.get() << std::endl;

    return 0;
}