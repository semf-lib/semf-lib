/**
 * @file average.h
 * @date 22.08.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_PROCESSING_AVERAGE_H_
#define SEMF_UTILS_PROCESSING_AVERAGE_H_

#include <semf/utils/core/buffer/buffer.h>

namespace semf
{
/**
 * @brief \c Average calculates the average value of the entries of
 * a given \c Buffer or an data array. The types uint8_t, int8_t, uint16_t,
 * int16_t, uint32_t, int32_t, uint64_t and int64_t are supported.
 * @tparam Data type.
 */
template <typename T>
class Average
{
public:
	/**
	 * @brief Calculates an average out of an array.
	 * @param data Pointer to the data array.
	 * @param dataSize Size of data array.
	 * @return Average.
	 */
	static T value(const T data[], size_t dataSize);

	/**
	 * @brief Calculates an average over the content of a buffer.
	 * @param data Buffer to calculate the average over.
	 * @return Average.
	 */
	static T value(const Buffer<T>& data);

private:
	/**
	 * @brief Builds the average by dividing a sum through a divider.
	 * @note average = [highValue lowValue] / divider
	 * @param highValue High part of the sum to divide.
	 * @param lowValue Low part of the sum to divide.
	 * @param divider Number to divide the sum with.
	 * @return Average.
	 */
	static T divide(size_t highValue, T lowValue, size_t divider);
};

/**Average for unsigned 8 bit.*/
template class Average<uint8_t>;
/**Average for signed 8 bit.*/
template class Average<int8_t>;
/**Average for unsigned 16 bit.*/
template class Average<uint16_t>;
/**Average for signed 16 bit.*/
template class Average<int16_t>;
/**Average for unsigned 32 bit.*/
template class Average<uint32_t>;
/**Average for signed 32 bit.*/
template class Average<int32_t>;
/**Average for unsigned 64 bit.*/
template class Average<uint64_t>;
/**Average for signed 64 bit.*/
template class Average<int64_t>;
} /* namespace semf */
#endif /* SEMF_UTILS_PROCESSING_AVERAGE_H_ */
