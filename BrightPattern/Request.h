#pragma once
#include "Reply.h"
#include "constants.h"
#include <chrono>
#include <thread>

// request from network packet
class Request
{
public:
	Request(bool isLong);
	// is it long or short in handling
	bool isShort() const { return !mLong; }
	bool islong() const { return mLong; }

	// creating reply packet (i.e. processing the request)
	Reply* process();

private:
	bool mLong;
};