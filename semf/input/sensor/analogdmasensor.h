/**
 * @file analogdmasensor.h
 * @date 29.05.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_SENSOR_ANALOGDMASENSOR_H_
#define SEMF_INPUT_SENSOR_ANALOGDMASENSOR_H_

#include <semf/input/analogindma.h>
#include <semf/input/sensor/hardwaresensor.h>
#include <semf/utils/core/buffer/lastindmabuffer.h>
#include <semf/utils/core/signals/slot.h>

namespace semf
{
/**
 * @brief Provides data from an \c AnalogInDma to one to more \c Sensor objects.
 *
 * @tparam T_RAW Type of a raw sensor value, e.g. uint16_t for a 16-bit ADC value.
 * @tparam N_CHANNELS Number of ADC channels connected to this ADC unit.
 *
 * @see \c AnalogDmaSensor
 * @see \c HardwareSensor
 * @see \c Sensor
 */
template <typename T, size_t N_CHANNELS = 1>
class AnalogDmaSensor : public HardwareSensor<T>
{
public:
	/**
	 * @brief Constructor.
	 * @param analogInDma ADC interface, that is used for measuring sensor value(s).
	 * @param buffer Pointer to the data buffer of the measured values.
	 * @param bufferSize Size of buffer in \c T.
	 */
	AnalogDmaSensor(AnalogInDma& analogInDma, T buffer[], size_t bufferSize);
	explicit AnalogDmaSensor(const AnalogDmaSensor& other) = delete;
	virtual ~AnalogDmaSensor() = default;

	void update() override;
	void update(size_t channel) override;
	LastInBuffer<T>& buffer(size_t index) override;

private:
	/**Puts the current value of sensor into the internal buffer and emits \c dataAvailable.*/
	void onDataAvailable();

	/**ADC with DMA interface, that is used for measuring sensor value(s).*/
	AnalogInDma& m_analogInDma;
	/**Buffer for saving the measured values.*/
	LastInDmaBuffer<T> m_buffers[N_CHANNELS];
	/**Buffer for saving the measured values.*/
	T* m_data = nullptr;
	/**Size of the data*/
	size_t m_dataSize = 0;
	/**Slot for onDataAvailable function.*/
	SEMF_SLOT(m_onDataAvailableSlot, AnalogDmaSensor, *this, onDataAvailable);
};

template <typename T, size_t N_CHANNELS>
AnalogDmaSensor<T, N_CHANNELS>::AnalogDmaSensor(AnalogInDma& analogInDma, T buffer[], size_t bufferSize)
: m_analogInDma(analogInDma),
  m_data(buffer),
  m_dataSize(bufferSize)
{
	m_analogInDma.dataAvailable.connect(m_onDataAvailableSlot);
	for (size_t i = 0; i < N_CHANNELS; i++)
	{
		m_buffers[i].setBuffer(buffer, bufferSize);
		m_buffers[i].setPos(i);
		m_buffers[i].setStride(N_CHANNELS);
	}
}

template <typename T, size_t N_CHANNELS>
void AnalogDmaSensor<T, N_CHANNELS>::update()
{
	m_analogInDma.start(reinterpret_cast<uint8_t*>(m_data), m_dataSize * sizeof(T));
}

template <typename T, size_t N_CHANNELS>
void AnalogDmaSensor<T, N_CHANNELS>::update(size_t channel)
{
	(void)channel;
	SEMF_INFO("Not implemented.");
}

template <typename T, size_t N_CHANNELS>
LastInBuffer<T>& AnalogDmaSensor<T, N_CHANNELS>::buffer(size_t index)
{
	return m_buffers[index];
}

template <typename T, size_t N_CHANNELS>
void AnalogDmaSensor<T, N_CHANNELS>::onDataAvailable()
{
	for (size_t i = 0; i < N_CHANNELS; i++)
		HardwareSensor<T>::dataAvailable(i);
}
} /* namespace semf */
#endif /* SEMF_INPUT_SENSOR_ANALOGDMASENSOR_H_ */
