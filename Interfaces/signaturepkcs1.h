/**
 * @file signaturepkcs1.h
 *
 * @date 18.01.2019
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */
#ifndef INTERFACES_SIGNATUREPKCS1_H_
#define INTERFACES_SIGNATUREPKCS1_H_

#include <stdio.h>
#include <cstdint>
#include <cstring>

namespace semf
{
/**
 * @brief Class for using PKCS1 (Public-Key Cryptography Standards Version 1)  asymmetric encryption/decryption for signing and verification.
 * @note The format of the keys (public and private), modular and the data is big endian, where the MSByte is places at the byte with the minimum address.
 * @note The size of the public key \c E and private key \c D must be less the modulus \c N.
*/
class SignaturePkcs1
{
public:
	/** Enum for the output state of the function.*/
	enum State : bool
	{
		Ok = true,
		Error = false,
	};

	/** Enum for the used hash algorithm*/
	enum HashAlgorithm
	{
		Sha256,
		Md5
	};

	virtual ~SignaturePkcs1() = default;

	/**
	 * @brief Pass the modulus \c N and public key \c E to the class.
	 *
	 * \note The size of the message \c m must fulfill 0 <= m < n.
	 *
	 *
	 * @param n Pointer to the address of the modulus (in binary form).
	 * @param nLen Size of the modulus in bytes.
	 * @param e Pointer to the address of the public key (in binary form).
	 * @param eLen Size of the public key in bytes.
	 *
	 * \return  \c STATE::OK on success and \c STATE::FAILED on failure.
	 **/
	virtual State setPublicKey(const uint8_t n[], size_t nLen, const uint8_t e[], size_t eLen) = 0;
	/**
	 * @brief Pass the modulus \c N and private key \c D to the class.
	 *
	 * \note The size of the message \c m must fulfill 0 <= m < n.
	 *
	 * @param n Pointer to the address of the modulus (in binary form).
	 * @param nLen Size of the modulus in bytes.
	 * @param d Pointer to the address of the private key (in binary form).
	 * @param dLen Size of the private key in bytes.
	 *
	 * \return  \c STATE::OK on success and \c STATE::FAILED on failure.
	 **/
	virtual State setPrivateKey(const uint8_t n[], size_t nLen, const uint8_t d[], size_t dLen)= 0;
	/**
	 * \brief   The function executes a hash algorithm on the given data, and signs the hash-digest using the private key.
	 *
	 * \note    v1.5 padding is applied to the hash-digest.     // this is mbedTLS-specific
	 *
	 * \param hashDigest       The hash which should be signed.
	 * \param hashBitLen The size of the hash checksum in bits.
	 * \param sign   The signature produced from the function, it has the same size of the modular \c N.
	 * \param hashAlgorithm The hash function which is performed on the input data.
	 *
	 *  \return  \c STATE::OK on success and \c STATE::FAILED on failure.
	 */
	virtual State sign(const uint8_t hashDigest[], size_t hashBitLen, uint8_t sign[], HashAlgorithm hashAlgorithm) = 0;
	/**
	 * \brief   The function executes a verification of the data \c data with the given signature \c sign.
	 *
	 * \note    v1.5 padding is applied to the hash-digest.     // this is mbedTLS-specific
	 *
	 * \param hashDigest       The hash which should be verified.
	 * \param hashBitLen The size of the hash checksum in bits.
	 * \param sign The claimed signature, it is decrypted and  compared to the hash-digest of the data.\c N.
	 * \param hashAlgorithm The hash function which is performed on the input data.
	 *
	 *  \return  \c STATE::OK on success and \c STATE::FAILED on failure.
	 */
	virtual State verify(const uint8_t hashDigest[], size_t hashBitLen, const uint8_t sign[],
						 HashAlgorithm hashAlgorithm) = 0;

protected:
	uint8_t *m_signature = nullptr; /**< Pointer to the signature of the hash-digest (binary format).*/
};
}  /* namespace semf */
#endif  // INTERFACES_SIGNATUREPKCS1_H_
