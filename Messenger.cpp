#include "Headers/Messenger.hpp"
#include <iostream>
#include <algorithm>

/**
 * @brief Compresses the data in the provided buffer using Run-Length Encoding (RLE).
 *
 * @param data_ptr Pointer to the data buffer to be compressed.
 * @param data_size The original size of the data buffer.
 * @return The size of the compressed data within the buffer.
 */
uint64_t Messenger::byte_compress(uint8_t *data_ptr, uint64_t data_size)
{
    if (data_size <= 1)
        return data_size; // Early return on low data_size

    uint8_t *tmpBuffer = new uint8_t[data_size * 2]; // data_size is dynamic. Can't modify in place in case worst case doubles and overwrites
    uint64_t newSize = 0;

    // RLE algorithm on tmpBuffer
    for (uint64_t i = 0; i < data_size;)
    {
        uint8_t value_read = data_ptr[i];
        uint8_t count = 1;
        while (i + count < data_size && data_ptr[i + count] == value_read && count < this->MAX_COMPRESSION_COUNT)
        {
            count++;
        }
        tmpBuffer[newSize++] = count;
        tmpBuffer[newSize++] = value_read;

        i += count;
    }

    std::copy(tmpBuffer, tmpBuffer + newSize, data_ptr);    // Copy compressed data to data_ptr
    std::fill(data_ptr + newSize, data_ptr + data_size, 0); // Clear excess data to 0

    delete[] tmpBuffer; // Clean up dynamically allocated memory
    return newSize;
}

/**
 * @brief Decompresses the data in the provided buffer using Run-Length Encoding (RLE).
 *
 * @param compressed_data_ptr Pointer to the data buffer already compressed with RLE
 * @param data_size The original size of the data buffer. Guarantees that original fits in this size after decompression
 * @return The size of the decompressed data within the buffer.
 */
uint64_t Messenger::byte_decompress(uint8_t *compressed_data_ptr, uint64_t data_size)
{
    if (data_size <= 1)
        return data_size; // Early return on low data_size

    uint8_t *tmpBuffer = new uint8_t[data_size]; // Dynamically allocate temp array to prevent read after writing

    // Decompress with RLE
    uint64_t decompressed_index = 0;
    for (uint64_t i = 0; i < data_size; i += 2)
    {
        uint8_t run_length = compressed_data_ptr[i];
        uint8_t value = compressed_data_ptr[i + 1];

        for (uint8_t run_index = 0; run_index < run_length; run_index++)
        {
            tmpBuffer[decompressed_index++] = value;
        }
    }

    std::copy(tmpBuffer, tmpBuffer + data_size, compressed_data_ptr);
    delete[] tmpBuffer;
    return data_size;
}
