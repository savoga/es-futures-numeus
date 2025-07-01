# Analysis of HFT trades (ES futures)

# Intro

In this project I process high-frequency market data captured from the CME (specifically ES futures on channel 310). The project parses packets from PCAP files, extracts timestamps from Metamako trailers, and matches packets from two feeds (A and B) based on timing and content similarity. The core module, FeedArbitrator, identifies unmatched packets and computes which feed is faster on average.

# Setup

- C++ version: 17

- Compiler: Clang

- Important libraries: PCAP

# How to run it

I usually run it either from my Visual Studio Code console or in the terminal with the command:

```clang++ -Iinclude/vcpkg/installed/arm64-osx/include -Linclude/vcpkg/installed/arm64-osx/lib -lpcap src/FeedArbitrator.cpp src/Packet.cpp src/PcapParser.cpp src/NumericUtils.cpp -std=c++17 src/main.cpp -o main``` then ```./main ./data```

# Improvements

Potential improvements:

- refactoring:
  - docstring generation
  - avoid copying some variables when unecessary
  - better error handling

- unit testing
  - parser logic
  - arbitrator (other tests)

