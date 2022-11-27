#pragma once
#include "Request.h"

class NetworkActivity
{
public:
	enum class Activity
	{
		newConnection,
		closeConnection,
		newRequest
	};

	[[nodiscard]] Activity activity() const; // what has happened
	[[nodiscard]] int connection() const; // socket handle
	[[nodiscard]] std::shared_ptr<Request> request() const;

	NetworkActivity(int, Activity, std::shared_ptr<Request> request);

private:
	int mConnection;
	Activity mActivity;
	std::shared_ptr<Request> mRequest;
};