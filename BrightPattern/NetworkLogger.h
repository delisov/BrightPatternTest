#pragma once
#include "NetworkLogsRepo.h"

/// <summary>
/// A proxy class that wraps Network and collects all the activity
/// </summary>
class NetworkLogger: public INetwork
{
public:
	NetworkLogger(std::weak_ptr<INetwork> network, std::weak_ptr<NetworkLogsRepo> logsRepo): mNetwork(network), mNetworkLogsRepo(logsRepo) {}

	void addActivity(NetworkActivity netActivity);
	std::list<NetworkActivity> Select(unsigned timeout);
	void sendReply(int connection, std::shared_ptr<Reply>);
	bool shouldExit() const;

private:
	mutable std::mutex mMutex;
	std::weak_ptr<INetwork> mNetwork;
	std::weak_ptr<NetworkLogsRepo> mNetworkLogsRepo;
};

