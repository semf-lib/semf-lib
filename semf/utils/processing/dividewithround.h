/**
 * @file dividewithround.h
 * @date 18.03.2020
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_PROCESSING_DIVIDEWITHROUND_H_
#define SEMF_UTILS_PROCESSING_DIVIDEWITHROUND_H_

#include <cstdint>

namespace semf
{
/**
 * @brief Class that divides 2 integers with a correct rounding. If the result has
 * a remainder (decimal) less than .5, it will be down rounded (result is the quotient);
 * otherwise it will be up rounded (result is quotient + 1).
 */
class DivideWithRound
{
public:
	/**
	 * @brief Divides 2 integers with the right rounding.
	 * @param dividend The number which we divide is called the dividend.
	 * @param divisor The number by which we divide is called the divisor.
	 * @return Quotient if the remainder less than .5, otherwise quotient + 1.
	 */
	static intmax_t divide(intmax_t dividend, intmax_t divisor);
};
} /* namespace semf */
#endif /* SEMF_UTILS_PROCESSING_DIVIDEWITHROUND_H_ */
