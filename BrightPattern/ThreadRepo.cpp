#include "ThreadRepo.h"

std::string ThreadRepo::name() { return "Threads"; }

void ThreadRepo::addThread(std::string name, std::string activity) {
	std::scoped_lock<std::mutex> guard(mMutex);
	mThreads[Utils::threadId()] = { name, activity };
}

std::shared_ptr<ThreadRepo> ThreadRepo::instance()
{
	static std::shared_ptr<ThreadRepo> s{ new ThreadRepo };
	return s;
}

void ThreadRepo::removeThread() {
	std::scoped_lock<std::mutex> guard(mMutex);
	mThreads.erase(Utils::threadId());
}

std::string ThreadRepo::printData() {
	std::scoped_lock<std::mutex> guard(mMutex);

	std::stringstream ss;
	ss << std::format("|{: ^20}|{: ^40}|{: ^40}|\n", "thread id", "name", "activity");

	for (auto it = mThreads.begin(); it != mThreads.end(); ++it) {
		ss << std::format("|{: ^20}|{: ^40}|{: ^40}|\n", it->first, it->second.name, it->second.activity);
	}

	return ss.str();
}
