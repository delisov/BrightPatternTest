#pragma once
#include "INetwork.h"
#include <thread>
#include <memory>
#include <mutex>

class RequestGenerator
{
public:
	RequestGenerator(std::weak_ptr<INetwork> net, unsigned int minMsecWait, unsigned int maxMsecWait);

	void start();
	void stop();

private:
	std::unique_ptr<std::thread> mThread;
	std::weak_ptr<INetwork> mNetwork;
	std::atomic<bool> mIsActive;
	unsigned int mMinMsecWait;
	unsigned int mMaxMsecWait;

	unsigned int mConnectionCounter = 0;
	void work();
};
