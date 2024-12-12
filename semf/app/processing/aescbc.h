/**
 * @file aescbc.h
 * @date 21.03.2019
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_PROCESSING_AESCBC_H_
#define SEMF_APP_PROCESSING_AESCBC_H_

#include <cstddef>
#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Abstract class for AES-CBC (Advanced Encryption Standard-Cipher Block Chaining) for symmetric
 * encryption/decryption.
 * @warning The format of the keys (public and private), modular and the data are in big endian format, where
 * the MSByte is placed at the byte with the minimum address.
 */
class AesCbc
{
public:
	/** Enum for the output state of a method.*/
	enum State : bool
	{
		Ok = true,     /**< Successful execution of a method.*/
		Error = false, /**< Failed execution of a method.*/
	};

	AesCbc() = default;
	explicit AesCbc(const AesCbc& other) = delete;
	virtual ~AesCbc() = default;

	/**
	 * @brief Sets the AES key.
	 * @param key Pointer to the address of the key (as binary).
	 * @param size Size of the key in bytes.
	 * @warning Only sizes of 128, 192, or 256 bits are supported.
	 **/
	virtual void setKey(const uint8_t key[], size_t size) = 0;
	/**
	 * @brief Sets the initialization vector.
	 * @attention The initialization vector must have the same size as the key.
	 * @param initializationVector Pointer to the address of the initialization vector (as binary).
	 **/
	virtual void setInitializationVector(const uint8_t initializationVector[]) = 0;
	/**Resets the initialization vector.*/
	virtual void resetInitializationVector() = 0;
	/**
	 * @brief Encrypts the given data using AES-CBC algorithm.
	 * @warning Initialization vector has to be set before using this function.
	 * @note The content of the initialization vector is updated so that you can
	 * call the same function again on the next block(s) of data and get the same result
	 * as if it is encrypted in one call. This allows a "streaming" usage.
	 * If you need to retain the contents of the initialization vector, you should
	 * either save it manually or use the cipher module instead.
	 * @param dataIn Input data which has to be encrypted.
	 * @param bufferOut Encrypted data.
	 * @param size Size of input data in bytes. it must be a
	 *  multiple of the block size (16 Bytes).
	 * @return  \c State::Ok on success and \c State::Error on failure.
	 */
	virtual State encrypt(const uint8_t dataIn[], uint8_t bufferOut[], size_t size) = 0;
	/**
	 * @brief   Performs an AES-CBC decryption on the given (encrypted) data.
	 * @warning    The initialization vector has to be set before using this function.
	 * @note    The content of the initialization vector is updated so that you can
	 * call the same function again on the next block(s) of data and get the same
	 * result as if it was encrypted in one call. This allows a "streaming" usage.
	 * If you need to retain the contents of the initialization vector, you should
	 * either save it manually or use the cipher module instead.
	 * @param dataIn Input data which has to be decrypted.
	 * @param bufferOut Decrypted data.
	 * @param size Size of input data in bytes. It must be a
	 * multiple of the block size (16 Bytes).
	 * @return  \c State::Ok on success and \c State::Error on failure.
	 */
	virtual State decrypt(const uint8_t dataIn[], uint8_t bufferOut[], size_t size) = 0;
};
} /* namespace app */
} /* namespace semf */
#endif  // SEMF_APP_PROCESSING_AESCBC_H_
