#pragma once
#include <memory>
#include "Request.h"
#include "INetwork.h"
#include <functional>
#include <queue>
#include "ThreadRepo.h"
#include "ConnectionRepo.h"

class RequestProcessor
{
public:
    RequestProcessor(std::weak_ptr<ConnectionRepo> connectionRepo, int threadsCount, std::string name);

    void processRequest(NetworkActivity a, std::weak_ptr<INetwork> network);
    void start();
    void stop();

private:
    struct RequestInfo {
        NetworkActivity activity;
        uuidv4Type recipientId;
        std::weak_ptr<INetwork> network;
    };

    std::string mName;
    std::atomic<bool> mIsRunning = false;
    std::atomic<bool> mIsInitialized = false;
    std::atomic<int> mThreadsNumber;
    std::mutex mMutex;
    std::unique_ptr<std::thread[]> mThreads;
    std::queue<RequestInfo> mRequests;
    std::condition_variable sRequestAvailableCv = {};
    std::weak_ptr<ConnectionRepo> mConnectionRepo;

    void worker();
    void initialize();
    void uninitialize();
};