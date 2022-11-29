#pragma once

class INetworkInterface {
public:
	virtual void send(int connection, int ip, void* data);
};