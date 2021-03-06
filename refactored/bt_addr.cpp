#include "bt_addr.h"
#include <iostream>

BtAddr::BtAddr(uint8_t fill) {
    memset(&m_addr, fill, sizeof(bdaddr_t));
}

BtAddr::BtAddr(const std::string& addr) {
    str2ba(addr.c_str(), &m_addr);
}

BtAddr::BtAddr(const std::vector<uint8_t>& buffer, size_t offset) {
    if (buffer.size() - offset < 6) {
        throw std::runtime_error("Insufficient data for a bluetooth addrerss");
    }
    memcpy(&m_addr, &buffer[offset], sizeof(bdaddr_t));
}

std::string BtAddr::toString() const {
    char str[128];
    ba2str(&m_addr, str);
    return std::string(str);   
}

void BtAddr::getSockAddr(struct sockaddr_rc& sockaddr, uint8_t channel) const {
    sockaddr.rc_family = AF_BLUETOOTH;
    sockaddr.rc_channel = channel;
    memcpy(&sockaddr.rc_bdaddr, &m_addr, sizeof(sockaddr.rc_bdaddr));
}

std::vector<uint8_t> BtAddr::asBytes() const {
    std::vector<uint8_t> message;
    for (size_t i = 0; i < sizeof(bdaddr_t); ++i) {
        message.push_back(m_addr.b[i]);
    }
    return message;
}
