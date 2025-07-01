#ifndef FEEDARBITRATOR_HPP
#define FEEDARBITRATOR_HPP

#include <iostream>
#include "NumericUtils.hpp"
#include "Packet.hpp"

class FeedArbitrator {
private:
    std::vector<Packet> packetsA;
    std::vector<Packet> packetsB;
    std::vector<uint32_t> unmatchedA;
    std::vector<uint32_t> unmatchedB;
    std::vector<std::string> finalPacketsFeedName;
    double averageTimeDiffAFaster;
    double averageTimeDiffBFaster;
    int toleranceNs = 10000; // 1000 ns = 1 us
    int numMatches = 0;
    int numMismatches = 0;
#ifdef UNIT_TEST
public:
#else
private:
#endif
    bool equal(Packet& a, Packet& b);
public:
    FeedArbitrator(std::vector<Packet> a, std::vector<Packet> b);
    void computeMetrics();
    void printReport();
}; 

#endif