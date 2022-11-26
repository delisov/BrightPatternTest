#include "Request.h"
#include "NetworkActivity.h"

typedef NetworkActivity::Activity Activity;

Activity NetworkActivity::activity() const
{
	return mActivity;
}

int NetworkActivity::connection() const
{
	return mConnection;
}

std::shared_ptr<Request> NetworkActivity::request() const // returns new request if activity is newRequest
{
	return mRequest;
}

NetworkActivity::NetworkActivity(int connection, Activity activity, std::shared_ptr<Request> request): mConnection(connection), mActivity(activity), mRequest(request)
{
}
