#pragma once
#include <vector>
#include "IPrinterDataSource.h"
#include <mutex>

// Periodically writes to console by updating the screen
class ConsolePrinter
{
public:
	void addPrinterSource(std::weak_ptr<IPrinterDataSource> source);
	void startPrintLogs();
	void stopPrintLogs();
private:
	std::vector<std::weak_ptr<IPrinterDataSource>> mDataSources;

	mutable std::mutex mMutex;
	bool mIsPrintActive;
    void printWork();
	std::unique_ptr<std::thread> mThread;


};

