#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>

uint64_t montgomery_modular_multiplication(uint64_t z, uint64_t p, uint64_t pq);
uint64_t get_32bit_prime(int bits, int seed);

/*
 * Calculates the greatest common divisor (GCD) of two integers using the Extended Euclidean Algorithm: a⋅x+b⋅y = GCD(a,b)
 * Parameters: int bits - the number of bits in the prime number
 *           : int seed - the seed for the random number generator
 * Returns: uint16_t - the generated prime number
 * */
int gcd_extended(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    int x1, y1;
    int gcd = gcd_extended(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

// Function to compute modular multiplicative inverse
int mod_inverse(int a, int m) {
    int x, y;
    int gcd = gcd_extended(a, m, &x, &y);
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
// (x * E) % Y = 1 % Y => E * X + Y * k = 1
// x % y = E^-1 % Y

// Function to compute X
int32_t compute_x(uint32_t y, uint16_t e) {
    uint64_t k = mod_inverse(e, y);
    if (k == -1) return -1;  // Error case

    int x = (k * e - 1) / y;

    // Ensure X is positive
    while (x <= 0) {
        x += y;
    }

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
    uint64_t r = 1ULL << ((uint64_t)(log2(m)) + 1);
    uint64_t r2 = (r * r) % m;
    printf("r: %llu, r2: %llu\n", r, r2);
    uint64_t z = 1;

    p = p % m; //to ensure that p does not become too large than 32 bits
    printf("p: %llu\n", p);
    while(e > 0){
        printf("e: %llu\n", e);
        uint64_t p_prime = montgomery_modular_multiplication(p, r2, m);
        if ((e & 1) == 1){
            uint64_t z_prime = montgomery_modular_multiplication(z, r2, m);
           printf("%llu * %llu mod %llu\n", z, p, m);
            z = montgomery_modular_multiplication(z_prime, p_prime, m);
            z = montgomery_modular_multiplication(z, 1, m);
           printf("result z: %llu\n", z);
        }
        e = e >> 1;
       printf("%llu * %llu mod %llu\n", p, p, m);
        p = montgomery_modular_multiplication(p_prime, p_prime, m);
        p = montgomery_modular_multiplication(p, 1, m);
       printf("result p: %llu\n", p);
    }
    printf("\nresult: %llu\n", z);
    return z;
}

uint64_t montgomery_modular_multiplication(uint64_t x, uint64_t y, uint64_t M) {
    uint64_t m = M;
    uint64_t t = 0;
    uint64_t n;
    n = ((t & 1)) ^ ((x & 1) & (y & 1));
    t = (t + ((x & 1) * y) + (n * M)) >> 1;
    m = m >> 1;
    x = x >> 1;

    while(m > 0){
        n = ((t & 1)) ^ ((x & 1) & (y & 1));
        t = (t + ((x & 1) * y) + (n * M)) >> 1;
        x = x >> 1;
        m = m >> 1;
    }
    if (t >= M) {
        t = t - M;
    }
    return t;
}

int main(void) {
    // P and Q are two large prime numbers
    //NOTE: The max bits must be 16 bits or less because when p and q are 16 bits, it produces a 32 bit pq value
    //When pq is 32 bits, the r value is 2^32 which is larger than 32 bits
    //When r * r is calculated, it becomes larger than 64 bits
    //So when p and q are larger than 16 bits, it results in r * r being larger than 64 bits
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
    uint64_t d = (uint64_t)x * phi + 1;
    d = d / e;
    // int32_t d = (int32_t)((uint64_t(x * phi) + 1) / e);

    // TODO: The message being encrypted, t, must be less that the modulus, PQ
    // This must be less than pq
    uint64_t t = 1845588466;
    if ((p * q) < t){
        printf("Our plain text t must be less than p * q\n");
        exit(-1);
    }
    uint64_t pq = p * q;
    printf("d:%llu, p: %d, q: %d, e: %d, pq: %d, (p-1)(q-1): %d\n", d, p, q, e, p * q, phi);

    uint64_t c_encrypted = modular_exponentiation(t, e, pq);

    printf("c_encrypted: %llu\n", c_encrypted);

    uint64_t t_decrypted = modular_exponentiation(c_encrypted, d, pq);

    printf("t_decrypted: %llu\n", t_decrypted);

    /* keep this for now */
//    int64_t m = p * q;
//    int64_t r = 1 << (int64_t)(log2(m) + 1);
//    int64_t r2 = (r * r) % m;
//    int64_t X = 3333;
//    int64_t Y = 522;
//    int64_t a = montgomery_modular_multiplication(X, r2, m);
//    int64_t b = montgomery_modular_multiplication(Y, r2, m);
//    int64_t result = montgomery_modular_multiplication(a, b, m);
//    int64_t final = montgomery_modular_multiplication(result, 1, m); // Convert back from Montgomery form
//
//    printf("p * q: %lld\n", m);
//    printf("result: %lld\n", result);
//    printf("%lld * %lld mod %lld: %lld\n", X, Y, m, final);

    return 0;
}

