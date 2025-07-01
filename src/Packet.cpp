#include <iostream>
#include "Packet.hpp"

Packet::Packet(uint32_t seq, uint64_t ts) : seqNum(seq), timestampNs(ts) {}