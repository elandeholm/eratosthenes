#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>

#include "eratosthenes.h"

uint64_t int_sqrt(uint64_t x)
{
	return (uint64_t)sqrt((double)x);
}

uint64_t next_prime(BITSTYPE *sieve, uint64_t prev, uint64_t sqrtn)
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
	uint64_t sqrtn, p, pp, i;
	size_t nmemb;

	if(n < 3)
	{
		return NULL;
	}

	nmemb = 1 + (n >> 1) / BITSTYPE_SIZE;
	sieve = calloc(nmemb, sizeof(BITSTYPE));

	if(!sieve)
	{
		return NULL;
	}

	sqrtn = int_sqrt(n);

	MARK_COMPOSITE(sieve, 1);

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
