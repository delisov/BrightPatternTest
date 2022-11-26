#pragma once
#include <thread>
#include <random>
#include <chrono>

class Utils
{
public:
	static void waitRandMsec(int minMsec, int maxMsec) {
		std::mt19937_64 eng{ std::random_device{}() };
		std::uniform_int_distribution<> dist{ minMsec, maxMsec };
		std::this_thread::sleep_for(std::chrono::milliseconds{ dist(eng) });
	}

	static bool getRandomBool() {
		std::mt19937_64 eng{ std::random_device{}() };
		std::uniform_int_distribution<int> dist{ 0, 1 };
		return dist(eng);
	}

	static long long timestamp() {
		const auto now = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	}

	static size_t threadId() {
		return std::hash<std::thread::id>{}(std::this_thread::get_id());
	}
};

