#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>

struct Packet {
    uint32_t seqNum;
    uint64_t timestampNs; // computed from Metamako trailer
    Packet(uint32_t seq, uint64_t ts);
};

#endif