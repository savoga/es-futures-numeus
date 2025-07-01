#include <iostream>
#include "NumericUtils.hpp"
#include "Packet.hpp"
#include "FeedArbitrator.hpp"

bool FeedArbitrator::equal(Packet& a, Packet& b) {
    if (std::abs(static_cast<int64_t>(a.timestampNs) - static_cast<int64_t>(b.timestampNs)) < toleranceNs) {
        return true;
    }
    return false;
}
FeedArbitrator::FeedArbitrator(std::vector<Packet> a, std::vector<Packet> b): packetsA(a), packetsB(b) {}
void FeedArbitrator::computeMetrics(){
    int counterA = 0;
    int counterB = 0;
    std::vector<uint64_t> diffAFaster;
    std::vector<uint64_t> diffBFaster;
    while (counterA<packetsA.size() && counterB<packetsB.size()) {
        int64_t diff = packetsA[counterA].timestampNs-packetsB[counterB].timestampNs;
        if (equal(packetsA[counterA], packetsB[counterB])) {
            if (diff < 0) {
                finalPacketsFeedName.push_back("A");
                diffAFaster.push_back(-diff);
            } else {
                finalPacketsFeedName.push_back("B");
                diffBFaster.push_back(diff);
            }
            counterA++;
            counterB++;
        } else if (diff < 0) {
            // packet A is much faster and has no counterpart
            unmatchedA.push_back(packetsA[counterA].seqNum);
            counterA++;
        } else {
            // packet B is much faster and has no counterpart
            unmatchedB.push_back(packetsB[counterB].seqNum);
            counterB++;
        }
    }
    averageTimeDiffAFaster = computeAverage(diffAFaster);
    averageTimeDiffBFaster = computeAverage(diffBFaster);
}
void FeedArbitrator::printReport(){
    std::cout << "Number of packets in side A: " << packetsA.size() << std::endl;
    std::cout << "Number of packets in side B: " << packetsB.size() << std::endl;
    std::cout << "Number of packets in A without a counterpart in B: " << unmatchedA.size() << std::endl;
    std::cout << "Number of packets in B without a counterpart in A: " << unmatchedB.size() << std::endl;
    int countA = 0;
    int countB = 0;
    for (const std::string& feedName : finalPacketsFeedName) {
        if (feedName == "A") countA++;
        else if (feedName == "B") countB++;
    }
    std::cout << "A: " << countA << ", B: " << countB << std::endl;
    std::cout << "Average time difference when A is faster: " << averageTimeDiffAFaster << "ns" << std::endl;
    std::cout << "Average time difference when B is faster: " << averageTimeDiffBFaster << "ns" << std::endl;
}