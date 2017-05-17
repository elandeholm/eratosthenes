#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BITSTYPE uint64_t

#define BITSTYPE_SIZE sizeof(BITSTYPE)
#define BITSTYPE_BITS (8 * BITSTYPE_SIZE)
#define BITSTYPE_MASK (BITSTYPE_BITS - 1)

#define BITVAL(n) ((BITSTYPE)1) << ((n) & BITSTYPE_MASK)
#define SIEVE_PTR(sieve, n) *((sieve) + (n) / BITSTYPE_BITS)

#define MARK_COMPOSITE(sieve, n) (SIEVE_PTR(sieve, n) |= BITVAL(n))
#define TEST_COMPOSITE(sieve, n) (SIEVE_PTR(sieve, n) &  BITVAL(n))

uint64_t int_sqrt(uint64_t x)
{
	return (uint64_t)sqrt((double)x);
}

void printu64(uint64_t x)
{
	printf("%" PRIu64, x);
}

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

static BITSTYPE *eratosthenes(uint64_t n)
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

	for(uint64_t e = 4; e < n; e += 2)
	{
		MARK_COMPOSITE(sieve, e);
	}

	p = 3;
	while(p <= sqrtn)
	{
		pp = p + p;
		i = p * p;
		while(i < n)
		{
			MARK_COMPOSITE(sieve, i);
			i += pp;
		}

		p = next_prime(sieve, p, sqrtn);
	}

	return sieve;
}

static int isprime_trial_div(uint64_t i)
{
	uint64_t sqrti, j;

	if(i >= 2) // common case
	{
		if(i % 2) // assumed common case
		{
			sqrti = (uint64_t)sqrt((double)i);

			j = 3;
			while(j <= sqrti)
			{
				if(!(i % j))
				{
					return 0; // fail as early as possible
				}
				j += 2;
			}
			return 1;
		}
		else
		{
			return i == 2; // common case
		}
	}
	else
	{
		return 0; // 0 & 1 are not prime
	}
}

int main(int argc, char **argv)
{
	char *endp;
	BITSTYPE *sieve;
	uint64_t n, i, j;
	int fail;
	clock_t start, diff;
	int msec;

	if(argc > 1)
	{
		n = strtoull(argv[1], &endp, 10);
		if(*endp == '\0')
		{
			start = clock();
			sieve = eratosthenes(n);
			diff = clock() - start;

			msec = diff * 1000 / CLOCKS_PER_SEC;
			fprintf(stderr, "*** Sieving: %d.%03d seconds\n", msec / 1000, msec % 1000);

			if(sieve)
			{
				fail = 0;

				start = clock();

				for(i = 0, j = 0; i < n && !fail; ++i)
				{
					if(!TEST_COMPOSITE(sieve, i))
					{
						++j;
/*						printu64(j);
						printf(": ");
						printu64(i);
						puts("");
*/
						if(0 && !isprime_trial_div(i))
						{
							fprintf(stderr, "*** trial_div(%" PRIu64 "): sieve prime is composite!\n", i);
							fail = 1;
						}
					}
					else
					{
						if(0 && isprime_trial_div(i))
						{
							fprintf(stderr, "*** trial_div(%" PRIu64 "): sieve composite is prime!\n", i);
							fail = 1;
						}
					}
				}
				diff = clock() - start;

				msec = diff * 1000 / CLOCKS_PER_SEC;
				fprintf(stderr, "*** TEST_COMPOSITE(), %" PRIu64 " primes: %d.%03d seconds\n", j, msec / 1000, msec % 1000);

				free(sieve);
			}
		}
		else
		{
			printf("Invalid number: %s\n", argv[1]);
		}
	}
	else
	{
		puts("Argument expected");
	}
	return 0;
}
