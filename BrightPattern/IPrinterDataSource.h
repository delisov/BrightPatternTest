#pragma once
#include <string>

class IPrinterDataSource {
public:
	virtual std::string name() = 0;
	virtual std::string printData() = 0;
};