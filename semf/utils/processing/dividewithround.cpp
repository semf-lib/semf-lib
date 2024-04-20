/**
 * @file dividewithround.cpp
 * @date 18.03.2020
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/processing/dividewithround.h>

namespace semf
{
intmax_t DivideWithRound::divide(intmax_t dividend, intmax_t divisor)
{
	intmax_t result = dividend / divisor;
	if (((result + 1) * divisor - dividend) < (dividend - (result * divisor)))
	{
		result = result + 1;
	}
	return result;
}
} /* namespace semf */
