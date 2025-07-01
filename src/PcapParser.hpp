#ifndef PCAPPARSER_HPP
#define PCAPPARSER_HPP

#include <string>
#include "Packet.hpp"

class PcapParser {
public:
    PcapParser(const std::string& filename);
    std::vector<Packet> parsePackets();
private:
    std::string filename_;
    uint64_t extractMetamakoTimestamp(const u_char* udpPayload, size_t udpLen);
};

#endif