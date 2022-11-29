#pragma once
#include <thread>
#include <map>
#include <iostream>
#include <mutex>
#include "Utils.h"
#include "IPrinterDataSource.h"
#include <sstream>

class ThreadRepo: public IPrinterDataSource
{
public:
	ThreadRepo(ThreadRepo const&) = delete;
	void operator=(ThreadRepo const&) = delete;

	[[nodiscard]] static std::shared_ptr<ThreadRepo> instance();

	void addThread(std::string name, std::string activity);
	void removeThread();

	// IPrinterDataSource
	[[nodiscard]] constexpr std::string name();
	[[nodiscard]] std::string printData();

private:
	struct ThreadInfo {
		std::string name;
		std::string activity;
	};

	std::map<size_t, ThreadInfo> mThreads;
	std::mutex mMutex;
	ThreadRepo() {}
};
