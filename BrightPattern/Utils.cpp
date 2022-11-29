#include "Utils.h"
void Utils::waitRandMsec(int minMsec, int maxMsec) {
	std::this_thread::sleep_for(std::chrono::milliseconds{ getRandomInt(minMsec, maxMsec) });
}

bool Utils::getRandomBool() {
	std::mt19937_64 eng{ std::random_device{}() };
	std::uniform_int_distribution<int> dist{ 0, 1 };
	return dist(eng);
}

int Utils::getRandomInt(int min, int max)
{
    std::mt19937_64 eng{ std::random_device{}() };
    std::uniform_int_distribution<> dist{ min, max };
    return dist(eng);
}


long long Utils::timestamp() {
	const auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

size_t Utils::threadId() {
	return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

std::string Utils::generate_guid() {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }

    return ss.str();
}
