#pragma once
#include "Request.h"

class NetworkActivity
{
public:
	enum Activity
	{
		newConnection,
		closeConnection,
		newRequest
	};

	Activity activity() const; // what has happened
	int connection() const; // socket handle
	std::shared_ptr<Request> request() const;

	NetworkActivity(int, Activity, std::shared_ptr<Request> request);

private:
	int mConnection;
	Activity mActivity;
	std::shared_ptr<Request> mRequest;
};