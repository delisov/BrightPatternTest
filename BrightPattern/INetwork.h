#pragma once
#include "NetworkActivity.h"
#include <list>

class INetwork {
public:
	virtual void addActivity(NetworkActivity) = 0;
	[[maybe_unused]] virtual std::list<NetworkActivity> Select(unsigned) = 0;
	virtual void sendReply(int, std::shared_ptr<Reply>) = 0;
	virtual bool shouldExit() const = 0;
};