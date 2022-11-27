#include "Network.h"

/// <summary>
/// Put an activity to the buffer
/// </summary>

void Network::addActivity(NetworkActivity netActivity) {
	std::scoped_lock<std::mutex> guard(mMutex);
	mActivityBuffer.push_back(netActivity);
}

/// <summary>
///	Removes activity from the buffer and returns it
/// </summary>

std::list<NetworkActivity> Network::Select(unsigned timeout) {
	if (mActivityBuffer.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}
	if (mActivityBuffer.empty()) {
		return std::list<NetworkActivity>();
	}
	std::scoped_lock<std::mutex> guard(mMutex);
	auto recentActivity = move(mActivityBuffer);
	return recentActivity;
}

/// <summary>
/// Sending reply to a request. no need to delete the reply after that
/// </summary>

void Network::sendReply(int connection, std::shared_ptr<Reply> reply) {
}

/// <summary>
/// Returns true if application should exit.
/// We assume that it should exit if the buffer is currently empty
/// </summary>

bool Network::shouldExit() const {
	std::scoped_lock<std::mutex> guard(mMutex);
	return mActivityBuffer.empty();
}
