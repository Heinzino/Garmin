#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
#include "Headers/TestMessenger.hpp"
#include "Headers/Messenger.hpp" // Adjust to match the directory structure

/**
 * @brief Constructor to initialize the random number generator.
 */
TestMessenger::TestMessenger()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

/**
 * @brief Runs multiple tests of compression and decompression.
 *
 * This function iterates through a series of tests where random data is generated,
 * compressed, decompressed, and verified for accuracy. The compression ratio is
 * calculated and displayed for each test case.
 *
 * @param testCount Number of tests to run.
 * @param dataSize Size of each data buffer to be generated and tested.
 */
void TestMessenger::runTests(int testCount, uint64_t dataSize)
{
    for (int i = 0; i < testCount; ++i)
    {
        uint8_t *data = generateRandomData(dataSize);

        std::cout << "Test #" << i + 1 << " - ";
        testCompression(data, dataSize);

        delete[] data;
    }
}

/**
 * @brief Generates a random data buffer with a tendency for repeated values.
 *
 * This function creates a data buffer of specified size, where values have a
 * higher likelihood of repeating in series between 2 and 6 times. This mimics
 * data patterns that can be compressed efficiently with Run-Length Encoding.
 *
 * @param size Size of the data buffer to generate.
 * @return Pointer to the generated data buffer. The caller is responsible for deleting it.
 */
uint8_t *TestMessenger::generateRandomData(uint64_t size)
{
    uint8_t *data = new uint8_t[size];
    std::default_random_engine generator(std::rand());
    std::uniform_int_distribution<int> repeatDistribution(2, 6); // For series length
    std::uniform_int_distribution<int> valueDistribution(0, 255); // For byte values

    uint64_t i = 0;
    while (i < size)
    {
        uint8_t value = valueDistribution(generator); // Random byte value
        int repeatCount = repeatDistribution(generator); // Random repeat count between 2 and 6

        for (int j = 0; j < repeatCount && i < size; ++j)
        {
            data[i++] = value; // Repeat the value in series
        }
    }
    return data;
}

/**
 * @brief Compresses and decompresses data, then validates the decompression accuracy.
 *
 * This function compresses a randomly generated data buffer, calculates the
 * compression ratio, and decompresses the data to verify if the original data is restored.
 * Additionally, it outputs the original, compressed, and decompressed data for inspection.
 *
 * @param data Pointer to the data buffer to be tested.
 * @param dataSize Size of the original data buffer.
 */
void TestMessenger::testCompression(uint8_t *data, uint64_t dataSize)
{
    Messenger messenger;
    uint8_t *originalData = new uint8_t[dataSize];
    std::copy(data, data + dataSize, originalData);

    std::cout << "Original Data: ";
    printData(originalData, dataSize);

    uint64_t compressedSize = messenger.byte_compress(data, dataSize);
    std::cout << "Compressed Data: ";
    printData(data, compressedSize);

    double compressionRatio = static_cast<double>(dataSize) / compressedSize;
    std::cout << "Original Size: " << dataSize << ", Compressed Size: " << compressedSize
              << ", Compression Ratio: " << compressionRatio << std::endl;

    uint64_t decompressedSize = messenger.byte_decompress(data, compressedSize);
    std::cout << "Decompressed Data: ";
    printData(data, decompressedSize);

    if (std::equal(data, data + decompressedSize, originalData))
    {
        std::cout << "Decompression validation passed." << std::endl;
    }
    else
    {
        std::cout << "Decompression validation failed." << std::endl;
    }

    delete[] originalData;
}

/**
 * @brief Prints data in hexadecimal format for easy viewing.
 *
 * This function iterates over a data buffer and prints each byte in two-digit
 * hexadecimal format, separated by spaces.
 *
 * @param data Pointer to the data buffer to print.
 * @param size Size of the data buffer.
 */
void TestMessenger::printData(uint8_t *data, uint64_t size)
{
    for (uint64_t i = 0; i < size; ++i)
    {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}
