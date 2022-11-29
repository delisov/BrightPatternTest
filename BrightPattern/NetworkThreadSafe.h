#pragma once
#include "Network.h"

class NetworkThreadSafe: public Network
{
public:
	NetworkThreadSafe(std::weak_ptr<ConnectionRepo> connectionRepo) : Network(connectionRepo) {}

	void addActivity(NetworkActivity netActivity);

	[[maybe_unused]] std::list<NetworkActivity> Select(unsigned timeout);

	void sendReply(int connection, std::shared_ptr<Reply> reply);

	[[nodiscard]] bool shouldExit() const;

private:
	mutable std::mutex mMutex;
};

