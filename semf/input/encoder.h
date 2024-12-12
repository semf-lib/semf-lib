/**
 * @file encoder.h
 * @date 18.09.2024
 * @author vb
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_ENCODER_H_
#define SEMF_INPUT_ENCODER_H_

#include <semf/utils/core/signals/signal.h>
#include <cstdint>

namespace semf
{
class Encoder
{
public:
	/**
	 * Direction for detected encoder rotate signal
	 */
	enum class Direction : uint8_t
	{
		Incremental = 0,
		Decremental,
	};

	Encoder() = default;
	explicit Encoder(const Encoder& other) = delete;
	virtual ~Encoder() = default;

	/**
	 * @brief Start rotation detecting
	 */
	virtual void start() = 0;
	/**
	 * @brief Stop rotation detecting
	 */
	virtual void stop() = 0;
	/**
	 * @brief Get current encoder position
	 * @return current position
	 */
	virtual uint32_t position() = 0;
	/**
	 * @brief Get last rotate direction
	 * @return last direction
	 */
	virtual Direction direction() = 0;

	semf::Signal<uint32_t, Direction> positionChanged;
	semf::Signal<Direction> directionChanged;
};
}  // namespace semf

#endif  // SEMF_INPUT_ENCODER_H_
