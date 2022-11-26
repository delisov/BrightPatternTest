#include "RequestProcessor.h"

RequestProcessor::RequestProcessor(int threadsCount) : mThreadsNumber(threadsCount) {

}
void RequestProcessor::processRequest(NetworkActivity a, std::weak_ptr<INetwork> network) {
    std::unique_lock<std::mutex> guard(mMutex);
    initialize();
    mRequests.push(std::make_pair(a, network));
    sRequestAvailableCv.notify_one();
}

void RequestProcessor::start() {
    mIsRunning = true;
    initialize();
}

void RequestProcessor::stop() {
    mIsRunning = false;
    uninitialize();
}

void RequestProcessor::initialize() {
    if (!mIsInitialized) {
        mThreads = std::make_unique<std::thread[]>(mThreadsNumber);
        for (auto i = 0; i < mThreadsNumber; ++i)
        {
            mThreads[i] = std::thread(&RequestProcessor::worker, this);
        }
        mIsInitialized = true;
    }
}

void RequestProcessor::uninitialize() {
    if (mIsInitialized) {
        for (auto i = 0; i < mThreadsNumber; ++i)
        {
            mThreads[i].join();
        }
        mThreads = nullptr;
        mIsInitialized = false;
    }
}

void RequestProcessor::worker() {
    while (mIsRunning)
    {
        ThreadRepo::instance()->addThread("Request processor", "Awaiting requests");
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
    }
}
