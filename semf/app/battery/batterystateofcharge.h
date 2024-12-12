/**
 * @file batterystateofcharge.h
 * @date 27.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_BATTERY_BATTERYSTATEOFCHARGE_H_
#define SEMF_APP_BATTERY_BATTERYSTATEOFCHARGE_H_

#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Interface for using state of charge in a battery model.
 * @tparam T_CAPACITY Type for capacity in Ah.
 * @tparam T_SOC Type of state of charge in percentage.
 * @tparam T_VOLTAGE Type of voltage in V.
 * @tparam T_CURRENT Type of current in A. Charging current is positive, discharging is negative.
 * @tparam T_TEMPERATURE Type of temperature in °C.
 */
template <typename T_SOC, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
class BatteryStateOfCharge
{
public:
	BatteryStateOfCharge() = default;
	explicit BatteryStateOfCharge(const BatteryStateOfCharge& ohter) = delete;
	virtual ~BatteryStateOfCharge() = default;

	/**
	 * @brief Updates the state of charge relative to the current, voltage and temperature of the battery.
	 * @param timeDelta Milliseconds past since last function call.
	 * @param voltage Battery voltage in V.
	 * @param current Battery current in A. Charging is positive, discharging is negative.
	 * @param temperature Battery temperature in °C.
	 */
	virtual void updateSoc(uint16_t timeDelta, T_VOLTAGE voltage, T_CURRENT current, T_TEMPERATURE temperature) = 0;
	/**
	 * @brief Returns the relative state of charge. This is the value normally showed on a display.
	 *
	 * It is calculated by absolute state of charge / state of health respectively with the full charged battery capacity.
	 * @return State of charge in percent.
	 */
	virtual T_SOC relativeSoc() const = 0;
	/**
	 * @brief Returns the absolute state of charge.
	 *
	 * It is calculated relative to the design capacity.
	 * @return State of charge in percent.
	 */
	virtual T_SOC absoluteSoc() const = 0;

protected:
	/**
	 * @brief Initializes the state of charge calculation by the open circuit voltage after booting.
	 * @param lastAbsoluteSoc Last known absolute state of charge in percent.
	 */
	virtual void initSocByLastSoc(T_SOC lastAbsoluteSoc) = 0;
	/**
	 * @brief Initializes the state of charge calculation by the open circuit voltage after booting.
	 * @param ocv Open circuit voltage after booting.
	 */
	virtual void initSocByOcv(T_VOLTAGE ocv) {}
	/**
	 * @brief Sets the absolute state of charge.
	 * @param soc Absolute state of charge in percent.
	 */
	virtual void setAbsoluteSoc(T_SOC soc) = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_BATTERY_BATTERYSTATEOFCHARGE_H_ */
