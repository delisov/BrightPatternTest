#pragma once
#include <iostream>
#include <map>
#include "Network.h"
#include "IPrinterDataSource.h"
#include "Utils.h"

typedef int IdType;
typedef long long TimestampType;

class NetworkLogsRepo : public IPrinterDataSource {
public:
	void addActivity(NetworkActivity netActivity);
	void addConnectionReply(int connection);
	std::string name() { return "Network activity"; }
	std::string printData();

private:
	struct OutputStats {
		TimestampType timestamp;
		bool isLong;
		bool isReplied;
		TimestampType replyMsec;
	};

	mutable std::mutex mMutex;
	std::map<IdType, OutputStats> mRecords;
	void clearOldRecords();
};