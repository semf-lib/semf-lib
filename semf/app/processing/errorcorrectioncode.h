/**
 * @file errorcorrectioncode.h
 * @date 07.06.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_PROCESSING_ERRORCORRECTIONCODE_H_
#define SEMF_APP_PROCESSING_ERRORCORRECTIONCODE_H_

#include <semf/utils/core/signals/signal.h>
#include <semf/utils/core/error.h>
#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Interface for error correction code implementations.
 */
class ErrorCorrectionCode
{
public:
	ErrorCorrectionCode() = default;
	explicit ErrorCorrectionCode(const ErrorCorrectionCode& other) = delete;
	virtual ~ErrorCorrectionCode() = default;

	/**
	 * @brief Add ecc bits to the data and write them to the buffer.
	 * @param data Data which should be encoded.
	 * @param dataBitSize The size of the data.
	 * @param buffer The buffer for storing the data including ecc bits.
	 * @param bufferSize The size of the buffer.
	 * @return The number of bits that have been written into the buffer.
	 */
	virtual size_t encode(const uint8_t data[], size_t dataBitSize, uint8_t buffer[], size_t bufferSize) const = 0;
	/**
	 * @brief Remove ecc bits from the data, corrects bit errors and write them to the buffer.
	 * @param data Data including ecc bits.
	 * @param dataSize The size of data.
	 * @param buffer Buffer for storing the corrected data without ecc bits.
	 * @param bufferSize The size of the buffer.
	 * @return The number of bits that have been written into the buffer.
	 */
	virtual size_t decode(const uint8_t data[], size_t dataSize, uint8_t buffer[], size_t bufferSize) const = 0;
	/**
	 * @brief Calculates the size of the bits which are needed to store a certain amount of bits with ecc.
	 * @param messageBitSize Bit size of message without ecc bits.
	 * @return The bits which are needed to store the message bits with ecc.
	 */
	virtual size_t blockBitSize(size_t messageBitSize) const = 0;

	/**Signal is emitted after an error occurred.*/
	Signal<Error> error;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_PROCESSING_ERRORCORRECTIONCODE_H_ */
