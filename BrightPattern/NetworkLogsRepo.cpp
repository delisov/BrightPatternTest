#include "NetworkLogsRepo.h"
#include "Utils.h"
#include "constants.h"
#include "ThreadRepo.h"
#include <mutex>

std::string NetworkLogsRepo::printData()
{
	std::scoped_lock<std::mutex> guard(mMutex);

	clearOldRecords();

	std::stringstream ss;
	ss << std::format("|{: ^10}|{: ^16}|{: ^20}|{: ^20}|\n", "id", "timetamp", "work time, msec", "response time, msec");

	for (auto it = mRecords.begin(); it != mRecords.end(); ++it) {
		std::string replyMsec = it->second.isReplied ? std::to_string(it->second.replyMsec) : "-";
		ss << std::format("|{: ^10}|{: ^16}|{: ^20}|{: ^20}|\n", it->first, it->second.timestamp, it->second.isLong ? LONG_WORK_MSEC : 0, replyMsec);
	}

	return ss.str();
}

void NetworkLogsRepo::addActivity(NetworkActivity netActivity) {
	std::scoped_lock<std::mutex> guard(mMutex);
	mRecords[netActivity.connection()] = { Utils::timestamp(), netActivity.request()->islong(), false, 0 };
}

void NetworkLogsRepo::addConnectionReply(int connection) {
	std::scoped_lock<std::mutex> guard(mMutex);
	mRecords[connection].replyMsec = Utils::timestamp() - mRecords[connection].timestamp;
	mRecords[connection].isReplied = true;
}

void NetworkLogsRepo::clearOldRecords() {
	if (mRecords.size() > RECORDS_TO_KEEP) {
		auto eraseIter = mRecords.begin();
		std::advance(eraseIter, mRecords.size() - RECORDS_TO_KEEP);
		mRecords.erase(mRecords.begin(), eraseIter);
	}
}