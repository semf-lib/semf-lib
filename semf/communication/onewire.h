/**
 * @file onewire.h
 * @date Jul 19, 2024
 * @author cb, vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_COMMUNICATION_ONEWIRE_H_
#define SEMF_COMMUNICATION_ONEWIRE_H_

#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
/*
 * @brief Interface for implementing the one wire protocol. Implementation is hardware dependent.
 */
class OneWire
{
public:
	OneWire() = default;
	explicit OneWire(const OneWire& other) = delete;
	virtual ~OneWire() = default;

protected:
	/**
	 * @brief The OneWire writeBit operation. Writes a single bit on the line.
	 * @param bit The bit to write.
	 */
	virtual void writeBit(bool bit) = 0;
	/**
	 * @brief The OneWire readBit operation. Reads a single bit from the slaves.
	 */
	virtual void readBit() = 0;
	/**
	 * @brief The OneWire reset operation. Checks for the presence of slaves on the line.
	 */
	virtual void reset() = 0;

	/** Emitted when a single bit has been written. */
	semf::Signal<> bitWritten;
	/** Emitted when a single bit has been read. The read bits value is the signals parameter. */
	semf::Signal<bool> bitRead;
	/**
	 * @brief Emitted when the reset operation has finished. The read bits value signifies the presence of a slave.
	 * @param bool true when presence was detected. Otherwise - false.
	 */
	semf::Signal<bool> resetted;
};
}  // namespace semf

#endif  // SEMF_COMMUNICATION_ONEWIRE_H_
