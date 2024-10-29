#ifndef TESTMESSENGER_HPP
#define TESTMESSENGER_HPP

#include <cstdint>

class TestMessenger
{
public:
    TestMessenger();
    void runTests(int testCount, uint64_t dataSize);

private:
    uint8_t *generateRandomData(uint64_t size);
    void testCompression(uint8_t *data, uint64_t dataSize);
    void printData(uint8_t *data, uint64_t size);
};

#endif // TESTMESSENGER_HPP
