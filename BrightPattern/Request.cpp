#include "Request.h"

Request::Request(bool isLong) : mLong(isLong) {}

// creating reply packet (i.e. processing the request)

Reply* Request::process()
{
	if (mLong)
		std::this_thread::sleep_for(std::chrono::milliseconds(LONG_WORK_MSEC));
	return new Reply;
}
