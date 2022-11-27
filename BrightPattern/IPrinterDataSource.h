#pragma once
#include <string>

class IPrinterDataSource {
public:
	[[nodiscard]] virtual std::string name() = 0;
	[[nodiscard]] virtual std::string printData() = 0;
};