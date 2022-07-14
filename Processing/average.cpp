/**
 * @file average.cpp
 *
 * @date 22.08.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <Processing/average.h>

namespace semf
{
template<typename T>
T Average<T>::value(const T data[], size_t dataSize)
{
	const Buffer<T> buffer(const_cast<T*>(data), dataSize);
	return value(buffer);
}

template<typename T>
T Average<T>::value(const Buffer<T>& data)
{
	T sumLow = 0;
	T sumLowLast = 0;
	size_t sumHigh = 0;
	size_t sumHighLast = 0;
	size_t nValuesSummed = 0;

	if (data.data() == nullptr)
	{
		return 0;
	}

	// summing up the values
	for (size_t i = 0; i < data.count(); i++)
	{
		sumLow += data[i];
		if (sumLowLast > sumLow)
		{
			sumHigh++;

			// prevent overflow from sumHigh
			if (sumHighLast > sumHigh)
			{
				sumLow = sumLowLast;
				sumHigh = sumHighLast;
				break;
			}
		}
		sumLowLast = sumLow;
		sumHighLast = sumHigh;
		nValuesSummed++;
	}

	return divide(sumHigh, sumLow, nValuesSummed);
}

template<typename T>
T Average<T>::divide(size_t highValue, T lowValue, size_t divider)
{
	int binPlacesDivider = 0;  // for counting the binary places of the divider
	T mask = 0;
	size_t divideHelper = 0;  // for saving the intermediate steps by the division
	T result = 0;

	// counting the binaryplaces of the divider
	for (int i=0; i < static_cast<int>(sizeof(divider)) * 8; i++)
	{
		if (!(divider >> i))
		{
			binPlacesDivider = i;
			break;
		}
	}
	// Make a mask that has as many ones as the divider has binary places
	for (int i = 0; i < binPlacesDivider; i++)
	{
		mask = mask << 1;
		mask += 1;
	}

	divideHelper = highValue>>((sizeof(std::size_t)*8)-binPlacesDivider)&mask;

	// do division
	for (int i=((sizeof(T) + sizeof(std::size_t)) * 8) - binPlacesDivider; i >= 0; i--)
	{
		result = result << 1;
		if (divideHelper >= divider)
		{
			result++;
			divideHelper = divideHelper-divider;
		}
		divideHelper = divideHelper << 1;

		if (i > static_cast<int>(sizeof(T))*8)
			divideHelper = divideHelper+((highValue>>(i-(sizeof(T)*8)-1))&1);
		else
			divideHelper = divideHelper+((lowValue>>(i-1))&1);
	}
	return result;
}

} /* namespace semf */
