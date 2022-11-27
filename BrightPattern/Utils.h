#pragma once
#include <thread>
#include <random>
#include <chrono>

class Utils
{
public:
	static void waitRandMsec(int minMsec, int maxMsec);

	[[nodiscard]] static bool getRandomBool();

	[[nodiscard]] static long long timestamp();

	[[nodiscard]] static size_t threadId();
};

