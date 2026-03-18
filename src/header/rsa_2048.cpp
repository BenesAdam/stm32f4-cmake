#include "rsa_2048.hpp"


#define LIMB_BITS   32
#define LIMB_COUNT  64   // 64 * 32 = 2048 bitů
#define DBL_LIMB_COUNT (2 * LIMB_COUNT)

typedef struct {
    uint32_t limb[LIMB_COUNT];
} bigint;


void bigint_zero(bigint *a) {
    for (int i = 0; i < LIMB_COUNT; i++)
        a->limb[i] = 0;
}



int bigint_cmp(const bigint *a, const bigint *b) {
    for (int i = LIMB_COUNT - 1; i >= 0; i--) {
        if (a->limb[i] > b->limb[i]) return 1;
        if (a->limb[i] < b->limb[i]) return -1;
    }
    return 0;
}


void bigint_sub(bigint *a, const bigint *b) {
    uint64_t borrow = 0;

    for (int i = 0; i < LIMB_COUNT; i++) {
        uint64_t tmp = (uint64_t)a->limb[i] - b->limb[i] - borrow;
        a->limb[i] = (uint32_t)tmp;
        borrow = (tmp >> 63) & 1;
    }
}


void bigint_shift_right_one(bigint *a) {
    uint32_t carry = 0;
    for (int i = LIMB_COUNT - 1; i >= 0; i--) {
        uint32_t new_carry = a->limb[i] & 1;
        a->limb[i] = (a->limb[i] >> 1) | (carry << 31);
        carry = new_carry;
    }
}

int bigint_is_zero(const bigint *a) {
    for (int i = 0; i < LIMB_COUNT; i++)
        if (a->limb[i]) return 0;
    return 1;
}

void bigint_one(bigint *a) {
    bigint_zero(a);
    a->limb[0] = 1;
}

void bigint_from_bytes_be(bigint *dst,
                           const uint8_t *src,
                           size_t len)
{
    bigint_zero(dst);

    for (size_t i = 0; i < len; i++) {
        size_t byte  = len - 1 - i;
        size_t limb  = i / 4;
        size_t shift = (i % 4) * 8;

        if (limb < LIMB_COUNT) {
            dst->limb[limb] |= ((uint32_t)src[byte]) << shift;
        }
    }
}


void bigint_to_bytes_be(const bigint *x,
                        uint8_t *dst,
                        size_t len)
{
    for (size_t i = 0; i < len; i++) {
        size_t src_index = len - 1 - i;
        size_t limb = src_index / 4;
        size_t shift = (src_index % 4) * 8;

        dst[i] = (x->limb[limb] >> shift) & 0xFF;
    }
}




void bigint_mul_full(uint32_t *tmp,
                      const bigint *a,
                      const bigint *b)
{
    memset(tmp, 0, sizeof(uint32_t) * DBL_LIMB_COUNT);

    for (int i = 0; i < LIMB_COUNT; i++) {
        uint64_t carry = 0;

        for (int j = 0; j < LIMB_COUNT; j++) {
            uint64_t r =
                tmp[i + j] +
                (uint64_t)a->limb[i] * b->limb[j] +
                carry;

            tmp[i + j] = (uint32_t)r;
            carry = r >> 32;
        }

        tmp[i + LIMB_COUNT] += (uint32_t)carry;
    }
}


void bigint_shift_left_one(bigint *a)
{
    uint32_t carry = 0;

    for (int i = 0; i < LIMB_COUNT; i++) {
        uint32_t new_carry = a->limb[i] >> 31;
        a->limb[i] = (a->limb[i] << 1) | carry;
        carry = new_carry;
    }
    // carry z nejvyššího limbu se záměrně zahodí
}



void bigint_mul2_mod(bigint *a, const bigint *mod)
{
    uint32_t carry = 0;

    for (int i = 0; i < LIMB_COUNT; i++) {
        uint64_t v = ((uint64_t)a->limb[i] << 1) | carry;
        a->limb[i] = (uint32_t)v;
        carry = (uint32_t)(v >> 32);
    }

    if (carry || bigint_cmp(a, mod) >= 0)
        bigint_sub(a, mod);
}


void bigint_mod_from_full(bigint *res,
                           uint32_t *tmp,
                           const bigint *mod)
{
    bigint_zero(res);

    for (int i = DBL_LIMB_COUNT - 1; i >= 0; i--) {

        // res <<= 32  (správně: 32× *2 mod n)
        for (int k = 0; k < 32; k++)
            bigint_mul2_mod(res, mod);

        // res += tmp[i]
        uint64_t acc = (uint64_t)res->limb[0] + tmp[i];
        res->limb[0] = (uint32_t)acc;

        uint32_t carry = (uint32_t)(acc >> 32);
        int j = 1;
        while (carry && j < LIMB_COUNT) {
            acc = (uint64_t)res->limb[j] + carry;
            res->limb[j] = (uint32_t)acc;
            carry = (uint32_t)(acc >> 32);
            j++;
        }

        if (carry || bigint_cmp(res, mod) >= 0)
            bigint_sub(res, mod);
    }
}







void bigint_mul_mod(bigint *res,
                    const bigint *a,
                    const bigint *b,
                    const bigint *mod)
{
    uint32_t tmp[DBL_LIMB_COUNT];

    bigint_mul_full(tmp, a, b);
    bigint_mod_from_full(res, tmp, mod);
}





void bigint_pow_65537(bigint *out,
                      bigint *in,
                      const bigint *mod)
{

    while (bigint_cmp(in, mod) >= 0) {
        bigint_sub(in, mod);
    }
    //bigint tmp;
    *out = *in;

    // 16× square
    for (int i = 0; i < 16; i++) {
        bigint_mul_mod(out, out, out, mod);
    }

    // × input
    bigint_mul_mod(out, out, in, mod);
}


// ---------------- main ----------------
void rsa_sig_to_hash(ui8 *hash, const sHeader *Header)
{
    bigint base, mod, result;

    ui8 base_be[256] = {0};
    ui8 mod_be[256] = {0};

    for(ui16 i = 0; i < 256; i++)
    {
        base_be[i] = Header->signature[i];
        mod_be[i] = Header->modulus[256 - 1 - i];
    }

    bigint_from_bytes_be(&base, base_be, 256);
    bigint_from_bytes_be(&mod,  mod_be,  256);

    bigint_pow_65537(&result, &base, &mod);


    uint8_t m_be[256];
    bigint_to_bytes_be(&result, m_be, 256);

    for (int i = 0; i < 32; i++)
    {
        hash[i] = m_be[224 + i];
    }
}