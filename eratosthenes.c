#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>

#define BITSTYPE uint64_t

#define BITSTYPE_SIZE sizeof(BITSTYPE)
#define BITSTYPE_BITS (8 * BITSTYPE_SIZE)
#define BITSTYPE_MASK (BITSTYPE_BITS - 1)

#define BITVAL(n) ((BITSTYPE)1) << ((n) & BITSTYPE_MASK)
#define SIEVE_PTR(sieve, n) *((sieve) + (n) / BITSTYPE_BITS)

#define MARK_COMPOSITE(sieve, n) (SIEVE_PTR(sieve, n) |= BITVAL(n))
#define TEST_COMPOSITE(sieve, n) (SIEVE_PTR(sieve, n) &  BITVAL(n))

static uint64_t next_prime(BITSTYPE *sieve, uint64_t prev, uint64_t sqrtn)
{
	uint64_t i;

	i = prev + 2;
	while((i <= sqrtn) && TEST_COMPOSITE(sieve, i))
	{
		i += 2;
	}

	return i;
}

BITSTYPE *eratosthenes(uint64_t n)
{
	BITSTYPE *sieve;
	uint64_t sqrtn, e, p, pp, i;

	if(n < 3)
	{
		return NULL;
	}

	sieve = calloc(n / BITSTYPE_SIZE, sizeof(BITSTYPE));

	if(!sieve)
	{
		return NULL;
	}

	sqrtn = (uint64_t)sqrt((double)n);

	MARK_COMPOSITE(sieve, 0);
	MARK_COMPOSITE(sieve, 1);

	for(e = 4; e < n; e += 2)
	{
		MARK_COMPOSITE(sieve, e);
	}

	p = 3;
	while(p <= sqrtn)
	{
		pp = p + p;
		i = p * p;

		while(i < n) // unrolling this loop makes things ~4 % faster on -O3
		{
			MARK_COMPOSITE(sieve, i);
			i += pp;
		}
		p = next_prime(sieve, p, sqrtn);
	}

	return sieve;
}
