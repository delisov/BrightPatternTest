#include <thread>
#include <random>
#include <chrono>
#include <iostream>
#include <mutex>
#include "RequestGenerator.h"
#include "Utils.h"
#include "ThreadRepo.h"

typedef NetworkActivity::Activity Activity;

RequestGenerator::RequestGenerator(std::weak_ptr<INetwork> net, 
	unsigned int minMsecWait, 
	unsigned int maxMsecWait, 
	unsigned int maxRequests) :
	mNetwork(net), mIsActive(false), mMinMsecWait(minMsecWait), mMaxMsecWait(maxMsecWait), mMaxRequests(maxRequests)
{
}

void RequestGenerator::start() {
	mIsActive = true;
	mThread = std::make_unique<std::thread>(&RequestGenerator::work, this);
}

void RequestGenerator::stop() {
	mIsActive = false;
	mThread->join();
}

void RequestGenerator::work()
{
	ThreadRepo::instance()->addThread("RequestGenerator", "Request generator");
	std::cout << "Starting request generation..." << std::endl;

	while (mIsActive && mConnectionCounter < mMaxRequests) {
		auto request = std::make_shared<Request>(Utils::getRandomBool());
		NetworkActivity activity(mConnectionCounter++, Activity::newRequest, request);
		mNetwork.lock()->addActivity(activity);
		Utils::waitRandMsec(mMinMsecWait, mMaxMsecWait);
	}

	std::cout << "Request generation finished" << std::endl;
	ThreadRepo::instance()->removeThread();
}