#pragma once
#include <thread>
#include <random>
#include <chrono>

class Utils
{
public:
	static void waitRandMsec(int minMsec, int maxMsec);

	[[nodiscard]] static bool getRandomBool();

	[[nodiscard]] static int getRandomInt(int min, int max);

	[[nodiscard]] static long long timestamp();

	[[nodiscard]] static size_t threadId();

	[[nodiscard]] static std::string generate_guid();
};

