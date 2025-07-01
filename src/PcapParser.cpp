#include <vector>
#include <pcap.h>
#include "Packet.hpp"
#include "PcapParser.hpp"

PcapParser::PcapParser(const std::string& filename) : filename_(filename) {}

std::vector<Packet> PcapParser::parsePackets() {
    std::vector<Packet> packets;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_offline(filename_.c_str(), errbuf);
    struct pcap_pkthdr* header;
    const u_char* pktData;
    while(pcap_next_ex(handle, &header, &pktData)>0) {
        const u_char* udpPayload = pktData + 42;
        size_t udpLen = header->caplen - 42; // caplen = "captured length" of the packet = number of bytes
        uint32_t seqNum = udpPayload[0] |
             (udpPayload[1] << 8) |
             (udpPayload[2] << 16) |
             (udpPayload[3] << 24);
        uint64_t tsNs = extractMetamakoTimestamp(udpPayload, udpLen);
        packets.emplace_back(seqNum, tsNs); // like push_back but construct an object at the same time
    }
    pcap_close(handle);
    return packets;
}
uint64_t PcapParser::extractMetamakoTimestamp(const u_char* udpPayload, size_t udpLen) {
    if (udpLen<20) return 0;
    const u_char* trailer = udpPayload + udpLen - 20;
    uint32_t seconds;
    uint32_t nanoseconds;
    memcpy(&seconds, trailer + 8, 4);
    memcpy(&nanoseconds, trailer + 12, 4);
    seconds = ntohl(seconds); // converter from big-endian (network convention) to little-endian (CPU readable)
    nanoseconds = ntohl(nanoseconds);
    return static_cast<uint64_t>(seconds) * 1'000'000'000ULL + nanoseconds;
}