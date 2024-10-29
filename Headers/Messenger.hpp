#include <stdint.h>

class Messenger
{

public:
    uint64_t byte_compress(uint8_t *data_ptr, uint64_t data_size); // Data size wasn't specifed -> Assumed 64 bits
    uint64_t byte_decompress(uint8_t *compressed_data_ptr, uint64_t compressed_data_size);

private:
    const uint8_t MAX_COMPRESSION_COUNT = 0xFF; // Count has to fit in 8 bits
};