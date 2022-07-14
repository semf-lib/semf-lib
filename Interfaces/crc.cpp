/**
 * @file crc.cpp
 *
 * @date 21.11.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#include <Interfaces/crc.h>

namespace semf
{
size_t Crc::byteSize() const
{
	size_t ret = bitSize() / 8;
	return (bitSize() % 8 == 0) ? ret : ret + 1;
}

}  /* namespace semf */
