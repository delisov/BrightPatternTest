#include "RequestProcessor.h"

RequestProcessor::RequestProcessor(std::weak_ptr<ConnectionRepo> connectionRepo,
                                   int threadsCount,
                                   std::string name) : 
                                   mConnectionRepo(connectionRepo),
                                   mThreadsNumber(threadsCount),
                                   mName(name)
{
}

void RequestProcessor::processRequest(NetworkActivity a, 
                                      std::weak_ptr<INetwork> network)
{
    std::unique_lock<std::mutex> guard(mMutex);
    initialize();
    auto recipientIdBegin = mConnectionRepo.lock()->getConnection(a.connection()).recipientUuid;
    mRequests.push({ a, recipientIdBegin, network });
    sRequestAvailableCv.notify_one();
}

void RequestProcessor::start() {
    mIsRunning = true;
    initialize();
}

void RequestProcessor::stop() {
    mIsRunning = false;
    sRequestAvailableCv.notify_all();
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
        ThreadRepo::instance()->addThread(mName, "Awaiting requests");
        std::unique_lock<std::mutex> guard(mMutex);
        sRequestAvailableCv.wait(guard, [this] { return !mRequests.empty() || !mIsRunning; });

        if (!mIsRunning) {
            break;
        }

        auto task = std::move(mRequests.front());
        mRequests.pop();
        guard.unlock();
        ThreadRepo::instance()->addThread(mName, "Processing connection " + std::to_string(task.activity.connection()));
        std::shared_ptr<Reply> rpl(task.activity.request()->process());
        
        guard.lock();
        if (mConnectionRepo.lock()->verifyConnectionRecipient(task.activity.connection(), task.recipientId)) {
            task.network.lock()->sendReply(task.activity.connection(), rpl);
        }

        ThreadRepo::instance()->addThread(mName, "Done with connection " + std::to_string(task.activity.connection()));
        
    }
}
