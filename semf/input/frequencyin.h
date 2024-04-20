/**
 * @file frequencyin.h
 * @date 15.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_INPUT_FREQUENCYIN_H_
#define SEMF_INPUT_FREQUENCYIN_H_

#include <semf/app/input/frequencyin.h>
#include <semf/utils/core/signals/slot.h>

namespace semf
{
/**
 * @copydoc app::FrequencyIn
 */
class FrequencyIn : public app::FrequencyIn
{
public:
	/**
	 * @brief Constructor.
	 * @param inputCapture Reference to a hardware interface.
	 */
	explicit FrequencyIn(InputCapture& inputCapture);
	explicit FrequencyIn(const FrequencyIn& other) = delete;
	//! @cond Doxygen_Suppress
	virtual ~FrequencyIn() = default;
	//! @endcond

	void start() override;
	void stop() override;
	void reset() override;
	uint32_t periodTicks() const override;
	uint32_t periodInMs() const override;
	uint32_t frequency() const override;

protected:
	/**This method must be called from the interrupt routine to update the frequency.*/
	virtual void onDataAvailable();

private:
	/**Hardware interface.*/
	InputCapture& m_inputCapture;
	/**Tick number of the Last measurement cycle.*/
	uint32_t m_lastTicks = 0;
	/**Delta Tick of the Last measurement cycle.*/
	uint32_t m_lastPeriod = 0;
	/**Slot for \c onDataAvailable function.*/
	SEMF_SLOT(m_onDataAvailableSlot, FrequencyIn, *this, onDataAvailable);
};
} /* namespace semf */
#endif /* SEMF_INPUT_FREQUENCYIN_H_ */
