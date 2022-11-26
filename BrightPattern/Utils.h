#pragma once
#include <thread>
#include <random>
#include <chrono>

class Utils
{
public:
	static void waitRandMsec(int minMsec, int maxMsec);

	static bool getRandomBool();

	static long long timestamp();

	static size_t threadId();
};

