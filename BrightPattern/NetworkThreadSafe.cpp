#include "NetworkThreadSafe.h"

void NetworkThreadSafe::addActivity(NetworkActivity netActivity) {
	std::unique_lock<std::mutex> guard(mMutex);

	Network::addActivity(netActivity);
}

[[maybe_unused]]
std::list<NetworkActivity> NetworkThreadSafe::Select(unsigned timeout) {
	std::unique_lock<std::mutex> guard(mMutex);
	auto result = Network::Select(0);
	guard.unlock();
	if (result.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
		guard.lock();
		result = Network::Select(0);
		guard.unlock();
	}
	return result;
}

void NetworkThreadSafe::sendReply(int connection, std::shared_ptr<Reply> reply) {
	std::unique_lock<std::mutex> guard(mMutex);
	Network::sendReply(connection, reply);
}

[[nodiscard]]
bool NetworkThreadSafe::shouldExit() const {
	std::unique_lock<std::mutex> guard(mMutex);
	return Network::shouldExit();
}
