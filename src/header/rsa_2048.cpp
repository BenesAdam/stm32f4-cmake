#include "rsa_2048.hpp"
#include <string.h>

#define LIMB_BITS   32
#define LIMB_COUNT  64
#define DBL_LIMB_COUNT (2 * LIMB_COUNT)

typedef struct 
{
  ui32 limb[LIMB_COUNT];
} bigint;

void bigint_zero(bigint *a) 
{
  for (int i = 0; i < LIMB_COUNT; i++)
  {
    a->limb[i] = 0;
  }
}

int bigint_cmp(const bigint *a, const bigint *b)
{
  for (int i = LIMB_COUNT - 1; i >= 0; i--)
  {
    if (a->limb[i] > b->limb[i]) return 1;
    if (a->limb[i] < b->limb[i]) return -1;
  }

  return 0;
}

void bigint_sub(bigint *a, const bigint *b)
{
  ui64 borrow = 0;

  for (int i = 0; i < LIMB_COUNT; i++)
  {
    ui64 tmp = (ui64)a->limb[i] - b->limb[i] - borrow;
    a->limb[i] = (ui32)tmp;
    borrow = (tmp >> 63) & 1;
  }
}

void bigint_shift_right_one(bigint *a) 
{
  ui32 carry = 0;

  for (int i = LIMB_COUNT - 1; i >= 0; i--)
  {
    ui32 new_carry = a->limb[i] & 1;
    a->limb[i] = (a->limb[i] >> 1) | (carry << 31);
    carry = new_carry;
  }
}

int bigint_is_zero(const bigint *a) 
{
  for (int i = 0; i < LIMB_COUNT; i++)
  {
    if (a->limb[i]) return 0;
  }

  return 1;
}

void bigint_one(bigint *a)
{
  bigint_zero(a);
  a->limb[0] = 1;
}

void bigint_from_bytes_be(bigint *dst,
    const ui8 *src,
    size_t len)
{
  bigint_zero(dst);

  for (size_t i = 0; i < len; i++) {
    size_t byte  = len - 1 - i;
    size_t limb  = i / 4;
    size_t shift = (i % 4) * 8;

    if (limb < LIMB_COUNT) {
      dst->limb[limb] |= ((ui32)src[byte]) << shift;
    }
  }
}

void bigint_to_bytes_be(const bigint *x,
    ui8 *dst,
    size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    size_t src_index = len - 1 - i;
    size_t limb = src_index / 4;
    size_t shift = (src_index % 4) * 8;

    dst[i] = (x->limb[limb] >> shift) & 0xFF;
  }
}

void bigint_mul_full(ui32 *tmp,
    const bigint *a,
    const bigint *b)
{
  memset(tmp, 0, sizeof(ui32) * DBL_LIMB_COUNT);

  for (int i = 0; i < LIMB_COUNT; i++)
  {
    ui64 carry = 0;

    for (int j = 0; j < LIMB_COUNT; j++) {
      ui64 r =
        tmp[i + j] +
        (ui64)a->limb[i] * b->limb[j] +
        carry;

      tmp[i + j] = (ui32)r;
      carry = r >> 32;
    }

    tmp[i + LIMB_COUNT] += (ui32)carry;
  }
}

void bigint_shift_left_one(bigint *a)
{
  ui32 carry = 0;

  for (int i = 0; i < LIMB_COUNT; i++)
  {
    ui32 new_carry = a->limb[i] >> 31;
    a->limb[i] = (a->limb[i] << 1) | carry;
    carry = new_carry;
  }
}

void bigint_mul2_mod(bigint *a, const bigint *mod)
{
  ui32 carry = 0;

  for (int i = 0; i < LIMB_COUNT; i++)
  {
    ui64 v = ((ui64)a->limb[i] << 1) | carry;
    a->limb[i] = (ui32)v;
    carry = (ui32)(v >> 32);
  }

  if (carry || bigint_cmp(a, mod) >= 0)
  {
    bigint_sub(a, mod);
  }
}

void bigint_mod_from_full(bigint *res,
    ui32 *tmp,
    const bigint *mod)
{
  bigint_zero(res);

  for (int i = DBL_LIMB_COUNT - 1; i >= 0; i--)
  {

    for (int k = 0; k < 32; k++)
    {
      bigint_mul2_mod(res, mod);
    }

    ui64 acc = (ui64)res->limb[0] + tmp[i];
    res->limb[0] = (ui32)acc;

    ui32 carry = (ui32)(acc >> 32);
    int j = 1;
    while (carry && j < LIMB_COUNT)
    {
      acc = (ui64)res->limb[j] + carry;
      res->limb[j] = (ui32)acc;
      carry = (ui32)(acc >> 32);
      j++;
    }

    if (carry || bigint_cmp(res, mod) >= 0)
    {
      bigint_sub(res, mod);
    }
  }
}

void bigint_mul_mod(bigint *res,
    const bigint *a,
    const bigint *b,
    const bigint *mod)
{
  ui32 tmp[DBL_LIMB_COUNT];

  bigint_mul_full(tmp, a, b);
  bigint_mod_from_full(res, tmp, mod);
}

void bigint_pow_65537(bigint *out,
    bigint *in,
    const bigint *mod)
{
  while (bigint_cmp(in, mod) >= 0) 
  {
    bigint_sub(in, mod);
  }

  *out = *in;

  for (int i = 0; i < 16; i++)
  {
    bigint_mul_mod(out, out, out, mod);
  }

  bigint_mul_mod(out, out, in, mod);
}

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


  ui8 m_be[256];
  bigint_to_bytes_be(&result, m_be, 256);

  for (int i = 0; i < 32; i++)
  {
    hash[i] = m_be[224 + i];
  }
}

