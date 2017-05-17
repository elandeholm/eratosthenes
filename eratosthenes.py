from sys import argv
from math import sqrt

def eratosthenes(n):
	sieve = [0]*n
	sqrtn = int(sqrt(n))

	def next_prime(sieve, prev):
		i = prev + 2
		while i <= sqrtn and sieve[i]:
			i += 2
		return i

	sieve[0] = 1
	sieve[1] = 1
	sieve[2] = 0

	for e in range(4, n, 2):
		sieve[e] = 1

	p = 3
	while p <= sqrtn:
		for i in range(p*p, n, 2*p):
			sieve[i] = 1

		p = next_prime(sieve, p)

	return sieve

def trial_div(i):
	if i % 2:
		sqrti = int(sqrt(i))

		for j in range(3, sqrti, 2):
			if not (i % j):
				print('{} is not a prime!'.format(i))

	else:
		if i != 2:
			print('{} is not a prime!'.format(i))

if __name__ == '__main__':
	n = int(argv[1])
	sieve = eratosthenes(n)
	j = 1
	for i, c in enumerate(sieve):
		if not c:
			print(j, i)
			trial_div(i)
			j += 1
