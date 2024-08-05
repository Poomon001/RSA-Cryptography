#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>
#include <sys/time.h>

uint64_t montgomery_modular_multiplication(register uint64_t x, register uint64_t y, register uint64_t M);
uint64_t modular_exponentiation(register uint64_t p, register uint64_t e, register uint64_t m);
int32_t compute_x(uint32_t phi, uint16_t e);
int mod_inverse(int e, int phi);
uint16_t get_16bit_prime(int bits, int seed);
void gcd_extended(register int e, register int phi, int* restrict x, int* restrict y);

/**
 * Calculates coefficient integer of integers e and phi: e*x + phi*⋅y = gcd(e,phi)
 * Parameters: int e - the prime number
 *           : int phi - (p - 1)(q - 1)
 *           : int* x - the coefficient integer of e
 *           : int* y - the coefficient integer of phi
 * Returns: None
 * */
// added restrict to ensure that x and y pointers do not overlap
void gcd_extended(register int e, register int phi, int* restrict x, int* restrict y) {
    if (e == 0) {
        *x = 0;
        *y = 1;
        return;
    }

    int x1, y1;
    gcd_extended(phi % e, e, &x1, &y1);

    *x = y1 - (phi / e) * x1;
    *y = x1;
}

/**
 * Calculates mod_inverse of E mod PHI
 * Parameters: int e - the prime number
 *           : int phi - (p - 1)(q - 1)
 * Returns: int - mod_inverse of E mod PHI
 * */
int mod_inverse(int e, int phi) {
    int x, y;
    gcd_extended(e, phi, &x, &y);

    // x is the modular multiplicative inverse of e % phi
    return (x % phi + phi) % phi;
}

/**
 * Computes X for the equation (X * phi + 1) % E = 0
 * Parameters: uint16_t e - the prime number
 *           : uint32_t phi - (p - 1)(q - 1)
 * Returns: int - x for the equation
 * */

int32_t compute_x(uint32_t phi, uint16_t e) {
    // Compute k such that k * e % phi = 1
    uint32_t k = mod_inverse(e, phi);

    // Compute X for the equation
    // if (k * e) % phi = 1, then k * e - 1 is divisible by phi. Then x is guaranteed to be an integer
    int x = (k * e - 1) / phi;

    // from (X * phi + 1) % E = 0, then (X * phi + 1) = kE, then k * phi = kE - 1, then X = (kE - 1) / phi
    return x;
}

/**
 * Generates a random prime number with a specified number of bits using the GMP library
 * Parameters: int bits - the number of bits in the prime number
 *           : int seed - the seed for the random number generator
 * Returns: uint16_t - the generated prime number
 * */
uint16_t get_16bit_prime(int bits, int seed) {
    gmp_randstate_t state;
    mpz_t prime;

    // initializes the random state
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    mpz_init(prime);

    // Generate a random number with the specified bits
    mpz_urandomb(prime, state, bits);

    // Find the next greater prime number then store it back in prime
    mpz_nextprime(prime, prime);

    // Convert prime to an integer
    // Use uint16_t to fit the result
    uint16_t result = mpz_get_ui(prime);

    // Clear memory
    mpz_clear(prime);
    gmp_randclear(state);

    return result;
}

/** With no optimization:
 * inefficient with exponential operator
 * Can only support 3 bits input with limited range of t values
 * **/
/*
int32_t bruteforce_rsa_cryptography(int32_t t, int32_t e, int32_t pq) {
    return (int32_t)pow(t, e) % pq;
}*/

/** With only modular_exponentiation:
 * faster with no exponential operator but still have multiplication operator
 * can only support upto 8 bits input
 * **/
//int32_t modular_exponentiation(uint64_t p, uint64_t e, uint64_t m){
//    uint64_t z = 1;
//    p = p % m; //to ensure that p does not become too large than 32 bits
//    while(e > 0){
//        if ((e & 1) == 1){
//            z = (z * p) % m;
//        }
//
//        e = e >> 1;
//        p = (p * p) % m;
//    }
//    return (int32_t)z;
//}

/** With modular_exponentiation and montgomery_modular_multiplication: **/
/**
 * Computes the modular exponentiation of a number, an optimal solution to a = b^c mod m
 * Parameters: uint64_t p - the base number
 *          : uint64_t e - the exponent
 *          : uint64_t m - the modulus
 * Returns: uint64_t z - the result of the modular exponentiation
 * */
uint64_t modular_exponentiation(register uint64_t p, register uint64_t e, register uint64_t m) {
    // r = 2^m bits
    uint64_t m_bits = log2(m) + 1;
    uint64_t r = 1ULL << m_bits;

    // operator strength reduction, replacing multiplication with bit-shifting
    // r*r mod m to pre-scale values later
    uint64_t r2 = (r << m_bits) % m;

    uint64_t z = 1;

    // to ensure that p does not become larger than 32 bits
    p = p % m;

    // loop through the number of bits in e
    while(e > 0){
        // p' = p * r*r * r^-1 mod m = p * r mod m
        uint64_t p_prime = montgomery_modular_multiplication(p, r2, m);

        // if right-most bit of e is 1
        // using predicate operations to replace boolean operations
        if (e & 1){
            // z' = z * r*r * r^-1 mod m = z * r mod m
            uint64_t z_prime = montgomery_modular_multiplication(z, r2, m);

            // z = z' * p' * r^-1 mod m = z * p * r mod m
            z = montgomery_modular_multiplication(z_prime, p_prime, m);

            // z = z * p * r * r^-1 mod m = z * p mod m
            z = montgomery_modular_multiplication(z, 1, m);
        }
        e = e >> 1;

        // p = p' * p' * r^-1 mod m = p * r * p mod m
        p = montgomery_modular_multiplication(p_prime, p_prime, m);

        // p = p * p * r * r^-1 mod m = p * p mod m
        p = montgomery_modular_multiplication(p, 1, m);
    }
    return z;
}

/** Montgomery Modular Multiplication Algorithm **/
/**
 * Computes the Montgomery Modular Multiplication of a number, an optimal solution to x * y * r^-1 mod m
 * Parameters: uint64_t x - the first number
 *         : uint64_t y - the second number
 *         : uint64_t M - the modulus
 * Returns: uint64_t t - the result of the Montgomery Modular Multiplication
 * */

uint64_t montgomery_modular_multiplication(register uint64_t x, register uint64_t y, register uint64_t M) {
    register uint64_t m = M;
    register uint64_t t = 0;
    register uint64_t y_and_1 = y & 1; // precompute y & 1
    register uint64_t n, xy, nm, x_and_1;


    // Loop through the number of m bits in pq
    // loop unrolled thrice
    // software pipelining by reordering instructions
    while (m > 2) {
        // First iteration
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace multiplications with predicate operations and 2's complement
        x_and_1 = (x & 1);
        xy = -x_and_1 & y;
        nm = (-n & M);
        t = (t + xy + nm) >> 1;
        x = x >> 1;

        // Second iteration (unrolled)
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace multiplications with predicate operations and 2's complement
        x_and_1 = (x & 1);
        xy = -x_and_1 & y;
        nm = (-n & M);
        t = (t + xy + nm) >> 1;
        x = x >> 1;

        // third iteration (unrolled)
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace multiplications with predicate operations and 2's complement
        x_and_1 = (x & 1);
        xy = -x_and_1 & y;
        nm = (-n & M);
        t = (t + xy + nm) >> 1;
        x = x >> 1;

        // improve by shifting 3 bits at once
        m = m >> 3;
    }

    // remaining iterations (1 to 2)
    while (m > 0) {
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace multiplications with predicate operation and 2's complement
        x_and_1 = (x & 1);
        xy = -x_and_1 & y;
        nm = (-n & M);
        t = (t + xy + nm) >> 1;
        x = x >> 1;

        m = m >> 1;
    }

    // branch elimination
    t -= (t >= M) * M;

    return t;
}

int main(void) {
    struct timeval start_time, end_time;

    // start time
    gettimeofday(&start_time, NULL);

    // P and Q are two large prime numbers
    // NOTE: The max bits must be 16 bits or less because when p and q are 16 bits, it produces a 32 bit pq value
    // When pq is 32 bits, the r value is 2^32 which is larger than 32 bits
    // When r * r is calculated, it becomes larger than 64 bits
    // So when p and q are larger than 16 bits, it results in r * r being larger than 64 bits
    const uint16_t p = get_16bit_prime(16, 99);
    const uint16_t q = get_16bit_prime(16, 5);

    //(P - 1) * (Q - 1) is an even number
    const uint32_t phi = (p - 1) * (q - 1);

    //E > 1 and E < P*Q
    //E is an odd prime number
    //E and (P - 1)*(Q - 1) are relatively prime (meaning they have no prime factors in common)
    uint16_t e;
    do {
        e = get_16bit_prime(16, 99);
    } while (phi % e == 0);

    uint32_t x = compute_x(phi, e);

    // Compute D = (X(P −1)(Q−1)+1)/E where d is an integer
    uint64_t d = (uint64_t)x * phi + 1;
    d = d / (uint64_t)e;

    // t is the plaintext (a positive integer) and t is a message being encrypted
    // t must be less than the modulus PQ
    // check that t is less than p * q
    uint32_t t = 1073938441;

    uint64_t pq = p * q;
    
    // branch elimination: minor performance improvements, but make the code less readable
    (pq) < t && (printf("Our plain text t must be less than p * q\n"), exit(-1), 0);

    // encryption of plaintext T, C = T^E mod PQ
    uint64_t c_encrypted = modular_exponentiation(t, e, pq);

    printf("c_encrypted: %llu\n", c_encrypted);

    // decryption of the ciphertext C, T = C^D mod PQ
    uint64_t t_decrypted = modular_exponentiation(c_encrypted, d, pq);

    printf("t_decrypted: %llu\n", t_decrypted);

    // end time
    gettimeofday(&end_time, NULL);
    printf("Total Time: %ld microseconds\n", ((end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)));

    return 0;
}