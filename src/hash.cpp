#include "hash.h"

namespace simhash {

    /*
    * Rot: This function reduces codes in HashLittle. Don't be too care of it.
    */
    static inline uint32_t Rot(uint32_t x, uint32_t k)
    {
        return ((x << k) | (x >> (32U - k)));
    }

    /*
    * HashLittle: return 2 32-bit hash values
    *
    * This function returns two 32-bit hash values instead of just one.This is
    * good enough for hash table lookup with 2^^64 buckets, or if you want a
    * second hash if you're not happy with the first, or if you want a probably
    * unique 64-bit ID for the key. *pc is better mixed than *pb, so use *pc 
    * first. If you want a 64-bit value do something like:
    *     "*pc + (((uint64_t)*pb) <<32)".
    */
    static void HashLittle(const void *key, uint32_t length, uint32_t *pc,
        uint32_t *pb)
    {
        uint32_t a, b, c;
        union { const void *ptr; uint32_t i; } u;
        /* Set up the internal state */
        a = b = c = 0xdeadbeef + ((uint32_t)length) + *pc;
        c += *pb;
        u.ptr = key;
        const uint8_t *k = (const uint8_t *)key;
        while (length > 12) {
            a += k[0];
            a += ((uint32_t)k[ 1]) << 8;
            a += ((uint32_t)k[ 2]) << 16;
            a += ((uint32_t)k[ 3]) << 24;
            b += k[4];
            b += ((uint32_t)k[ 5]) << 8;
            b += ((uint32_t)k[ 6]) << 16;
            b += ((uint32_t)k[ 7]) << 24;
            c += k[8];
            c += ((uint32_t)k[ 9]) << 8;
            c += ((uint32_t)k[10]) << 16;
            c += ((uint32_t)k[11]) << 24;

            a -= c;  a ^= Rot(c,  4);  c += b;
            b -= a;  b ^= Rot(a,  6);  a += c;
            c -= b;  c ^= Rot(b,  8);  b += a;
            a -= c;  a ^= Rot(c, 16);  c += b;
            b -= a;  b ^= Rot(a, 19);  a += c;
            c -= b;  c ^= Rot(b,  4);  b += a;

            length -= 12;
            k += 12;
        }

        switch (length)
        {
        case 12: c += ((uint32_t)k[11]) << 24;
        case 11: c += ((uint32_t)k[10]) << 16;
        case 10: c += ((uint32_t)k[ 9]) << 8;
        case  9: c += k[8];
        case  8: b += ((uint32_t)k[ 7]) << 24;
        case  7: b += ((uint32_t)k[ 6]) << 16;
        case  6: b += ((uint32_t)k[ 5]) << 8;
        case  5: b += k[4];
        case  4: a += ((uint32_t)k[ 3]) << 24;
        case  3: a += ((uint32_t)k[ 2]) << 16;
        case  2: a += ((uint32_t)k[ 1]) << 8;
        case  1: a += k[0];
            break;
        case 0: *pc = c; *pb = b; return;
        }

        c ^= b; c -= Rot(b, 14);
        a ^= c; a -= Rot(c, 11);
        b ^= a; b -= Rot(a, 25);
        c ^= b; c -= Rot(b, 16);
        a ^= c; a -= Rot(c, 4);
        b ^= a; b -= Rot(a, 14);
        c ^= b; c -= Rot(b, 24);

        *pc = c; *pb = b;
    }

    hash_t JenkinsHash(const std::string &str)
    {
        uint32_t a = 0, b = 0;
        HashLittle(reinterpret_cast<const void*>(str.c_str()),
            static_cast<uint32_t>(str.size()), &a, &b);
        return static_cast<uint64_t>(a) | (static_cast<uint64_t>(b) << 32);
    }
}
