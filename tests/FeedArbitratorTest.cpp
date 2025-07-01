#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define UNIT_TEST
#include "doctest.h"
#include "./../src/FeedArbitrator.hpp"

Packet createPacket(uint32_t seqNum, uint64_t timestampNs) {
    return Packet(seqNum, timestampNs);
}

TEST_CASE("FeedArbitrator basic instantiation") {
    std::vector<Packet> a = {createPacket(1, 1000000)};
    std::vector<Packet> b = {createPacket(1, 1000005)};

    FeedArbitrator arb(a, b);
    CHECK_NOTHROW(arb.computeMetrics());
}

TEST_CASE("equal() correctly identifies close timestamps") {
    Packet a(1, 1000000);
    Packet b(1, 1000050); // diff = 50ns < 10000ns

    FeedArbitrator arb({a}, {b});
    CHECK(arb.equal(a, b) == true);
}

TEST_CASE("FeedArbitrator handles mix of matches and unmatched packets") {
    std::vector<Packet> a = {
        createPacket(1, 1000000),   // matched
        createPacket(2, 2000000),   // unmatched
        createPacket(3, 3000000),   // matched
        createPacket(4, 5000000),   // matched
    };
    std::vector<Packet> b = {
        createPacket(1, 1000020),   // matched
        createPacket(99, 1500000),  // unmatched
        createPacket(3, 3000010),   // matched
        createPacket(4, 4999900),   // matched
    };

    FeedArbitrator arb(a, b);
    arb.computeMetrics();

    CHECK_NOTHROW(arb.printReport());
}