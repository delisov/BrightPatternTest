#include "NetworkLogger.h"

void NetworkLogger::addActivity(NetworkActivity netActivity) {
    if (netActivity.activity() == NetworkActivity::Activity::newRequest) {
        int connection = netActivity.connection();
        auto recipientIdBegin = mConnectionRepo.lock()->getConnection(connection).recipientUuid;
        mNetworkLogsRepo.lock()->addActivity(netActivity, recipientIdBegin);
    }
    else if (netActivity.activity() == NetworkActivity::Activity::closeConnection) {
        mNetworkLogsRepo.lock()->invalidateRequest(netActivity.connection());
    }
    mNetwork.lock()->addActivity(netActivity);
}

std::list<NetworkActivity> NetworkLogger::Select(unsigned timeout) {
    auto result = mNetwork.lock()->Select(timeout);
    return result;
}

void NetworkLogger::sendReply(int connection, std::shared_ptr<Reply> reply) {
    auto recipientIdBegin = mConnectionRepo.lock()->getConnection(connection).recipientUuid;
    mNetworkLogsRepo.lock()->addConnectionReply(connection, recipientIdBegin);
    mNetwork.lock()->sendReply(connection, reply);
}

bool NetworkLogger::shouldExit() const {
    return mNetwork.lock()->shouldExit();
}
