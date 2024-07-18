#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>

int extended_euclidean(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    int x1, y1;
    int gcd = extended_euclidean(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

// Function to compute modular multiplicative inverse
int mod_inverse(int a, int m) {
    int x, y;
    int gcd = extended_euclidean(a, m, &x, &y);
    if (gcd != 1) {
        printf("Modular inverse does not exist\n");
        return -1;
    }
    return (x % m + m) % m;
}

// from D = (1 + X * (P - 1) * (Q - 1)) / E where X and D are integers
// let (P - 1) * (Q - 1) = Y
// D = (1 + x * Y) / E
// (1 + X * Y) = x * E where k is an integer
// X * Y = x * E - 1
// (x * E) % Y = 1 % Y
// x % y = E^-1 % Y

// Function to compute X
int32_t compute_x(int32_t y, int32_t e) {
    int k = mod_inverse(e, y);
    if (k == -1) return -1;  // Error case

    int x = (k * e - 1) / y;

    // Ensure X is positive
    while (x <= 0) {
        x += y;
    }

    return x;
}

int32_t get_32bit_prime(int bits) {
    gmp_randstate_t state;
    mpz_t prime;
    unsigned long seed = 12345; // Use a proper random seed in real applications

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
    const int maxBits = 16;
    const int minBits = 3;
    const int32_t p = get_32bit_prime(maxBits);
    const int32_t q = get_32bit_prime(maxBits);

    //(P - 1)*(Q - 1) is an even number
    int32_t pq = (p - 1) * (q - 1);
    //E > 1 and E < P*Q
    const int randomBits = (rand() % (maxBits - minBits + 1)) + minBits;
    //E is an odd prime number
    // E and (P - 1)*(Q - 1) are relatively prime (meaning they have no prime factors in common)
    int32_t e;
    do {
        e = get_32bit_prime(randomBits);
    } while (pq % e == 0);

    printf("%d, %d, %d, %d\n", p, q, e, pq);

    int32_t x = compute_x(pq, e);
    double d = ((double)(x * pq) + 1) / e;
    printf("D: %.2f\n", d);

    return 0;
}

