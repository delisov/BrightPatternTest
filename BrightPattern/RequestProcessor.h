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

	void processRequest(NetworkActivity a, std::weak_ptr<INetwork> network) {
        std::unique_lock<std::mutex> guard(mMutex);
        initialize();
        mRequests.push(std::make_pair(a, network));
        sRequestAvailableCv.notify_one();
	}

    void start() {
        mIsRunning = true;
        initialize();
    }

    void stop() {
        mIsRunning = false;
        uninitialize();
    }

    void initialize() {
        if (!mIsInitialized) {
            mThreads = std::make_unique<std::thread[]>(mThreadsNumber);
            for (auto i = 0; i < mThreadsNumber; ++i)
            {
                mThreads[i] = std::thread(&RequestProcessor::worker, this);
            }
            mIsInitialized = true;
        }
    }

    void uninitialize() {
        if (mIsInitialized) {
            for (auto i = 0; i < mThreadsNumber; ++i)
            {
                mThreads[i].join();
            }
            mThreads = nullptr;
            mIsInitialized = false;
        }
    }

private:
    std::atomic<bool> mIsRunning = false;
    std::atomic<bool> mIsInitialized = false;
    std::atomic<int> mThreadsNumber = THREADS_COUNT;
    std::mutex mMutex;
	std::unique_ptr<std::thread[]> mThreads;
    std::queue<std::pair<NetworkActivity, std::weak_ptr<INetwork>>> mRequests;
    
    std::condition_variable sRequestAvailableCv = {};
    std::condition_variable task_done_cv = {};

	void worker() {
        while (mIsRunning)
        {
            ThreadRepo::instance()->addThread("Request processor", "Reading requests");
            std::unique_lock<std::mutex> guard(mMutex);
            sRequestAvailableCv.wait(guard, [this] { return !mRequests.empty() || !mIsRunning; });
            auto task = std::move(mRequests.front());
            mRequests.pop();
            guard.unlock();
            ThreadRepo::instance()->addThread("Request processor", "Processing connection " + std::to_string(task.first.connection()));
            std::shared_ptr<Reply> rpl(task.first.request()->process());
            task.second.lock()->sendReply(task.first.connection(), rpl);
            ThreadRepo::instance()->addThread("Request processor", "Done with connection " + std::to_string(task.first.connection()));
            guard.lock();
            task_done_cv.notify_one();
        }
	}
};