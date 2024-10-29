import heapq
from collections import Counter

class HuffmanNode:
    def __init__(self, data, freq):
        self.data = data
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

class Messenger:
    def byte_compress(self, data):
        if len(data) <= 1:
            return data, len(data), None  # No compression needed for small data sizes

        # Step 1: Build frequency table
        freq_table = Counter(data)

        # Step 2: Build Huffman Tree
        root = self.build_huffman_tree(freq_table)

        # Step 3: Generate Huffman codes
        huffman_codes = {}
        self.generate_huffman_codes(root, "", huffman_codes)

        # Step 4: Encode data using Huffman codes
        compressed_data_bits = ''.join(huffman_codes[byte] for byte in data)
        
        # Convert the bit string to bytearray
        compressed_data = bytearray()
        for i in range(0, len(compressed_data_bits), 8):
            byte = compressed_data_bits[i:i+8]
            compressed_data.append(int(byte.ljust(8, '0'), 2))

        # Serialize Huffman table for transmission
        huffman_table = [(byte, code) for byte, code in huffman_codes.items()]

        # Calculate Huffman table size in bytes
        huffman_table_size = sum(1 + (len(code) + 7) // 8 for byte, code in huffman_table)

        # Total size of compressed data + Huffman table
        compressed_size = len(compressed_data)
        total_transmission_size = compressed_size + huffman_table_size

        return compressed_data, compressed_size, huffman_table, total_transmission_size

    def build_huffman_tree(self, freq_table):
        priority_queue = [HuffmanNode(byte, freq) for byte, freq in freq_table.items()]
        heapq.heapify(priority_queue)

        while len(priority_queue) > 1:
            left = heapq.heappop(priority_queue)
            right = heapq.heappop(priority_queue)
            merged = HuffmanNode(None, left.freq + right.freq)
            merged.left = left
            merged.right = right
            heapq.heappush(priority_queue, merged)

        return priority_queue[0] if priority_queue else None

    def generate_huffman_codes(self, root, code, huffman_codes):
        if root is None:
            return

        if root.data is not None:
            huffman_codes[root.data] = code

        self.generate_huffman_codes(root.left, code + "0", huffman_codes)
        self.generate_huffman_codes(root.right, code + "1", huffman_codes)

# Example usage
if __name__ == "__main__":
    data = bytearray([0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
                      0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09])

    messenger = Messenger()
    compressed_data, compressed_size, huffman_table, total_transmission_size = messenger.byte_compress(data)

    print("Compressed data (in bytes):", compressed_data)
    print("Compressed size:", compressed_size, "bytes")
    print("Huffman Table (for decoding):")
    for byte, code in huffman_table:
        print(f"  Byte: {hex(byte)} Code: {code}")
    print("Total transmission size (compressed data + Huffman table):", total_transmission_size, "bytes")
