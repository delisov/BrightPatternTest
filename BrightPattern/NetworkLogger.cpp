#include "NetworkLogger.h"

void NetworkLogger::addActivity(NetworkActivity netActivity) {
	mNetworkLogsRepo.lock()->addActivity(netActivity);
	mNetwork.lock()->addActivity(netActivity);
}

std::list<NetworkActivity> NetworkLogger::Select(unsigned timeout) {
	return mNetwork.lock()->Select(timeout);
}

void NetworkLogger::sendReply(int connection, std::shared_ptr<Reply> reply) {
	mNetworkLogsRepo.lock()->addConnectionReply(connection);
	mNetwork.lock()->sendReply(connection, reply);
}

bool NetworkLogger::shouldExit() const {
	return mNetwork.lock()->shouldExit();
}
