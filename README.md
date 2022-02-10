# Contents:

### 1. Allocation Restricted Base

This is a base class intended to enforce a "static allocation" policy. 

It has the following deleted members:

- `operator new`
- `operator delete`
- Copy constructor and assignment operator
- Move constructor and move operator

It also overrides global `new`

Note that you should still break point `placement new` and `placement delete` in your code if you want to prevent unexpected dynamic allocations.
Any use of the STL should also require careful review of the specific STL library source code.

### 2. Bitset Utils

`void insert_bitset_at_offset(std::bitset<TARGET_SIZE> &target,  const std::bitset<SOURCE_SIZE> &source, const uint16_t &msb_offset)`

This inserts the bits from a source bitset into a target bitset at the specified offset. Useful when manipulating large sequences of bits.

`void bitset_to_bytearray(std::array<uint8_t, TARGET_SIZE> &target_array, const std::bitset<SOURCE_SIZE> &source_bitset)`

This converts a source bitset into a target byte std::array. This doesn't check sizes so user must take care that the `source.size() / 8` does not exceed `target.size()`

`void print_bits(std::bitset<BITSET_SIZE> &pattern [[maybe_unused]])`

This is a pretty print function for displaying the contents of the bitset. 

Supports `SEGGER_RTT` and `iostream`

### 3. Byte Utils

`void print_bytes(std::array<uint8_t, BYTE_ARRAY_SIZE> &bytes [[maybe_unused]])`

This is a pretty print function for displaying the contents of the byte std::array. 

Supports `SEGGER_RTT`.

### 4. LL I2C Utils



### 7. LL SPI Utils
### 8. Static Map
### 9. Timer Management
