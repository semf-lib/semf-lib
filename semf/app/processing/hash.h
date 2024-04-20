/**
 * @file hash.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_PROCESSING_HASH_H_
#define SEMF_APP_PROCESSING_HASH_H_

#include <cstddef>
#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Interface for hash classes
 */
class Hash
{
public:
	virtual ~Hash() = default;

	/**Starts hash computation.**/
	virtual void start() = 0;
	/**
	 * @brief Updates the hash with a new chunk of data.
	 * \param data Pointer to a new chunk of data.
	 * \param dataSize Size of a new chunk of data in bytes.
	 **/
	virtual void update(uint8_t data[], size_t dataSize) = 0;
	/**
	 * @brief Returns the hash result.
	 * \param buffer Pointer to a buffer for storing the hash result.
	 * \param bufferSize Size of the \c buffer in bytes.
	 **/
	virtual void finish(uint8_t buffer[], size_t bufferSize) = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_PROCESSING_HASH_H_ */
