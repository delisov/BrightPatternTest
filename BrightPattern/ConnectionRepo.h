#pragma once
#include <string>
#include <map>
#include <memory>
#include "Utils.h"
#include "IPrinterDataSource.h"
#include <list>

typedef std::string uuidv4Type;

class ConnectionRepo : public IPrinterDataSource
{
public:
    struct ConnectionInfo {
        bool isOpen = true;
        unsigned int openRequests = 0;
        uuidv4Type recipientUuid = "";
    };

    [[nodiscard]] constexpr std::string name() {
        return "Open connections";
    }

    [[nodiscard]] std::string printData();

    [[maybe_unused]] uuidv4Type addConnection(int connection);

    [[maybe_unused]] void registerNewRequest(int connection);

    [[nodiscard]] ConnectionInfo getConnection(int connection) const;

    [[nodiscard]] std::vector<int> listConnections() const;

    [[nodiscard]] bool verifyConnectionRecipient(int connection, uuidv4Type recipient) const;

    void invalidateConnection(int connection);

private:
    mutable std::mutex mMutex;
    std::map<unsigned int, ConnectionInfo> mConnections;
};

