#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

// List of all primes
static const uint32_t primes_16bit[] = {
        33811, 33827, 33829, 33851, 33857, 33863, 33871, 33889, 33893, 33911, 33923, 33931, 33937, 33941, 33961, 33967, 33997, 34019, 34031, 34033, 34039, 34057, 34061, 34123, 34127, 34129, 34141, 34147, 34157, 34159, 34171, 34183, 34211, 34213, 34217, 34231, 34253, 34259, 34261, 34267, 34273, 34283, 34297, 34301, 34303, 34313, 34319, 34327, 34337, 34351, 34361, 34367, 34369, 34381, 34403, 34421, 34429, 34439, 34457, 34469, 34471, 34483, 34487, 34499, 34501, 34511, 34513, 34519, 34537, 34543, 34549, 34583, 34589, 34591, 34603, 34607, 34613, 34631, 34649, 34651, 34667, 34673, 34679, 34687, 34693, 34703, 34721, 34729, 34739, 34747, 34757, 34759, 34763, 34781, 34807, 34819, 34841, 34843, 34847, 34849, 34871,
        34877, 34883, 34897, 34913, 34919, 34939, 34949, 34961, 34963, 34981, 35023, 35027, 35051, 35053, 35059, 35069, 35081, 35083, 35089, 35099, 35107, 35111, 35117, 35129, 35141, 35149, 35153, 35159, 35171, 35201, 35221, 35227, 35251, 35257, 35267, 35279, 35281, 35291, 35311, 35317, 35323, 35327, 35339, 35353, 35363, 35381, 35393, 35401, 35407, 35419, 35423, 35437, 35447, 35449, 35461, 35491, 35507, 35509, 35521, 35527, 35531, 35533, 35537, 35543, 35569, 35573, 35591, 35593, 35597, 35603, 35617, 35671, 35677, 35729, 35731, 35747, 35753, 35759, 35771, 35797, 35801, 35803, 35809, 35831, 35837, 35839, 35851, 35863, 35869, 35879, 35897, 35899, 35911, 35923, 35933, 35951, 35963, 35969, 35977, 35983,
        35993, 35999, 36007, 36011, 36013, 36017, 36037, 36061, 36067, 36073, 36083, 36097, 36107, 36109, 36131, 36137, 36151, 36161, 36187, 36191, 36209, 36217, 36229, 36241, 36251, 36263, 36269, 36277, 36293, 36299, 36307, 36313, 36319, 36341, 36343, 36353, 36373, 36383, 36389, 36433, 36451, 36457, 36467, 36469, 36473, 36479, 36493, 36497, 36523, 36527, 36529, 36541, 36551, 36559, 36563, 36571, 36583, 36587, 36599, 36607, 36629, 36637, 36643, 36653, 36671, 36677, 36683, 36691, 36697, 36709, 36713, 36721, 36739, 36749, 36761, 36767, 36779, 36781, 36787, 36791, 36793, 36809, 36821, 36833, 36847, 36857, 36871, 36877, 36887, 36899, 36901, 36913, 36919, 36923, 36929, 36931, 36943, 36947, 36973, 36979,
        36997, 37003, 37013, 37019, 37021, 37039, 37049, 37057, 37061, 37087, 37097, 37117, 37123, 37139, 37159, 37171, 37181, 37189, 37199, 37201, 37217, 37223, 37243, 37253, 37273, 37277, 37307, 37309, 37313, 37321, 37337, 37339, 37357, 37361, 37363, 37369, 37379, 37397, 37409, 37423, 37441, 37447, 37463, 37483, 37489, 37493, 37501, 37507, 37511, 37517, 37529, 37537, 37547, 37549, 37561, 37567, 37571, 37573, 37579, 37589, 37591, 37607, 37619, 37633, 37643, 37649, 37657, 37663, 37691, 37693, 37699, 37717, 37747, 37781, 37783, 37799, 37811, 37813, 37831, 37847, 37853, 37861, 37871, 37879, 37889, 37897, 37907, 37951, 37957, 37963, 37967, 37987, 37991, 37993, 37997, 38011, 38039, 38047, 38053, 38069,
        38083, 38113, 38119, 38149, 38153, 38167, 38177, 38183, 38189, 38197, 38201, 38219, 38231, 38237, 38239, 38261, 38273, 38281, 38287, 38299, 38303, 38317, 38321, 38327, 38329, 38333, 38351, 38371, 38377, 38393, 38431, 38447, 38449, 38453, 38459, 38461, 38501, 38543, 38557, 38561, 38567, 38569, 38593, 38603, 38609, 38611, 38629, 38639, 38651, 38653, 38669, 38671, 38677, 38693, 38699, 38707, 38711, 38713, 38723, 38729, 38737, 38747, 38749, 38767, 38783, 38791, 38803, 38821, 38833, 38839, 38851, 38861, 38867, 38873, 38891, 38903, 38917, 38921, 38923, 38933, 38953, 38959, 38971, 38977, 38993, 39019, 39023, 39041, 39043, 39047, 39079, 39089, 39097, 39103, 39107, 39113, 39119, 39133, 39139, 39157,
        39161, 39163, 39181, 39191, 39199, 39209, 39217, 39227, 39229, 39233, 39239, 39241, 39251, 39293, 39301, 39313, 39317, 39323, 39341, 39343, 39359, 39367, 39371, 39373, 39383, 39397, 39409, 39419, 39439, 39443, 39451, 39461, 39499, 39503, 39509, 39511, 39521, 39541, 39551, 39563, 39569, 39581, 39607, 39619, 39623, 39631, 39659, 39667, 39671, 39679, 39703, 39709, 39719, 39727, 39733, 39749, 39761, 39769, 39779, 39791, 39799, 39821, 39827, 39829, 39839, 39841, 39847, 39857, 39863, 39869, 39877, 39883, 39887, 39901, 39929, 39937, 39953, 39971, 39979, 39983, 39989, 40009, 40013, 40031, 40037, 40039, 40063, 40087, 40093, 40099, 40111, 40123, 40127, 40129, 40151, 40153, 40163, 40169, 40177, 40189,
        40193, 40213, 40231, 40237, 40241, 40253, 40277, 40283, 40289, 40343, 40351, 40357, 40361, 40387, 40423, 40427, 40429, 40433, 40459, 40471, 40483, 40487, 40493, 40499, 40507, 40519, 40529, 40531, 40543, 40559, 40577, 40583, 40591, 40597, 40609, 40627, 40637, 40639, 40693, 40697, 40699, 40709, 40739, 40751, 40759, 40763, 40771, 40787, 40801, 40813, 40819, 40823, 40829, 40841, 40847, 40849, 40853, 40867, 40879, 40883, 40897, 40903, 40927, 40933, 40939, 40949, 40961, 40973, 40993, 41011, 41017, 41023, 41039, 41047, 41051, 41057, 41077, 41081, 41113, 41117, 41131, 41141, 41143, 41149, 41161, 41177, 41179, 41183, 41189, 41201, 41203, 41213, 41221, 41227, 41231, 41233, 41243, 41257, 41263, 41269,
        41281, 41299, 41333, 41341, 41351, 41357, 41381, 41387, 41389, 41399, 41411, 41413, 41443, 41453, 41467, 41479, 41491, 41507, 41513, 41519, 41521, 41539, 41543, 41549, 41579, 41593, 41597, 41603, 41609, 41611, 41617, 41621, 41627, 41641, 41647, 41651, 41659, 41669, 41681, 41687, 41719, 41729, 41737, 41759, 41761, 41771, 41777, 41801, 41809, 41813, 41843, 41849, 41851, 41863, 41879, 41887, 41893, 41897, 41903, 41911, 41927, 41941, 41947, 41953, 41957, 41959, 41969, 41981, 41983, 41999, 42013, 42017, 42019, 42023, 42043, 42061, 42071, 42073, 42083, 42089, 42101, 42131, 42139, 42157, 42169, 42179, 42181, 42187, 42193, 42197, 42209, 42221, 42223, 42227, 42239, 42257, 42281, 42283, 42293, 42299,
        42307, 42323, 42331, 42337, 42349, 42359, 42373, 42379, 42391, 42397, 42403, 42407, 42409, 42433, 42437, 42443, 42451, 42457, 42461, 42463, 42467, 42473, 42487, 42491, 42499, 42509, 42533, 42557, 42569, 42571, 42577, 42589, 42611, 42641, 42643, 42649, 42667, 42677, 42683, 42689, 42697, 42701, 42703, 42709, 42719, 42727, 42737, 42743, 42751, 42767, 42773, 42787, 42793, 42797, 42821, 42829, 42839, 42841, 42853, 42859, 42863, 42899, 42901, 42923, 42929, 42937, 42943, 42953, 42961, 42967, 42979, 42989, 43003, 43013, 43019, 43037, 43049, 43051, 43063, 43067, 43093, 43103, 43117, 43133, 43151, 43159, 43177, 43189, 43201, 43207, 43223, 43237, 43261, 43271, 43283, 43291, 43313, 43319, 43321, 43331,
        43391, 43397, 43399, 43403, 43411, 43427, 43441, 43451, 43457, 43481,43487, 43499, 43517, 43541, 43543, 43573, 43577, 43579, 43591, 43597,43607, 43609, 43613, 43627, 43633, 43649, 43651, 43661, 43669, 43691,43711, 43717, 43721, 43753, 43759, 43777, 43781, 43783, 43787, 43789,43793, 43801, 43853, 43867, 43889, 43891, 43913, 43933, 43943, 43951,43961, 43963, 43969, 43973, 43987, 43991, 43997, 44017, 44021, 44027,44029, 44041, 44053, 44059, 44071, 44087, 44089, 44101, 44111, 44119,44123, 44129, 44131, 44159, 44171, 44179, 44189, 44201, 44203, 44207,44221, 44249, 44257, 44263, 44267, 44269, 44273, 44279, 44281, 44293,44351, 44357, 44371, 44381, 44383, 44389, 44417, 44449, 44453,
};

#define NUM_16BIT_PRIMES (sizeof(primes_16bit) / sizeof(primes_16bit[0]))

uint64_t montgomery_modular_multiplication(register uint64_t x, register uint64_t y, register uint64_t M);
uint64_t modular_exponentiation(register uint64_t p, register uint64_t e, register uint64_t m);
int32_t compute_x(uint32_t phi, uint16_t e);
int mod_inverse(int e, int phi);
uint16_t get_16bit_prime(int seed);
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
    uint64_t k = mod_inverse(e, phi);

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
uint16_t get_16bit_prime(int seed) {
    return primes_16bit[seed % NUM_16BIT_PRIMES];
}

/** With no optimization:
 * inefficient with exponential operator
 * Can only support 3 bits input with limited range of t values
 * **/
//int32_t bruteforce_rsa_cryptography(int32_t t, int32_t e, int32_t pq) {
//    return (int32_t)pow(t, e) % pq;
//}

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
uint64_t modular_exponentiation(uint64_t p, uint64_t e, uint64_t m) {
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
uint64_t montgomery_modular_multiplication(uint64_t x, uint64_t y, uint64_t M) {
    uint64_t m = M;
    uint64_t t = 0;
    uint64_t y_and_1 = y & 1; // precompute y & 1
    uint64_t n, xy, nm, x_and_1;


    // Loop through the number of m bits in pq
    // loop unrolled thrice
    // software pipelining by reordering instructions
    while (m > 2) {
        // First iteration
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace with multiplications with predicate operations and 2's complement
        x_and_1 = (x & 1);
        xy = -x_and_1 & y;
        nm = (-n & M);
        t = (t + xy + nm) >> 1;
        x = x >> 1;

        // Second iteration (unrolled)
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace with multiplications with predicate operations and 2's complement
        x_and_1 = (x & 1);
        xy = -x_and_1 & y;
        nm = (-n & M);
        t = (t + xy + nm) >> 1;
        x = x >> 1;

        // third iteration (unrolled)
        n = ((t & 1)) ^ ((x & 1) & y_and_1);
        // replace with multiplications with predicate operations and 2's complement
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
        // replace with nm multiplication with predicate operation and 2' complement
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

int cryptography(uint32_t t, int seed_p, int seed_q, int seed_e) {
    // P and Q are two large prime numbers
    // NOTE: The max bits must be 16 bits or less because when p and q are 16 bits, it produces a 32 bit pq value
    // When pq is 32 bits, the r value is 2^32 which is larger than 32 bits
    // When r * r is calculated, it becomes larger than 64 bits
    // So when p and q are larger than 16 bits, it results in r * r being larger than 64 bits
    const uint16_t p = get_16bit_prime(seed_p);
    const uint16_t q = get_16bit_prime(seed_q);

    //(P - 1) * (Q - 1) is an even number
    const uint32_t phi = (uint32_t)(p - 1) * (uint32_t)(q - 1);

    //E > 1 and E < P*Q
    //E is an odd prime number
    //E and (P - 1)*(Q - 1) are relatively prime (meaning they have no prime factors in common)
    uint16_t e;
    do {
        e = get_16bit_prime(seed_e);
    } while (phi % e == 0);

    uint32_t x = compute_x(phi, e);

    // Compute D = (X(P −1)(Q−1)+1)/E where d is an integer
    uint64_t d = (uint64_t)x * phi + 1;
    d = d / (uint64_t)e;

    // t is the plaintext (a positive integer) and t is a message being encrypted
    // t must be less than the modulus PQ
    // check that t is less than p * q
    uint64_t pq = (uint32_t)p * (uint32_t)q;

    // branch elimination: minor performance improvements, but make the code less readable
    (pq) < t && (printf("Our plain text t must be less than p * q [ q: %d, p: %d, p * q: %llu, t: %d] \n", q, p, pq, t), exit(-1), 0);

    // encryption of plaintext T, C = T^E mod PQ
    uint64_t c_encrypted = modular_exponentiation(t, e, pq);

//    printf("c_encrypted: %llu\n", c_encrypted);

    // decryption of the ciphertext C, T = C^D mod PQ
    uint64_t t_decrypted = modular_exponentiation(c_encrypted, d, pq);

//    printf("t_decrypted: %llu\n", t_decrypted);

    return t_decrypted == t;
}

int main(void) {
    struct timeval start_time, end_time;

    // start time
    gettimeofday(&start_time, NULL);

    const int lookup[10] = {1,2, 10,999,1000,5678,98765,666666,9876543,10000000};

    // t is the plaintext (a positive integer) and t is a message being encrypted
    // t must be less than the modulus PQ (less than 31 bits, since the multiplication of two lowest 16 bit int is under 31 bits)
    for(int k = 0 ; k < 1; k++){
        for (int i = 0; i < 10; i++) {
            assert(cryptography(lookup[i], 99, 5, 99) == 1);
            assert(cryptography(lookup[i], 1, 5, 99) == 1);
            assert(cryptography(lookup[i], 11, 10, 111) == 1);
        }
    }

    // end time
    gettimeofday(&end_time, NULL);
    printf("Total Time: %ld microseconds\n", ((end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)));

    return 0;
}