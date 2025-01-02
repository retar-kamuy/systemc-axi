#ifndef CONSTANS_HPP_
#define CONSTANS_HPP_

// Burst types
// AWBURST/ARBURST
enum AxiBurstType {
    FIXED   = 0b00,
    INCR    = 0b01,
    WRAP    = 0b10
};

// Burst sizes (per beat)
// AWSIZE/ARSIZE
enum AxiBurstSize {
    SIZE_1      = 0b000,
    SIZE_2      = 0b001,
    SIZE_4      = 0b010,
    SIZE_8      = 0b011,
    SIZE_16     = 0b100,
    SIZE_32     = 0b101,
    SIZE_64     = 0b110,
    SIZE_128    = 0b111
};

// Lock types
// AWLOCK/ARLOCK
enum AxiLockType {
    NORMAL      = 0b0,
    EXCLUSIVE   = 0b1
};

// Cache control
// AWCACHE/ARCACHE
enum AxiCacheBit {
    B   = 0b0001,
    M   = 0b0010,
    RA  = 0b0100,
    WA  = 0b1000
};

// ARCACHE
#define ARCACHE_DEVICE_NON_BUFFERABLE               0b0000
#define ARCACHE_DEVICE_BUFFERABLE                   0b0001
#define ARCACHE_NORMAL_NON_CACHEABLE_NON_BUFFERABLE 0b0010
#define ARCACHE_NORMAL_NON_CACHEABLE_BUFFERABLE     0b0011
#define ARCACHE_WRITE_THROUGH_NO_ALLOC              0b1010
#define ARCACHE_WRITE_THROUGH_READ_ALLOC            0b1110
#define ARCACHE_WRITE_THROUGH_WRITE_ALLOC           0b1010
#define ARCACHE_WRITE_THROUGH_READ_AND_WRITE_ALLOC  0b1110
#define ARCACHE_WRITE_BACK_NO_ALLOC                 0b1011
#define ARCACHE_WRITE_BACK_READ_ALLOC               0b1111
#define ARCACHE_WRITE_BACK_WRITE_ALLOC              0b1011
#define ARCACHE_WRITE_BACK_READ_AND_WRITE_ALLOC     0b1111

// AWCACHE
#define AWCACHE_DEVICE_NON_BUFFERABLE               0b0000
#define AWCACHE_DEVICE_BUFFERABLE                   0b0001
#define AWCACHE_NORMAL_NON_CACHEABLE_NON_BUFFERABLE 0b0010
#define AWCACHE_NORMAL_NON_CACHEABLE_BUFFERABLE     0b0011
#define AWCACHE_WRITE_THROUGH_NO_ALLOC              0b0110
#define AWCACHE_WRITE_THROUGH_READ_ALLOC            0b0110
#define AWCACHE_WRITE_THROUGH_WRITE_ALLOC           0b1110
#define AWCACHE_WRITE_THROUGH_READ_AND_WRITE_ALLOC  0b1110
#define AWCACHE_WRITE_BACK_NO_ALLOC                 0b0111
#define AWCACHE_WRITE_BACK_READ_ALLOC               0b0111
#define AWCACHE_WRITE_BACK_WRITE_ALLOC              0b1111
#define AWCACHE_WRITE_BACK_READ_AND_WRITE_ALLOC     0b1111

// Protection bits
// AWPROT/ARPROT
enum AxiProt {
    PRIVILEGED  = 0b001,
    NONSECURE   = 0b010,
    INSTRUCTION = 0b100
};

// Operation status responses
// BRESP/RRESP
enum AxiResp {
    OKAY    = 0b00,
    EXOKAY  = 0b01,
    SLVERR  = 0b10,
    DECERR  = 0b11
};

#endif  // CONSTANS_HPP_
