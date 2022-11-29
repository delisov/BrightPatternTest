#include "Network.h"

/// <summary>
/// Put an activity to the buffer
/// </summary>

void Network::addActivity(NetworkActivity netActivity) {switch (auto act = netActivity.activity()) {
	case NetworkActivity::Activity::newConnection:
		mConnectionRepo.lock()->addConnection(netActivity.connection());
		break;
	case NetworkActivity::Activity::closeConnection:
		mConnectionRepo.lock()->invalidateConnection(netActivity.connection());
		break;
	case NetworkActivity::Activity::newRequest:
		mConnectionRepo.lock()->registerNewRequest(netActivity.connection());
		break;
	default:
		throw std::runtime_error(std::format("Unknown activity type {}", static_cast<int>(act)));
	}

	mActivityBuffer.push_back(netActivity);
}

/// <summary>
///	Removes activity from the buffer and returns it
/// </summary>

std::list<NetworkActivity> Network::Select(unsigned timeout) {
	if (mActivityBuffer.empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}
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
	return mActivityBuffer.empty();
}
