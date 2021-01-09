#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    std::vector<int> values{12, 4, 30, 5, 78, 45, 64};
    std::vector<std::thread> thread_pool;
    for (int v : values) {
        thread_pool.emplace_back([v&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(v*10));
            std::cout << v << std::endl;
        });
    }
    for (auto &th : thread_pool) {
        th.join();
    }

    std::cout << "Please do not use in production code\n";
    return 0;
}
