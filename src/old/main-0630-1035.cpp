#include <iostream>
#include <filesystem>

#include <pcap.h>

void packetHandler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // for each packet
    // one packet: header + data
    int *packet_count = reinterpret_cast<int*>(user_data);
    (*packet_count)++;
    // std::cout << "Packet total length: " << pkthdr->len << std::endl;
    
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "No directory was given." << std::endl;
        return 1;
    }

    std::string directory = argv[1];
    std::cout << "Directory provided: " << directory << std::endl;

    std::string filenameSideA = directory + "/1567029600000000000_xcme_delta-s1p1-udp_0.0.0.0_0-224.0.31.1_14310-00000000.pcap"; // side A (lowest number as per CME)
    std::string filenameSideB = directory + "/1567029600000000000_xcme_delta-s1p1-udp_0.0.0.0_0-224.0.32.1_15310-00000000.pcap";
    
    std::vector<std::string> filenameAll = {filenameSideA, filenameSideB};

    for (int i=0; i<2; i++) {
        std::string filename = filenameAll[i];
        // c_str() returns a POINTER
        // this array is necessary to correctly handle errors by the library pcap
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *handle = pcap_open_offline(filenameSideA.c_str(), errbuf);
        if (handle == nullptr) {
            std::cerr << "Failed to open file: " << errbuf << std::endl;
            return 1;
        } else {
            std::cout << "Opening: " << filename << std::endl;
        }
        int packet_count = 0;
        // pcap_loop needs to have user_data as a pointer on u_char -> we need to cast
        if (pcap_loop(handle, 0, packetHandler, reinterpret_cast<u_char*>(&packet_count)) < 0) {
            std::cerr << "Error processing packets: " << pcap_geterr(handle) << std::endl;
            return 1;
        }

        std::cout << "Total number of packets: " << packet_count << std::endl;

        pcap_close(handle);
    }
    return 0;
}