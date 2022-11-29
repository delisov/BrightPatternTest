#include <thread>
#include <random>
#include <chrono>
#include <iostream>
#include <mutex>
#include "RequestGenerator.h"
#include "Utils.h"
#include "ThreadRepo.h"

typedef NetworkActivity::Activity Activity;

RequestGenerator::RequestGenerator(std::weak_ptr<INetwork> net,
                                   unsigned int minMsecWait,
                                   unsigned int maxMsecWait,
                                   unsigned int maxRequests,
                                   unsigned int connectionPoolSize) :
                                   mNetwork(net),
                                   mIsActive(false),
                                   mMinMsecWait(minMsecWait),
                                   mMaxMsecWait(maxMsecWait),
                                   mMaxRequests(maxRequests),
                                   mConnectionPoolSize(connectionPoolSize)
{
    for (unsigned int i = 0; i < mConnectionPoolSize; i++) {
        allConnections.push_back(i);
    }
}

void RequestGenerator::start() {
    mIsActive = true;
    mThread = std::make_unique<std::thread>(&RequestGenerator::work, this);
}

void RequestGenerator::stop() {
    mIsActive = false;
    mThread->join();
}

int RequestGenerator::getRandomOpenConnection() {
    if (mOpenConnections.empty()) {
        return -1;
    }

    auto it = mOpenConnections.begin();
    int pos = Utils::getRandomInt(0, mOpenConnections.size() - 1);

    std::advance(it, pos);
    return it->first;
}

struct KeyRetriever
{
    template <typename T>
    typename T::first_type operator()(T keyValuePair) const
    {
        return keyValuePair.first;
    }
};

int RequestGenerator::getRandomClosedConnection() {
    if (mOpenConnections.size() == mConnectionPoolSize) {
        return -1;
    }

    std::vector<int> closedConnections;
    if (mOpenConnections.size() != 0) {
        std::vector<int> openConnections;

        transform(mOpenConnections.begin(), mOpenConnections.end(), back_inserter(openConnections), KeyRetriever());

        auto ccIt = closedConnections.begin();

        std::set_difference(allConnections.begin(), allConnections.end(), openConnections.begin(), openConnections.end(), 
            std::inserter(closedConnections, closedConnections.begin()));
    }
    else {
        closedConnections = allConnections;
    }

    int pos = Utils::getRandomInt(0, closedConnections.size() - 1);
    return closedConnections[pos];
}

void RequestGenerator::work()
{
    ThreadRepo::instance()->addThread("RequestGenerator", "Request generator");
    std::cout << "Starting request generation..." << std::endl;

    while (mIsActive && mRequestsGenerated++ < mMaxRequests) {
        std::mt19937_64 eng{ std::random_device{}() };
        std::discrete_distribution<int> dist{ 45, 10, 45 };
        auto activityType = static_cast<Activity>(dist(eng));

        switch (activityType) {
        case Activity::newRequest:
        {
            auto request = std::make_shared<Request>(Utils::getRandomBool());
            int connectionId = getRandomOpenConnection();

            // Impossible to use a connection as the connection pool is empty
            // or we already sent request for this connection
            if (connectionId == -1 || mOpenConnections.at(connectionId) > 0) {
                mRequestsGenerated--;
                continue;
            }

            mOpenConnections[connectionId]++;
            NetworkActivity activity(connectionId, Activity::newRequest, request);
            mNetwork.lock()->addActivity(activity);
            Utils::waitRandMsec(mMinMsecWait, mMaxMsecWait);
            break;
        }
        case Activity::closeConnection:
        {
            int connectionId = getRandomOpenConnection();

            // Impossible to close a connection as the connection pool is empty
            if (connectionId == -1) {
                mRequestsGenerated--;
                continue;
            }

            NetworkActivity activity(connectionId, Activity::closeConnection);
            mOpenConnections.erase(connectionId);
            mNetwork.lock()->addActivity(activity);
            break;
        }
        case Activity::newConnection:
        {
            // Impossible to open a new connection as the connection pool is full
            if (mOpenConnections.size() == mConnectionPoolSize) {
                mRequestsGenerated--;
                continue;
            }

            int connectionId = getRandomClosedConnection();

            NetworkActivity activity(connectionId, Activity::newConnection);
            mOpenConnections[connectionId] = 0;
            mNetwork.lock()->addActivity(activity);
            break;
        }
        }
    }

    std::cout << "Request generation finished" << std::endl;
    ThreadRepo::instance()->removeThread();
}