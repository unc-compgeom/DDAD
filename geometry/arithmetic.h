/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-02-12
 */

#ifndef GE_ARITHMETIC_H
#define GE_ARITHMETIC_H

#include "common.h"

namespace DDAD {

typedef mpz_class integer;
typedef mpq_class rational;

//=============================================================================
// Interfaces
//=============================================================================

integer Floor(const rational& x);
integer FloorKeepFraction(const rational& x, rational* out_frac);
integer Ceil(const rational& x);
integer CeilKeepFraction(const rational& x, rational* out_frac);

//=============================================================================
// Floor/Ceiling
//=============================================================================

/*!
 * @brief Floor - rounds a rational toward -infinity.
 * \param r - rational to be rounded.
 * \return nearest integer in the -infinity direction.
 */
inline integer Floor(const rational& x) {
    integer out_int;

    // mpz_fdiv_q(q, n, d), n = q*d+r
    mpz_fdiv_q(out_int.get_mpz_t(), x.get_num_mpz_t(), x.get_den_mpz_t());

    return out_int;
}

/*!
 * @brief FloorKeepFraction - rounds a rational toward -infinity, returning
 * the fractional portion to the user.
 * \param x - rational to be rounded.
 * \param out_frac - fractional remainder, 0 <= |out_frac| < 1.
 * \return nearest integer in the -infinity direction.
 */
inline integer FloorKeepFraction(const rational& x, rational* out_frac) {
    integer out_int;
    integer out_frac_n;

    // mpz_fdiv_qr(q, r, n, d), n = q*d+r
    mpz_fdiv_qr(out_int.get_mpz_t(), out_frac_n.get_mpz_t(), x.get_num_mpz_t(),
                x.get_den_mpz_t());

    *out_frac = rational(out_frac_n, x.get_den());
    return out_int;
}

/*!
 * @brief Ceil - rounds a rational toward +infinity.
 * \param r - rational to be rounded.
 * \return nearest integer in the +infinity direction.
 */
inline integer Ceil(const rational &x) {
    integer out_int;

    // mpz_cdiv_q(q, n, d), n = q*d+r
    mpz_cdiv_q(out_int.get_mpz_t(), x.get_num_mpz_t(), x.get_den_mpz_t());

    return out_int;
}

/*!
 * @brief CeilKeepFraction - rounds a rational toward +infinity, returning
 * the fractional portion to the user.
 * \param x - rational to be rounded.
 * \param out_frac - fractional remainder, 0 <= |out_frac| < 1.
 * \return nearest integer in the +infinity direction.
 */
inline integer CeilKeepFraction(const rational &x, rational *out_frac) {
    integer out_int;
    integer out_frac_n;

    // mpz_cdiv_qr(q, r, n, d), n = q*d+r
    mpz_cdiv_qr(out_int.get_mpz_t(), out_frac_n.get_mpz_t(), x.get_num_mpz_t(),
                x.get_den_mpz_t());

    *out_frac = rational(out_frac_n, x.get_den());
    return out_int;
}

/*
class Integer {
public:

private:
    std::vector<uint32_t> value_;
};
*/

} // namespace DDAD

#endif // GE_ARITHMETIC_H
