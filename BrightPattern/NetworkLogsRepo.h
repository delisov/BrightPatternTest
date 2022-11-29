#pragma once
#include <iostream>
#include <map>
#include "Network.h"
#include "IPrinterDataSource.h"
#include "Utils.h"
#include "ConnectionRepo.h"

typedef int IdType;
typedef long long TimestampType;

class NetworkLogsRepo : public IPrinterDataSource {
public:
	void addActivity(NetworkActivity netActivity, uuidv4Type recipientId);
	void addConnectionReply(int connection, uuidv4Type recipientId);
	void invalidateRequest(int connection);
	[[nodiscard]] constexpr std::string name() { return "Network activity"; }
	[[nodiscard]] std::string printData();

private:
	struct OutputStats {
		int connectionId;
		uuidv4Type recipientIdBegin;
		uuidv4Type recipientIdEnd;
		bool isLong;
		bool isReplied;
		bool isCancelled;
		TimestampType timestamp;
		TimestampType replyMsec;
	};

	IdType mRequestCounter = 0;
	mutable std::mutex mMutex;
	std::map<IdType, std::shared_ptr<OutputStats>> mRecords;
	std::map<IdType, std::shared_ptr<OutputStats>> mConnections;
	void clearOldRecords();
};