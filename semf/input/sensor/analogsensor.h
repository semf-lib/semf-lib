/**
 * @file analogsensor.h
 * @date 29.05.2023
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_SENSOR_ANALOGSENSOR_H_
#define SEMF_INPUT_SENSOR_ANALOGSENSOR_H_

#include <semf/input/analogin.h>
#include <semf/input/sensor/hardwaresensor.h>
#include <semf/utils/core/buffer/lastindmabuffer.h>
#include <semf/utils/core/signals/slot.h>

namespace semf
{
template <typename T, size_t N_CHANNELS = 1>
/**
 * @brief Provides data from an \c AnalogIn to one to more \c Sensor objects.
 *
 * @tparam T_RAW Type of a raw sensor value, e.g. uint16_t for a 16-bit ADC value.
 * @tparam N_CHANNELS Number of ADC channels connected to this ADC unit.
 *
 * @see \c AnalogDmaSensor
 * @see \c HardwareSensor
 * @see \c Sensor
 */
class AnalogSensor : public HardwareSensor<T>
{
public:
	/**
	 * @brief Constructor.
	 * @param analogIn ADC Interface, that is used for measuring sensor value(s).
	 * @param buffer Pointer to the data buffer of the measured values.
	 * @param bufferSize Size of buffer in \c T.
	 */
	AnalogSensor(AnalogIn& analogIn, T buffer[], size_t bufferSize);
	explicit AnalogSensor(const AnalogSensor& other) = delete;
	virtual ~AnalogSensor() = default;

	void update() override;
	void update(size_t channel) override;
	LastInBuffer<T>& buffer(size_t index) override;

private:
	/**Puts the current value of sensor into the internal buffer and emits \c dataAvailable.*/
	void onDataAvailable();

	/**ADC Interface, that is used for measuring sensor value(s).*/
	AnalogIn& m_analogIn;
	/**Buffer for saving the measured adc values.*/
	LastInDmaBuffer<T> m_buffers[N_CHANNELS];
	/**Index of the current sensor.*/
	size_t m_currentChannelIndex = 0;
	/**Slot for onDataAvailable function.*/
	SEMF_SLOT(m_onDataAvailableSlot, AnalogSensor, *this, onDataAvailable);
};

template <typename T, size_t N_CHANNELS>
AnalogSensor<T, N_CHANNELS>::AnalogSensor(AnalogIn& analogIn, T buffer[], size_t bufferSize)
: m_analogIn(analogIn)
{
	m_analogIn.dataAvailable.connect(m_onDataAvailableSlot);
	for (size_t i = 0; i < N_CHANNELS; i++)
	{
		m_buffers[i].setBuffer(buffer, bufferSize);
		m_buffers[i].setStride(N_CHANNELS);
		m_buffers[i].setPos(i);
	}
}

template <typename T, size_t N_CHANNELS>
void AnalogSensor<T, N_CHANNELS>::update()
{
	m_analogIn.start();
}

template <typename T, size_t N_CHANNELS>
void AnalogSensor<T, N_CHANNELS>::update(size_t channel)
{
	(void)channel;
	SEMF_INFO("Not implemented.");
}

template <typename T, size_t N_CHANNELS>
LastInBuffer<T>& AnalogSensor<T, N_CHANNELS>::buffer(size_t index)
{
	return m_buffers[index];
}

template <typename T, size_t N_CHANNELS>
void AnalogSensor<T, N_CHANNELS>::onDataAvailable()
{
	m_buffers[m_currentChannelIndex].put(static_cast<T>(m_analogIn.value()));
	HardwareSensor<T>::dataAvailable(m_currentChannelIndex);
	m_currentChannelIndex++;
	m_currentChannelIndex %= N_CHANNELS;
}
} /* namespace semf */
#endif /* SEMF_INPUT_SENSOR_ANALOGSENSOR_H_ */
