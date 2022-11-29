#pragma once
#include <list>
#include <mutex>
#include <iostream>
#include "NetworkActivity.h"
#include "Reply.h"
#include "INetwork.h"
#include "ConnectionRepo.h"

// network layer
class Network : public INetwork
{
public:
	Network(std::weak_ptr<ConnectionRepo> connectionRepo) : mConnectionRepo(connectionRepo) {
	}

	/// <summary>
	/// Put an activity to the buffer
	/// </summary>
	void addActivity(NetworkActivity netActivity);
	/// <summary>
	///	Removes activity from the buffer and returns it
	/// </summary>
	[[maybe_unused]] std::list<NetworkActivity> Select(unsigned timeout);

	/// <summary>
	/// Sending reply to a request. no need to delete the reply after that
	/// </summary>
	void sendReply(int connection, std::shared_ptr<Reply> reply);

	/// <summary>
	/// Returns true if application should exit.
	/// We assume that it should exit if the buffer is currently empty
	/// </summary>
	[[nodiscard]] bool shouldExit() const;

private:
	std::list<NetworkActivity> mActivityBuffer;
	std::weak_ptr<ConnectionRepo> mConnectionRepo;
};

