#pragma once
#include <memory>
#include "Request.h"
#include "INetwork.h"
#include <functional>
#include <queue>
#include "ThreadRepo.h"

class RequestProcessor
{
public:
    RequestProcessor(int threadsCount);

    void processRequest(NetworkActivity a, std::weak_ptr<INetwork> network);
    void start();
    void stop();

private:
    std::atomic<bool> mIsRunning = false;
    std::atomic<bool> mIsInitialized = false;
    std::atomic<int> mThreadsNumber = THREADS_COUNT;
    std::mutex mMutex;
    std::unique_ptr<std::thread[]> mThreads;
    std::queue<std::pair<NetworkActivity, std::weak_ptr<INetwork>>> mRequests;
    std::condition_variable sRequestAvailableCv = {};

    void worker();
    void initialize();
    void uninitialize();
};