#pragma once
#include "INetwork.h"
#include <thread>
#include <memory>
#include <mutex>
#include "ConnectionRepo.h"
#include <set>

class RequestGenerator
{
public:
    RequestGenerator(std::weak_ptr<INetwork> net, 
                     unsigned int minMsecWait,
                     unsigned int maxMsecWait,
                     unsigned int maxRequests,
                     unsigned int connectionPoolSize);

    void start();
    void stop();

private:
    std::unique_ptr<std::thread> mThread;
    std::weak_ptr<INetwork> mNetwork;
    std::atomic<bool> mIsActive;
    unsigned int mRequestsGenerated = 0;
    unsigned int mMinMsecWait;
    unsigned int mMaxMsecWait;
    unsigned int mMaxRequests;
    unsigned int mConnectionPoolSize;
    std::map<int, int> mOpenConnections;
    std::vector<int> allConnections;
    
    int getRandomOpenConnection();
    int getRandomClosedConnection();
    void work();
};
