#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "eratosthenes.h"

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

typedef struct
{
	int interactive;
	int list;
	int time;
	int verify;
	int64_t n;
} driver_args_t;

static int parse_args(int argc, char * const argv[], driver_args_t *da)
{
	int c, fail;
	int option_index;
	static struct option long_options[] =
	{
		{ "interactive",  0, 0, 0 },
		{ "list",         0, 0, 0 },
		{ "time",         0, 0, 0 },
		{ "verify",       0, 0, 0 },
		{ "n",            1, 0, 0 },
		{              0, 0, 0, 0 }
	};

	da->interactive = 0;
	da->list        = 0;
	da->time        = 0;
	da->verify      = 0;
	da->n           = 1000;

	fail = 0;

	while(EOF != (c = getopt_long(argc, argv, "iltvn", long_options, &option_index)))
	{
		switch(c)
		{
			case 0:
				switch(option_index)
				{
					case 0:       /* --interactive */
						da->interactive = 1;
						break;

					case 1:       /* --list */
						da->list = 1;
						break;

					case 2:       /* --time */
						da->time = 1;
						break;

					case 3:       /* --verify */
						da->verify = 1;
						break;

					case 4:       /* --n */
						if(optarg)
						{
							char *endp;
							da->n = strtoll(optarg, &endp, 10);
							if(*endp != '\0')
							{
								fprintf(stderr, "invalid n: %s\n", optarg);
								fail = -1;
							}
						}
						break;

					default:
						fprintf(stderr, "unrecognized long option\n");
						fail = -1;
						break;
				}
				break;

			case 'i':
				da->interactive = 1;
				break;

			case 'l':
				da->list = 1;
				break;

			case 't':
				da->time = 1;
				break;

			case 'v':
				da->verify = 1;
				break;

			default:
				fprintf(stderr, "unrecognized option '%c'\n", c);
				fail = -1;
				break;
		}
	}

	if(optind < argc)
    {
		char *endp;
		da->n = strtoll(argv[optind], &endp, 10);
		if(*endp != '\0')
		{
			fprintf(stderr, "invalid n: %s\n", argv[optind]);
			fail = -1;
		}
		++optind;
 	}

	if(optind < argc)
	{
		fprintf(stderr, "too many arguments!\n");
		fail = -1;
	}

	if(!fail && da->n < 10)
	{
		da->n = 10;
	}

	return fail;
}

static const char *eat_white(const char *buffer)
{
	int i = 0;
	char c;

	do
	{
		c = buffer[i];
		++i;

	} while(isspace(c));

	return buffer + i - 1;
}

static void interactive(BITSTYPE *sieve, uint64_t n, int time)
{
	int quit = 0, is_prime;
	char *rl;
	char const *line;
	char *endp;
	int64_t i, j;

	rl_bind_key('\t', rl_abort); // disable auto-complete

	while(!quit)
	{
		if((rl = readline("> ")))
		{
			line = eat_white(rl);

			if(*line != '\0')
			{
				add_history(line);

				if(*line == '#')
				{
					// find k:th prime

					uint64_t k;
					int oor = 0;

					k = strtoll(line + 1, &endp, 10);
					if(*endp == '\0')
					{
						j = 2;
						i = 3;

						if(k > 1)
						{
							while(j < k && !oor)
							{
								i += 2;
								if(i < n)
								{
									if(!TEST_COMPOSITE(sieve, i))
									{
										++j;
									}
								}
								else
								{
									oor = 1;
								}
							}
						}
						else if(k == 1)
						{
							j = 1;
							i = 2;
						}
						else
						{
							oor = 1;							
						}

						if(oor)
						{
							fprintf(stderr, "out of range: %s\n", line);
						}
						else
						{
							printf("%" PRIu64 ": %" PRIu64 "\n", j, i);
						}
					}
				}
				else
				{
					// test primality

					i = strtoll(line, &endp, 10);
					if(*endp == '\0')
					{
						if(i < n)
						{
							if(i % 2)
							{
								is_prime = !TEST_COMPOSITE(sieve, i);
							}
							else
							{
								is_prime = i == 2;
							}

							puts(is_prime ? "prime" : "composite");
						}
						else
						{
							fprintf(stderr, "out of range: %s\n", line);
						}
					}
					else
					{
						fprintf(stderr, "invalid number: %s\n", line);
					}
				}
			}

			free(rl);
		}
		else
		{
			quit = 1;
		}
	}
}

static void list(BITSTYPE *sieve, uint64_t n, int time)
{
	uint64_t i, j;
	clock_t start, diff;
	int msec;

	start = clock();

	j = 1;
	i = 2;

	printf("%" PRIu64 ": %" PRIu64 "\n", j, i);

	for(i = 3, j = 1; i < n; i += 2)
	{
		if(!TEST_COMPOSITE(sieve, i))
		{
			++j;
			printf("%" PRIu64 ": %" PRIu64 "\n", j, i);
		}
	}

	fprintf(stderr, "*** list, %" PRIu64 " primes", j);

	if(time)
	{
		diff = clock() - start;
		msec = diff * 1000 / CLOCKS_PER_SEC;
		fprintf(stderr, ": %d.%03d seconds\n", msec / 1000, msec % 1000);
	}
	else
	{
		fputs("\n", stderr);
	}
}

static void verify(BITSTYPE *sieve, uint64_t n, int time)
{
	uint64_t i, j;
	clock_t start, diff;
	int msec;
	int fail = 0;

	start = clock();

	for(i = 3, j = 0; i < n && !fail; i += 2)
	{
		if(TEST_COMPOSITE(sieve, i))
		{
			if(isprime_trial_div(i))
			{
				fprintf(stderr, "*** trial_div(%" PRIu64 "): sieve prime is composite!\n", i);
				fail = 1;
			}
		}
		else
		{
			++j;

			if(!isprime_trial_div(i))
			{
				fprintf(stderr, "*** trial_div(%" PRIu64 "): sieve composite is prime!\n", i);
				fail = 1;
			}
		}
	}

	fprintf(stderr, "*** verify, %" PRIu64 " primes", j);

	if(time)
	{
		diff = clock() - start;
		msec = diff * 1000 / CLOCKS_PER_SEC;
		fprintf(stderr, ": %d.%03d seconds\n", msec / 1000, msec % 1000);
	}
	else
	{
		fputs("\n", stderr);
	}
}

int main(int argc, char * const argv[])
{
	int fail;
	driver_args_t da;
	BITSTYPE *sieve;

	fail = 0;

	if(!parse_args(argc, argv, &da))
	{
		clock_t start, diff;
		int msec;

		fprintf(stderr, "*** Sieving %" PRIu64 " numbers...\n", da.n);

		start = clock();
		sieve = eratosthenes(da.n);

		if(sieve == NULL)
		{
			fprintf(stderr, "calloc() failed\n");
			fail = -1;
		}
		else
		{
			fprintf(stderr, "*** Done");

			if(da.time)
			{
				diff = clock() - start;
				msec = diff * 1000 / CLOCKS_PER_SEC;
				fprintf(stderr, ": %d.%03d seconds\n", msec / 1000, msec % 1000);
			}
			else
			{
				fputs("\n", stderr);
			}

			if(da.list)
			{
				list(sieve, da.n, da.time);
			}

			if(da.verify)
			{
				verify(sieve, da.n, da.time);
			}

			if(da.interactive)
			{
				interactive(sieve, da.n, da.time);
			}

			free(sieve);
		}
	}
	else
	{
		fputs("*** Errors parsing arguments\n", stderr);
		fail = -1;
	}

	if(fail)
	{
		fputs("*** Operation aborted\n", stderr);
	}

	return fail;
}
