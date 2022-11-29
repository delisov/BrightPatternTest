#include "NetworkLogsRepo.h"
#include "Utils.h"
#include "constants.h"
#include "ThreadRepo.h"
#include <mutex>

std::string NetworkLogsRepo::printData()
{
    std::scoped_lock<std::mutex> guard(mMutex);

    clearOldRecords();

    std::stringstream ss;
    ss << std::format("|{: ^20}|{: ^20}|{: ^16}|{: ^20}|{: ^20}|{: ^20}|{: ^12}|\n",
        "request #",
        "connection id",
        "client ID begin",
        "client ID end",
        "workload, msec",
        "response time, msec",
        "cancelled");
    ss << std::format("|{: ^20}|{:-^20}|{:-^16}|{:-^20}|{:-^20}|{:-^20}|{:-^12}|\n", "", "", "", "", "", "", "", "");

    auto it = mRecords.begin();
    int toSkip = mRecords.size() - RECORDS_TO_KEEP;
    std::advance(it, toSkip > 0 ? toSkip : 0);
    for (; it != mRecords.end(); ++it) {
        std::string replyMsec = it->second->isReplied ? std::to_string(it->second->replyMsec) : "-";
        ss << std::format("|{: ^20}|{: ^20}|{: ^16}|{: ^20}|{: ^20}|{: ^20}|{: ^12}|\n",
            it->first,
            it->second->connectionId,
            it->second->recipientIdBegin,
            it->second->recipientIdEnd,
            it->second->isLong ? LONG_WORK_MSEC : 0,
            replyMsec,
            it->second->isCancelled ? "yes" : "-");
    }

    return ss.str();
}

void NetworkLogsRepo::addActivity(NetworkActivity netActivity, uuidv4Type recipientId) {
    std::scoped_lock<std::mutex> guard(mMutex);
    auto info = std::shared_ptr<OutputStats>(new OutputStats{ netActivity.connection(), recipientId, uuidv4Type(), netActivity.request()->islong(), false, false, Utils::timestamp(), 0 });
    mRecords[mRequestCounter++] = info;
    mConnections[netActivity.connection()] = info;
}

void NetworkLogsRepo::addConnectionReply(int connection, uuidv4Type recipientId) {
    std::scoped_lock<std::mutex> guard(mMutex);
    mConnections.at(connection)->replyMsec = Utils::timestamp() - mConnections.at(connection)->timestamp;
    mConnections.at(connection)->recipientIdEnd = recipientId;
    mConnections.at(connection)->isReplied = true;
    mConnections.erase(connection);
}

void NetworkLogsRepo::invalidateRequest(int connection) {
    std::scoped_lock<std::mutex> guard(mMutex);
    if (mConnections.contains(connection)) {
        mConnections.at(connection)->isCancelled = true;
    }
}

void NetworkLogsRepo::clearOldRecords() {
   /* if (mRecords.size() > RECORDS_TO_KEEP) {
        auto eraseIter = mRecords.begin();
        std::advance(eraseIter, mRecords.size() - RECORDS_TO_KEEP);
        mRecords.erase(mRecords.begin(), eraseIter);
    }*/
}