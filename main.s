	.arch armv7-a
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.global	__aeabi_idivmod
	.global	__aeabi_idiv
	.align	2
	.global	gcd_extended
	.arch armv7-a
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	gcd_extended, %function
gcd_extended:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #28
	str	r0, [fp, #-24]
	str	r1, [fp, #-28]
	str	r2, [fp, #-32]
	str	r3, [fp, #-36]
	ldr	r3, [fp, #-24]
	cmp	r3, #0
	bne	.L2
	ldr	r3, [fp, #-32]
	mov	r2, #0
	str	r2, [r3]
	ldr	r3, [fp, #-36]
	mov	r2, #1
	str	r2, [r3]
	b	.L1
.L2:
	ldr	r3, [fp, #-28]
	ldr	r1, [fp, #-24]
	mov	r0, r3
	bl	__aeabi_idivmod
	mov	r3, r1
	mov	r0, r3
	sub	r3, fp, #20
	sub	r2, fp, #16
	ldr	r1, [fp, #-24]
	bl	gcd_extended
	ldr	r4, [fp, #-20]
	ldr	r1, [fp, #-24]
	ldr	r0, [fp, #-28]
	bl	__aeabi_idiv
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-16]
	mul	r3, r3, r2
	sub	r2, r4, r3
	ldr	r3, [fp, #-32]
	str	r2, [r3]
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-36]
	str	r2, [r3]
.L1:
	sub	sp, fp, #8
	@ sp needed
	pop	{r4, fp, pc}
	.size	gcd_extended, .-gcd_extended
	.align	2
	.global	mod_inverse
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	mod_inverse, %function
mod_inverse:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	sub	r3, fp, #12
	sub	r2, fp, #8
	ldr	r1, [fp, #-20]
	ldr	r0, [fp, #-16]
	bl	gcd_extended
	ldr	r3, [fp, #-8]
	ldr	r1, [fp, #-20]
	mov	r0, r3
	bl	__aeabi_idivmod
	mov	r3, r1
	mov	r2, r3
	ldr	r3, [fp, #-20]
	add	r3, r2, r3
	ldr	r1, [fp, #-20]
	mov	r0, r3
	bl	__aeabi_idivmod
	mov	r3, r1
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
	.size	mod_inverse, .-mod_inverse
	.global	__aeabi_uldivmod
	.align	2
	.global	compute_x
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	compute_x, %function
compute_x:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #24
	str	r0, [fp, #-32]
	mov	r3, r1
	strh	r3, [fp, #-34]	@ movhi
	ldrh	r3, [fp, #-34]
	ldr	r2, [fp, #-32]
	mov	r1, r2
	mov	r0, r3
	bl	mod_inverse
	mov	r3, r0
	mov	r2, r3
	asr	r3, r2, #31
	strd	r2, [fp, #-20]
	ldrh	r2, [fp, #-34]
	mov	r3, #0
	ldr	r1, [fp, #-20]
	mul	r0, r3, r1
	ldr	r1, [fp, #-16]
	mul	r1, r2, r1
	add	r1, r0, r1
	ldr	r0, [fp, #-20]
	umull	r2, r3, r0, r2
	add	r1, r1, r3
	mov	r3, r1
	subs	r4, r2, #1
	sbc	r5, r3, #0
	ldr	r3, [fp, #-32]
	mov	r2, r3
	mov	r3, #0
	mov	r0, r4
	mov	r1, r5
	bl	__aeabi_uldivmod
	mov	r2, r0
	mov	r3, r1
	mov	r3, r2
	str	r3, [fp, #-24]
	ldr	r3, [fp, #-24]
	mov	r0, r3
	sub	sp, fp, #12
	@ sp needed
	pop	{r4, r5, fp, pc}
	.size	compute_x, .-compute_x
	.align	2
	.global	get_16bit_prime
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	get_16bit_prime, %function
get_16bit_prime:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #48
	str	r0, [fp, #-48]
	sub	r3, fp, #28
	mov	r0, r3
	bl	__gmp_randinit_default
	ldr	r2, [fp, #-48]
	sub	r3, fp, #28
	mov	r1, r2
	mov	r0, r3
	bl	__gmp_randseed_ui
	sub	r3, fp, #40
	mov	r0, r3
	bl	__gmpz_init
	sub	r1, fp, #28
	sub	r3, fp, #40
	mov	r2, #15
	mov	r0, r3
	bl	__gmpz_urandomb
	sub	r2, fp, #40
	sub	r3, fp, #40
	mov	r1, r2
	mov	r0, r3
	bl	__gmpz_nextprime
	sub	r3, fp, #40
	mov	r0, r3
	bl	__gmpz_get_ui
	mov	r3, r0
	strh	r3, [fp, #-6]	@ movhi
	sub	r3, fp, #40
	mov	r0, r3
	bl	__gmpz_clear
	sub	r3, fp, #28
	mov	r0, r3
	bl	__gmp_randclear
	ldrh	r3, [fp, #-6]
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
	.size	get_16bit_prime, .-get_16bit_prime
	.global	__aeabi_ul2d
	.global	__aeabi_d2ulz
	.align	2
	.global	modular_exponentiation
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	modular_exponentiation, %function
modular_exponentiation:
	@ args = 8, pretend = 0, frame = 56
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, fp, lr}
	add	fp, sp, #20
	sub	sp, sp, #64
	strd	r0, [fp, #-68]
	strd	r2, [fp, #-76]
	ldrd	r0, [fp, #4]
	bl	__aeabi_ul2d
	mov	r2, r0
	mov	r3, r1
	vmov	d0, r2, r3
	bl	log2
	vmov	r2, r3, d0
	mov	r0, r2
	mov	r1, r3
	bl	__aeabi_d2ulz
	mov	r2, r0
	mov	r3, r1
	mov	r3, r2
	add	r1, r3, #1
	mov	r2, #1
	mov	r3, #0
	sub	ip, r1, #32
	rsb	r0, r1, #32
	lsl	r5, r3, r1
	orr	r5, r5, r2, lsl ip
	orr	r5, r5, r2, lsr r0
	lsl	r4, r2, r1
	strd	r4, [fp, #-36]
	ldr	r3, [fp, #-32]
	ldr	r2, [fp, #-36]
	mul	r2, r2, r3
	ldr	r3, [fp, #-32]
	ldr	r1, [fp, #-36]
	mul	r3, r1, r3
	add	r3, r2, r3
	ldr	r1, [fp, #-36]
	ldr	r2, [fp, #-36]
	umull	r0, r1, r1, r2
	add	r3, r3, r1
	mov	r1, r3
	ldrd	r2, [fp, #4]
	bl	__aeabi_uldivmod
	strd	r2, [fp, #-44]
	mov	r2, #1
	mov	r3, #0
	strd	r2, [fp, #-28]
	ldrd	r0, [fp, #-68]
	ldrd	r2, [fp, #4]
	bl	__aeabi_uldivmod
	strd	r2, [fp, #-68]
	b	.L11
.L13:
	ldrd	r2, [fp, #4]
	strd	r2, [sp]
	ldrd	r2, [fp, #-44]
	ldrd	r0, [fp, #-68]
	bl	montgomery_modular_multiplication
	strd	r0, [fp, #-52]
	ldrd	r2, [fp, #-76]
	mov	r0, #1
	mov	r1, #0
	and	r6, r2, r0
	and	r7, r3, r1
	orrs	r3, r6, r7
	beq	.L12
	ldrd	r2, [fp, #4]
	strd	r2, [sp]
	ldrd	r2, [fp, #-44]
	ldrd	r0, [fp, #-28]
	bl	montgomery_modular_multiplication
	strd	r0, [fp, #-60]
	ldrd	r2, [fp, #4]
	strd	r2, [sp]
	ldrd	r2, [fp, #-52]
	ldrd	r0, [fp, #-60]
	bl	montgomery_modular_multiplication
	strd	r0, [fp, #-28]
	ldrd	r2, [fp, #4]
	strd	r2, [sp]
	mov	r2, #1
	mov	r3, #0
	ldrd	r0, [fp, #-28]
	bl	montgomery_modular_multiplication
	strd	r0, [fp, #-28]
.L12:
	ldrd	r0, [fp, #-76]
	mov	r2, #0
	mov	r3, #0
	lsr	r2, r0, #1
	orr	r2, r2, r1, lsl #31
	lsr	r3, r1, #1
	strd	r2, [fp, #-76]
	ldrd	r2, [fp, #4]
	strd	r2, [sp]
	ldrd	r2, [fp, #-52]
	ldrd	r0, [fp, #-52]
	bl	montgomery_modular_multiplication
	strd	r0, [fp, #-68]
	ldrd	r2, [fp, #4]
	strd	r2, [sp]
	mov	r2, #1
	mov	r3, #0
	ldrd	r0, [fp, #-68]
	bl	montgomery_modular_multiplication
	strd	r0, [fp, #-68]
.L11:
	ldrd	r2, [fp, #-76]
	orrs	r3, r2, r3
	bne	.L13
	ldrd	r2, [fp, #-28]
	mov	r0, r2
	mov	r1, r3
	sub	sp, fp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, fp, pc}
	.size	modular_exponentiation, .-modular_exponentiation
	.align	2
	.global	montgomery_modular_multiplication
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	montgomery_modular_multiplication, %function
montgomery_modular_multiplication:
	@ args = 8, pretend = 0, frame = 80
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r5, r6, r7, r8, r9, fp}
	add	fp, sp, #24
	sub	sp, sp, #84
	strd	r0, [fp, #-60]
	strd	r2, [fp, #-68]
	ldrd	r2, [fp, #4]
	strd	r2, [fp, #-36]
	mov	r2, #0
	mov	r3, #0
	strd	r2, [fp, #-44]
	b	.L16
.L17:
	ldrd	r0, [fp, #-60]
	ldrd	r2, [fp, #-68]
	and	ip, r0, r2
	str	ip, [fp, #-76]
	and	r3, r1, r3
	str	r3, [fp, #-72]
	ldrd	r2, [fp, #-44]
	ldrd	r0, [fp, #-76]
	mov	ip, r0
	eor	ip, ip, r2
	str	ip, [fp, #-84]
	eor	r3, r1, r3
	str	r3, [fp, #-80]
	mov	r2, #1
	mov	r3, #0
	ldrd	r0, [fp, #-84]
	mov	ip, r0
	and	ip, ip, r2
	str	ip, [fp, #-100]
	and	r3, r1, r3
	str	r3, [fp, #-96]
	ldrd	r2, [fp, #-100]
	strd	r2, [fp, #-52]
	ldrd	r0, [fp, #-60]
	mov	r2, #1
	mov	r3, #0
	and	r6, r0, r2
	and	r7, r1, r3
	ldr	r3, [fp, #-68]
	mul	r2, r7, r3
	ldr	r3, [fp, #-64]
	mul	r3, r6, r3
	add	r2, r2, r3
	ldr	r3, [fp, #-68]
	umull	r4, r5, r3, r6
	add	r3, r2, r5
	mov	r5, r3
	ldr	r2, [fp, #-48]
	ldr	r3, [fp, #4]
	mul	r1, r3, r2
	ldr	r2, [fp, #8]
	ldr	r3, [fp, #-52]
	mul	r3, r3, r2
	add	r1, r1, r3
	ldr	r2, [fp, #-52]
	ldr	r3, [fp, #4]
	umull	r2, r3, r2, r3
	add	r1, r1, r3
	mov	r3, r1
	adds	r1, r4, r2
	str	r1, [fp, #-92]
	adc	r3, r5, r3
	str	r3, [fp, #-88]
	ldrd	r2, [fp, #-44]
	ldrd	r0, [fp, #-92]
	mov	ip, r0
	adds	r8, ip, r2
	adc	r9, r1, r3
	mov	r2, #0
	mov	r3, #0
	lsr	r2, r8, #1
	orr	r2, r2, r9, lsl #31
	lsr	r3, r9, #1
	strd	r2, [fp, #-44]
	ldrd	r2, [fp, #-60]
	mov	r0, #0
	mov	r1, #0
	lsr	r0, r2, #1
	orr	r0, r0, r3, lsl #31
	lsr	r1, r3, #1
	strd	r0, [fp, #-60]
	ldrd	r2, [fp, #-36]
	mov	r0, #0
	mov	r1, #0
	lsr	r0, r2, #1
	orr	r0, r0, r3, lsl #31
	lsr	r1, r3, #1
	strd	r0, [fp, #-36]
.L16:
	ldrd	r2, [fp, #-36]
	orrs	r3, r2, r3
	bne	.L17
	ldrd	r0, [fp, #-44]
	ldrd	r2, [fp, #4]
	cmp	r1, r3
	cmpeq	r0, r2
	bcc	.L18
	ldrd	r0, [fp, #-44]
	ldrd	r2, [fp, #4]
	subs	ip, r0, r2
	str	ip, [fp, #-108]
	sbc	r3, r1, r3
	str	r3, [fp, #-104]
	ldrd	r2, [fp, #-108]
	strd	r2, [fp, #-44]
.L18:
	ldrd	r2, [fp, #-44]
	mov	r0, r2
	mov	r1, r3
	sub	sp, fp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, fp}
	bx	lr
	.size	montgomery_modular_multiplication, .-montgomery_modular_multiplication
	.global	__aeabi_uidivmod
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Our plain text t must be less than p * q [ q: %d, p"
	.ascii	": %d, p * q: %llu, t: %d] \012\000"
	.text
	.align	2
	.global	cryptography
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	cryptography, %function
cryptography:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #80
	str	r0, [fp, #-64]
	str	r1, [fp, #-68]
	str	r2, [fp, #-72]
	str	r3, [fp, #-76]
	ldr	r0, [fp, #-68]
	bl	get_16bit_prime
	mov	r3, r0
	strh	r3, [fp, #-14]	@ movhi
	ldr	r0, [fp, #-72]
	bl	get_16bit_prime
	mov	r3, r0
	strh	r3, [fp, #-16]	@ movhi
	ldrh	r3, [fp, #-14]
	sub	r3, r3, #1
	mov	r2, r3
	ldrh	r3, [fp, #-16]
	sub	r3, r3, #1
	mul	r3, r3, r2
	str	r3, [fp, #-20]
.L21:
	ldr	r0, [fp, #-76]
	bl	get_16bit_prime
	mov	r3, r0
	strh	r3, [fp, #-22]	@ movhi
	ldrh	r2, [fp, #-22]
	ldr	r3, [fp, #-20]
	mov	r1, r2
	mov	r0, r3
	bl	__aeabi_uidivmod
	mov	r3, r1
	cmp	r3, #0
	beq	.L21
	ldrh	r3, [fp, #-22]
	mov	r1, r3
	ldr	r0, [fp, #-20]
	bl	compute_x
	mov	r3, r0
	str	r3, [fp, #-28]
	ldr	r3, [fp, #-28]
	mov	r0, r3
	mov	r1, #0
	ldr	r3, [fp, #-20]
	mov	r2, r3
	mov	r3, #0
	mul	lr, r2, r1
	mul	ip, r0, r3
	add	ip, lr, ip
	umull	r2, r3, r0, r2
	add	r1, ip, r3
	mov	r3, r1
	adds	r4, r2, #1
	adc	r5, r3, #0
	strd	r4, [fp, #-36]
	ldrh	r2, [fp, #-22]
	mov	r3, #0
	ldrd	r0, [fp, #-36]
	bl	__aeabi_uldivmod
	mov	r2, r0
	mov	r3, r1
	strd	r2, [fp, #-36]
	ldrh	r3, [fp, #-14]
	ldrh	r2, [fp, #-16]
	mul	r3, r2, r3
	mov	r2, r3
	mov	r3, #0
	strd	r2, [fp, #-44]
	ldr	r3, [fp, #-64]
	mov	r2, r3
	mov	r3, #0
	ldrd	r0, [fp, #-44]
	cmp	r1, r3
	cmpeq	r0, r2
	bcs	.L22
	ldrh	r1, [fp, #-16]
	ldrh	r0, [fp, #-14]
	ldr	r3, [fp, #-64]
	str	r3, [sp, #8]
	ldrd	r2, [fp, #-44]
	strd	r2, [sp]
	mov	r2, r0
	movw	r0, #:lower16:.LC0
	movt	r0, #:upper16:.LC0
	bl	printf
	mvn	r0, #0
	bl	exit
.L22:
	ldr	r3, [fp, #-64]
	mov	r0, r3
	mov	r1, #0
	ldrh	r4, [fp, #-22]
	mov	r5, #0
	ldrd	r2, [fp, #-44]
	strd	r2, [sp]
	mov	r2, r4
	mov	r3, r5
	bl	modular_exponentiation
	strd	r0, [fp, #-52]
	ldrd	r2, [fp, #-44]
	strd	r2, [sp]
	ldrd	r2, [fp, #-36]
	ldrd	r0, [fp, #-52]
	bl	modular_exponentiation
	strd	r0, [fp, #-60]
	ldr	r3, [fp, #-64]
	mov	r2, r3
	mov	r3, #0
	ldrd	r0, [fp, #-60]
	cmp	r1, r3
	cmpeq	r0, r2
	moveq	r3, #1
	movne	r3, #0
	uxtb	r3, r3
	mov	r0, r3
	sub	sp, fp, #12
	@ sp needed
	pop	{r4, r5, fp, pc}
	.size	cryptography, .-cryptography
	.section	.rodata
	.align	2
.LC1:
	.ascii	"main.c\000"
	.align	2
.LC2:
	.ascii	"cryptography(10000000, 99, 5, 99) == 1\000"
	.align	2
.LC3:
	.ascii	"Total Time: %ld microseconds\012\000"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu vfpv3-d16
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	sub	r3, fp, #12
	mov	r1, #0
	mov	r0, r3
	bl	gettimeofday
	mov	r3, #99
	mov	r2, #5
	mov	r1, #99
	movw	r0, #38528
	movt	r0, 152
	bl	cryptography
	mov	r3, r0
	cmp	r3, #1
	beq	.L25
	movw	r3, #:lower16:__PRETTY_FUNCTION__.9201
	movt	r3, #:upper16:__PRETTY_FUNCTION__.9201
	mov	r2, #268
	movw	r1, #:lower16:.LC1
	movt	r1, #:upper16:.LC1
	movw	r0, #:lower16:.LC2
	movt	r0, #:upper16:.LC2
	bl	__assert_fail
.L25:
	sub	r3, fp, #20
	mov	r1, #0
	mov	r0, r3
	bl	gettimeofday
	ldr	r1, [fp, #-20]
	mov	r2, r1
	lsl	r2, r2, #5
	sub	r2, r2, r1
	lsl	r3, r2, #6
	sub	r3, r3, r2
	lsl	r3, r3, #3
	add	r3, r3, r1
	lsl	r3, r3, #6
	mov	r2, r3
	ldr	r3, [fp, #-16]
	add	r0, r2, r3
	ldr	r1, [fp, #-12]
	mov	r2, r1
	lsl	r2, r2, #5
	sub	r2, r2, r1
	lsl	r3, r2, #6
	sub	r3, r3, r2
	lsl	r3, r3, #3
	add	r3, r3, r1
	lsl	r3, r3, #6
	mov	r2, r3
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	sub	r3, r0, r3
	mov	r1, r3
	movw	r0, #:lower16:.LC3
	movt	r0, #:upper16:.LC3
	bl	printf
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
	.size	main, .-main
	.section	.rodata
	.align	2
	.type	__PRETTY_FUNCTION__.9201, %object
	.size	__PRETTY_FUNCTION__.9201, 5
__PRETTY_FUNCTION__.9201:
	.ascii	"main\000"
	.ident	"GCC: (GNU) 8.2.1 20180801 (Red Hat 8.2.1-2)"
	.section	.note.GNU-stack,"",%progbits
