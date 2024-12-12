/**
 * @file batterymodelwithoutdataset.h
 * @date 09.11.2019
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_BATTERY_BATTERYMODELWITHOUTDATASET_H_
#define SEMF_APP_BATTERY_BATTERYMODELWITHOUTDATASET_H_

#include <semf/app/battery/batterystateofcharge.h>
#include <semf/app/battery/batterystateofhealth.h>
#include <semf/utils/core/signals/signal.h>

namespace semf
{
namespace app
{
/**
 * @brief Simple battery model implementation without using a battery model.
 *
 * To use this class, inherit from it and implement the both functions \c
 * updateSoc and \c updateSoh with the calculation algorithms for state of
 * charge and state of health.
 *
 * @tparam T_CAPACITY Type for capacity in Ah.
 * @tparam T_SOCSOH Type of state of charge and state of health in percentage.
 * @tparam T_VOLTAGE Type of voltage in V.
 * @tparam T_CURRENT Type of current in A. Charging current is positive,
 * discharging is negative.
 * @tparam T_TEMPERATURE Type of temperature in °C.
 */
template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
class BatteryModelWithoutDataset : public BatteryStateOfCharge<T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>,
								   public BatteryStateOfHealth<T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>
{
public:
	/**
	 *  @brief Constructor.
	 *  @param designCapacity Designed battery capacity of a new battery in Ah.
	 */
	explicit BatteryModelWithoutDataset(T_CAPACITY designCapacity);
	explicit BatteryModelWithoutDataset(const BatteryModelWithoutDataset& other) = delete;
	virtual ~BatteryModelWithoutDataset() = default;

	/**
	 * @brief Updates the state of charge and state of health.
	 *
	 * This function should be called periodically (e.g. every 100ms).
	 * @param timeDelta Milliseconds past since last function call.
	 * @param voltage Battery voltage in V.
	 * @param current Battery current in A. Charging current is positive,
	 * discharging current is negative.
	 * @param temperature Battery temperature in °C.
	 */
	void update(uint16_t timeDelta, T_VOLTAGE voltage, T_CURRENT current, T_TEMPERATURE temperature);

	virtual void updateSoc(uint16_t timeDelta, T_VOLTAGE voltage, T_CURRENT current, T_TEMPERATURE temperature) = 0;
	virtual void updateSoh() = 0;
	/**
	 * @brief Returns the relative state of charge.
	 * @note relative soc = absolute soc * soh.
	 * @return relative soc in percent.
	 */
	T_SOCSOH relativeSoc() const;
	/**
	 * @brief Returns the absolute state of charge.
	 * @return Absolute soc in percent.
	 */
	T_SOCSOH absoluteSoc() const;
	/**
	 * @brief Returns the state of health of charge.
	 * @return soh in percent.
	 */
	T_SOCSOH soh() const;
	/**
	 * @brief Returns the designed battery capacity.
	 * @return Battery capacity in Ah.
	 */
	T_CAPACITY designCapacity() const;
	/**
	 * @brief Returns the battery capacity of a full charged battery influenced by
	 * the state of health.
	 * @return Battery capacity in Ah.
	 */
	T_CAPACITY fullChargeCapacity() const;
	/**
	 * @brief Returns the remaining battery capacity.
	 * @return Battery capacity in Ah.
	 */
	T_CAPACITY remainingCapacity() const;
	/**
	 * @brief Sets the absolute state of charge.
	 * @param soc Absolute soc in percent.
	 */
	void setAbsoluteSoc(T_SOCSOH soc);
	/**
	 * @brief Sets the state of health.
	 * @param soh Soh in percent.
	 */
	void setSoh(T_SOCSOH soh);

private:
	/**Designed battery capacity in mAh.*/
	const T_CAPACITY m_designCapacity;
	/**Momentary state of charge in percent.*/
	T_SOCSOH m_absoluteSoc = 0;
	/**Momentary state of health in percent.*/
	T_SOCSOH m_soh = 0;
};

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::BatteryModelWithoutDataset(T_CAPACITY designCapacity)
: m_designCapacity(designCapacity)
{
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
void BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::update(uint16_t timeDelta, T_VOLTAGE voltage, T_CURRENT current,
																								   T_TEMPERATURE temperature)
{
	updateSoc(timeDelta, voltage, current, temperature);
	updateSoh();
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
T_SOCSOH BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::relativeSoc() const
{
	if (m_soh != 0)
		return static_cast<T_SOCSOH>(m_absoluteSoc / m_soh * 100);
	else
		return 0;
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
T_SOCSOH BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::absoluteSoc() const
{
	return m_absoluteSoc;
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
T_SOCSOH BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::soh() const
{
	return m_soh;
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
T_CAPACITY BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::designCapacity() const
{
	return m_designCapacity;
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
T_CAPACITY BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::fullChargeCapacity() const
{
	return static_cast<T_CAPACITY>(m_designCapacity * m_soh / 100);
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
T_CAPACITY BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::remainingCapacity() const
{
	return static_cast<T_CAPACITY>(m_designCapacity * relativeSoc() / 100);
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
void BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::setAbsoluteSoc(T_SOCSOH soc)
{
	m_absoluteSoc = soc;
}

template <typename T_CAPACITY, typename T_SOCSOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
void BatteryModelWithoutDataset<T_CAPACITY, T_SOCSOH, T_VOLTAGE, T_CURRENT, T_TEMPERATURE>::setSoh(T_SOCSOH soh)
{
	m_soh = soh;
}
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_BATTERY_BATTERYMODELWITHOUTDATASET_H_ */
