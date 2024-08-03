#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>

uint64_t montgomery_modular_multiplication(uint64_t z, uint64_t p, uint64_t pq);
uint16_t get_16bit_prime(int bits, int seed);
int mod_inverse(int e, int phi);
int gcd_extended(int e, int phi, int *x, int *y);
int32_t compute_x(uint32_t phi, uint16_t e);
uint64_t modular_exponentiation(uint64_t p, uint64_t e, uint64_t m);

/*
 * Calculates the greatest common divisor (GCD) of two integers using the Extended Euclidean Algorithm: a⋅x+b⋅y = GCD(a,b)
 * Parameters: int bits - the number of bits in the prime number
 *           : int seed - the seed for the random number generator
 * Returns: uint16_t - the generated prime number
 * */
int gcd_extended(int e, int phi, int *x, int *y) {
    if (e == 0) {
        *x = 0;
        *y = 1;
        return phi;
    }

    int x1, y1;
    int gcd = gcd_extended(phi % e, e, &x1, &y1);

    *x = y1 - (phi / e) * x1;
    *y = x1;

    return gcd;
}

// D is called the multiplicative inverse of E.
int mod_inverse(int e, int phi) {
    int x, y;
    int gcd = gcd_extended(e, phi, &x, &y);
    if (gcd != 1) {
        printf("Modular inverse does not exist\n");
        return -1;
    }
    return (x % phi + phi) % phi;
}

// from D = (1 + X * (P - 1) * (Q - 1)) / E where X and D are integers, find X
int32_t compute_x(uint32_t phi, uint16_t e) {
    // Compute k such that k * e % phi = 1
    uint64_t k = mod_inverse(e, phi);

    // Compute X for the equation
    // if (k * e) % phi = 1, then k * e - 1 is divisible by phi. Then x is garanteed to be an integer
    int x = (k * e - 1) / phi;

    // from (X * phi + 1) % E = 0, then (X * phi + 1) = nE, then X * phi = nE - 1, then X = (nE - 1) / phi
    return x;
}

/*
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
uint64_t modular_exponentiation(uint64_t p, uint64_t e, uint64_t m){
    // r = 2^m
    uint64_t r = 1ULL << ((uint64_t)(log2(m)) + 1);

    // r*r mod m to pre-scale values later
    uint64_t r2 = (r * r) % m;
    printf("r: %llu, r2: %llu\n", r, r2);

    uint64_t z = 1;

    // to ensure that p does not become larger than 32 bits
    p = p % m;
    printf("p: %llu\n", p);

    // montgomery_modular_multiplication(x, y, m) = x * y * r^-1 mod m
    // loop through the number of bits in e
    while(e > 0){
        printf("e: %llu\n", e);
        // p' = p * r*r * r^-1 mod m = p * r mod m
        uint64_t p_prime = montgomery_modular_multiplication(p, r2, m);

        // if ith bit of e is 1
        if ((e & 1) == 1){
            // z' = z * r*r * r^-1 mod m = z * r mod m
            uint64_t z_prime = montgomery_modular_multiplication(z, r2, m);
            printf("%llu * %llu mod %llu\n", z, p, m);

            // z = z' * p' * r^-1 mod m = z' * p' mod m
            z = montgomery_modular_multiplication(z_prime, p_prime, m);

            // z = z * p * r * r^-1 mod m = z * p mod m
            z = montgomery_modular_multiplication(z, 1, m);
            printf("result z: %llu\n", z);
        }
        e = e >> 1;
        printf("%llu * %llu mod %llu\n", p, p, m);

        // p = p' * p' * r^-1 mod m = p' * p' mod m
        p = montgomery_modular_multiplication(p_prime, p_prime, m);

        // p = p * p * r * r^-1 mod m = p * p mod m
        p = montgomery_modular_multiplication(p, 1, m);
        printf("result p: %llu\n", p);
    }
    printf("\nresult: %llu\n", z);
    return z;
}

//Montgomery Modular Multiplication Algorithm
uint64_t montgomery_modular_multiplication(uint64_t x, uint64_t y, uint64_t M) {
    uint64_t m = M;
    uint64_t t = 0;
    uint64_t n;

    // first one iteration outside the loop so that X(i) = X(0)
    n = ((t & 1)) ^ ((x & 1) & (y & 1));
    t = (t + ((x & 1) * y) + (n * M)) >> 1;
    m = m >> 1;
    x = x >> 1;

    // loop through the number of bits in M (M is PQ)
    while(m > 0){
        // n = T(0) XOR (X(i) AND Y(0))
        n = ((t & 1)) ^ ((x & 1) & (y & 1));

        // T = (T + X(i)Y + nM) ≫ 1
        t = (t + ((x & 1) * y) + (n * M)) >> 1;

        // since x is always less than m, (t < PQ)
        // we can get next bit of x by shifting x to the right
        x = x >> 1;
        m = m >> 1;
    }

    // reduce t by M if t is greater than or equal to M
    if (t >= M) {
        t = t - M;
    }
    return t;
}

int main(void) {
    // P and Q are two large prime numbers
    // NOTE: The max bits must be 16 bits or less because when p and q are 16 bits, it produces a 32 bit pq value
    // When pq is 32 bits, the r value is 2^32 which is larger than 32 bits
    // When r * r is calculated, it becomes larger than 64 bits
    // So when p and q are larger than 16 bits, it results in r * r being larger than 64 bits
    const int maxBits = 16;
    const int minBits = 3;
    const uint16_t p = get_16bit_prime(maxBits, 99);
    const uint16_t q = get_16bit_prime(maxBits, 5);

    //(P - 1) * (Q - 1) is an even number
    const uint32_t phi = (p - 1) * (q - 1);

    //E > 1 and E < P*Q
    const int randomBits = rand() % (maxBits - minBits + 1) + minBits;

    //E is an odd prime number
    //E and (P - 1)*(Q - 1) are relatively prime (meaning they have no prime factors in common)
    uint16_t e;
    do {
        e = get_16bit_prime(randomBits, 99);
    } while (phi % e == 0);

    uint32_t x = compute_x(phi, e);
    printf("x: %d\n", x);

    // Compute D = (X(P −1)(Q−1)+1)/E = (x * phi + 1) / e
    uint64_t d = (uint64_t)x * phi + 1;
    d = d / e;

    // t is the plaintext (a positive integer) and t is a message being encrypted
    // t must be less than the modulus PQ
    // check that t is less than p * q
    uint64_t t = 1845588466;
    if ((p * q) < t){
        printf("Our plain text t must be less than p * q\n");
        exit(-1);
    }
    uint64_t pq = p * q;
    printf("d:%llu, p: %d, q: %d, e: %d, pq: %d, (p-1)(q-1): %d\n", d, p, q, e, p * q, phi);

    // C = T^E mod PQ, i.e., the encryption of the plaintext T
    uint64_t c_encrypted = modular_exponentiation(t, e, pq);

    printf("c_encrypted: %llu\n", c_encrypted);

    // T = C^D mod PQ, i.e., the decryption of the ciphertext C
    uint64_t t_decrypted = modular_exponentiation(c_encrypted, d, pq);

    printf("t_decrypted: %llu\n", t_decrypted);

    return 0;
}

