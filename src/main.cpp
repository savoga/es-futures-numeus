#include <iostream>
#include <filesystem>
#include <optional>
#include <numeric>

#include <pcap.h>

#include "Packet.hpp"
#include "PcapParser.hpp"
#include "FeedArbitrator.hpp"
 
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "No directory was given." << std::endl;
        return 1;
    }

    std::string directory = argv[1];
    std::cout << "Directory provided: " << directory << std::endl;

    std::string filenameSideA = directory + "/1567029600000000000_xcme_delta-s1p1-udp_0.0.0.0_0-224.0.31.1_14310-00000000.pcap"; // side A (lowest number as per CME)
    std::string filenameSideB = directory + "/1567029600000000000_xcme_delta-s1p1-udp_0.0.0.0_0-224.0.32.1_15310-00000000.pcap";
    
    PcapParser parserA(filenameSideA);
    PcapParser parserB(filenameSideB);

    // First element: GMT Thursday, August 29, 2019 5:09:48.322 PM
    // Last element: GMT Thursday, August 29, 2019 5:13:12.293 PM
    auto packetsA = parserA.parsePackets();
    auto packetsB = parserB.parsePackets();

    FeedArbitrator fa(packetsA, packetsB);
    fa.computeMetrics();
    fa.printReport();
    
    return 0;
}

