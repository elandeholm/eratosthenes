#define BITSTYPE uint64_t

#define BITSTYPE_SIZE sizeof(BITSTYPE)
#define BITSTYPE_BITS (8 * BITSTYPE_SIZE)
#define BITSTYPE_MASK (BITSTYPE_BITS - 1)

#define BITVAL(n) ((BITSTYPE)1) << (((n) >> 1) & BITSTYPE_MASK)
#define SIEVE_PTR(sieve, n) *((sieve) + ((n) >> 1) / BITSTYPE_BITS)

#define MARK_COMPOSITE(sieve, n) (SIEVE_PTR(sieve, n) |= BITVAL(n))
#define TEST_COMPOSITE(sieve, n) (SIEVE_PTR(sieve, n) &  BITVAL(n))

uint64_t int_sqrt(uint64_t x);
uint64_t next_prime(BITSTYPE *sieve, uint64_t prev, uint64_t sqrtn);
BITSTYPE *eratosthenes(uint64_t n);
