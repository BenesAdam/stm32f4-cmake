#ifndef RSA_2048_HPP
#define RSA_2048_HPP

#include "types.h"
#include "header_structure.hpp"

/* ---------- EXTRACT HASH ---------- */
void rsa_sig_to_hash(ui8 *hash, const sHeader *Header);

#endif // #ifndef RSA_2048_HPP
