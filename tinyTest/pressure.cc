#include "../include/travial.h"
#include <myownHead.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define PORT 8080
#define ADDR "0.0.0.0"

std::mutex mtx;  // 用于输出控制

void clientTask(int id, int numRequests) {
    struct sockaddr_in serAddr;
    int cliFd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(PORT);
    serAddr.sin_addr.s_addr = inet_addr(ADDR);

    if (connect(cliFd, (struct sockaddr*)&serAddr, sizeof(serAddr)) != 0) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cerr << "Thread " << id << ": Failed to connect to server\n";
        return;
    }

    for (int i = 0; i < numRequests; ++i) {
        nlohmann::json jsonreq;
        jsonreq["type"] = "WebPageSeacher";
        jsonreq["value"] = "Test Query " + std::to_string(id) + "_" + std::to_string(i);
        std::string req = jsonreq.dump();

        int len = req.size();
        write(cliFd, &len, sizeof(len));
        send(cliFd, req.data(), req.size(), 0);

        int retLen = 0;
        read(cliFd, &retLen, sizeof(retLen));
        std::vector<char> buff(retLen);
        recv(cliFd, &buff[0], retLen, 0);

        std::string res(buff.begin(), buff.end());

        // 输出响应
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Thread " << id << ": Received response for request " << i << "\n";
        }

        // Sleep for a short duration to simulate a real client
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    close(cliFd);
}

int main(int argc, char *argv[]) {
    int numThreads = 10;      // 并发客户端数量
    int numRequestsPerThread = 100;  // 每个线程发起的请求数量

    std::vector<std::thread> threads;

    // 启动多个线程进行并发测试
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(clientTask, i, numRequestsPerThread);
    }

    // 等待所有线程结束
    for (auto &t : threads) {
        t.join();
    }

    return 0;
}

