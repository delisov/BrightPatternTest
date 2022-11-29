#include "ConsolePrinter.h"
#include "ThreadRepo.h"
#include "constants.h"

#define gotoxy(x, y) printf("\033[%d;%dH", x, y)

void ConsolePrinter::addPrinterSource(std::weak_ptr<IPrinterDataSource> source) {
	mDataSources.push_back(source);
}

void ConsolePrinter::start()
{
	std::scoped_lock<std::mutex> guard(mMutex);
	mIsPrintActive = true;
	mThread = std::make_unique<std::thread>(&ConsolePrinter::printWork, this);
}

void ConsolePrinter::stop()
{
	std::scoped_lock<std::mutex> guard(mMutex);
	mIsPrintActive = false;
	mThread->join();
}

void clear()
{
#if defined _WIN32
	system("cls");
	//clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
	//std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
	system("clear");
#endif

	gotoxy(0, 0);
}

void ConsolePrinter::printWork()
{
	ThreadRepo::instance()->addThread("ConsolePrinter", "Printing output");

	while (mIsPrintActive) {
		std::stringstream ss;

		std::this_thread::sleep_for(std::chrono::milliseconds(PRINT_REFRESH_MSEC));

		for (auto it = mDataSources.begin(); it != mDataSources.end(); ++it) {
			if (auto dataSource = it->lock()) {
				ss << dataSource->name() << std::endl << "-------------------------------" << std::endl;
				ss << dataSource->printData() << std::endl;
				ss << std::endl <<
					"==============================" << 
					std::endl << std::endl;
			}
		}

		clear();
		printf("%s", ss.str().c_str());
	}

	ThreadRepo::instance()->removeThread();
}