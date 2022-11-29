#include "ConnectionRepo.h"

[[nodiscard]]
std::string ConnectionRepo::printData() {
    std::scoped_lock<std::mutex> guard(mMutex);

    std::stringstream ss;
    ss << std::format("|{: ^20}|{: ^16}|{: ^20}|\n", "connection id", "open requests", "recipient uuid");
    ss << std::format("|{:-^20}|{:-^16}|{:-^20}|\n", "", "", "");

    for (auto it = mConnections.begin(); it != mConnections.end(); ++it) {
        ss << std::format("|{: ^20}|{: ^16}|{: ^20}|\n", it->first, it->second.openRequests, it->second.recipientUuid);
    }

    return ss.str();
}

[[maybe_unused]]
uuidv4Type ConnectionRepo::addConnection(int connection) {
    std::scoped_lock<std::mutex> guard(mMutex);
    uuidv4Type recipientIdBegin = Utils::generate_guid();
    mConnections[connection] = { true, 0, recipientIdBegin };
    return recipientIdBegin;
}

[[maybe_unused]]
void ConnectionRepo::registerNewRequest(int connection) {
    std::scoped_lock<std::mutex> guard(mMutex);
    mConnections.at(connection).openRequests++;
}

[[maybe_unused]]
void ConnectionRepo::registerReply(int connection) {
    std::scoped_lock<std::mutex> guard(mMutex);
    mConnections.at(connection).openRequests--;
}

[[nodiscard]]
ConnectionRepo::ConnectionInfo ConnectionRepo::getConnection(int connection) const {
    std::scoped_lock<std::mutex> guard(mMutex);
    if (mConnections.contains(connection)) {
        return mConnections.at(connection);
    }
    else {
        return { false, 0, "" };
    }
}

struct KeyRetriever
{
    template <typename T>
    typename T::first_type operator()(T keyValuePair) const
    {
        return keyValuePair.first;
    }
};

[[nodiscard]]
std::vector<int> ConnectionRepo::listConnections() const {
    std::scoped_lock<std::mutex> guard(mMutex);
    std::vector<int> keys;

    // Retrieve all keys
    std::transform(mConnections.begin(), mConnections.end(), std::back_inserter(keys), KeyRetriever());
    return keys;
}

[[nodiscard]]
bool ConnectionRepo::verifyConnectionRecipient(int connection, uuidv4Type recipient) const {
    std::scoped_lock<std::mutex> guard(mMutex);
    return (mConnections.contains(connection) && mConnections.at(connection).recipientUuid == recipient);
}

void ConnectionRepo::invalidateConnection(int connection) {
    std::scoped_lock<std::mutex> guard(mMutex);
    mConnections.erase(connection);
}
