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

	static std::shared_ptr<ThreadRepo> instance();

	void addThread(std::string name, std::string activity);
	void removeThread();

	// IPrinterDataSource
	std::string name();
	std::string printData();

private:
	struct ThreadInfo {
		std::string name;
		std::string activity;
	};

	std::map<int, ThreadInfo> mThreads;
	std::mutex mMutex;
	ThreadRepo() {}
};