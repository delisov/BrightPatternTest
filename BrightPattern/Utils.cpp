#include "Utils.h"
void Utils::waitRandMsec(int minMsec, int maxMsec) {
	std::mt19937_64 eng{ std::random_device{}() };
	std::uniform_int_distribution<> dist{ minMsec, maxMsec };
	std::this_thread::sleep_for(std::chrono::milliseconds{ dist(eng) });
}

bool Utils::getRandomBool() {
	std::mt19937_64 eng{ std::random_device{}() };
	std::uniform_int_distribution<int> dist{ 0, 1 };
	return dist(eng);
}

long long Utils::timestamp() {
	const auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

size_t Utils::threadId() {
	return std::hash<std::thread::id>{}(std::this_thread::get_id());
}
