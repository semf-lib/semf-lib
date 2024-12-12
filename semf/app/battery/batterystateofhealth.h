/**
 * @file batterystateofhealth.h
 * @date 27.01.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_APP_BATTERY_BATTERYSTATEOFHEALTH_H_
#define SEMF_APP_BATTERY_BATTERYSTATEOFHEALTH_H_

#include <cstdint>

namespace semf
{
namespace app
{
/**
 * @brief Interface for using state of health in a battery model.
 * @tparam T_SOH Type of state of health in percentage.
 * @tparam T_VOLTAGE Type of voltage in V.
 * @tparam T_CURRENT Type of current in A. Charging current is positive, discharging is negative.
 * @tparam T_TEMPERATURE Type of temperature in °C.
 */
template <typename T_SOH, typename T_VOLTAGE, typename T_CURRENT, typename T_TEMPERATURE>
class BatteryStateOfHealth
{
public:
	BatteryStateOfHealth() = default;
	explicit BatteryStateOfHealth(const BatteryStateOfHealth& other) = delete;
	virtual ~BatteryStateOfHealth() = default;

	/**Updates the state of health.*/
	virtual void updateSoh() = 0;
	/**
	 * @brief Returns the state of health.
	 * @return State of health in percent.
	 */
	virtual T_SOH soh() const = 0;

protected:
	/**
	 * @brief Initializes the state of health calculation by the last known state of charge after booting.
	 * @param lastSoh Last known state of health in percent.
	 */
	virtual void initSoh(T_SOH lastSoh) = 0;
	/**
	 * @brief Sets the state of health.
	 * @param soh State of health in percent.
	 */
	virtual void setSoh(T_SOH soh) = 0;
};
} /* namespace app */
} /* namespace semf */
#endif /* SEMF_APP_BATTERY_BATTERYSTATEOFHEALTH_H_ */
