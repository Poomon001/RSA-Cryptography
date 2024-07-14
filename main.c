#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>

int32_t get_32bit_prime(int bits) {
    gmp_randstate_t state;
    mpz_t prime;
    unsigned long seed = 50; // Use a proper random seed in real applications

    // Initialize state and prime
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    mpz_init(prime);

    // Generate a random number with the specified bit size
    mpz_urandomb(prime, state, bits);

    // Find the next prime greater than the random number
    mpz_nextprime(prime, prime);

    // Convert prime to an integer
    uint32_t result = mpz_get_ui(prime); // Use uint32_t to fit the result

    // Clear memory
    mpz_clear(prime);
    gmp_randclear(state);

    return (int32_t)result;
}

int main(void) {
    // P and Q are two large prime numbers
    const int maxBits = 32;
    const int minBits = 3;
    const int32_t p = get_32bit_prime(maxBits);
    const int32_t q = get_32bit_prime(maxBits);

    //(P - 1)*(Q - 1) is an even number
    int64_t pq = (int64_t) (p - 1) * (q - 1);
    //E > 1 and E < P*Q
    int randomBits = (rand() % (maxBits - minBits + 1)) + minBits;
    //E is an odd prime number
    // E and (P - 1)*(Q - 1) are relatively prime (meaning they have no prime factors in common)
    int32_t e;
    do {
        e = get_32bit_prime(randomBits);
    } while (pq % e == 0);

    printf("%d, %d, %d, %lld\n", p, q, e, pq);

    return 0;
}

