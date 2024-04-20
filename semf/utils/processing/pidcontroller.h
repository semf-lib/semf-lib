/**
 * @file pidcontroller.h
 * @date 20.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_UTILS_PROCESSING_PIDCONTROLLER_H_
#define SEMF_UTILS_PROCESSING_PIDCONTROLLER_H_

#include <cfloat>

namespace semf
{
/**
 * @brief Implementation of an pid (proportional integral derivative) regulator.
 */
class PidController
{
public:
	/**
	 * @brief Constructor
	 * @param kp Proportional coefficient.
	 * @param ki Integral coefficient.
	 * @param kd Derivative coefficient.
	 * @param sampleTime The sampling time [s].
	 * @param yMin The minimum output value.
	 * @param yMax The maximum output value.
	 * @param setpoint The value on which the output is to be regulated.
	 * @param yStart This value is used for the first calculation.
	 * @param deltaMax The maximum permissible change of the output value.
	 */
	PidController(double kp, double ki, double kd, double sampleTime, double yMin, double yMax, double setpoint = 0, double yStart = 0,
				  double deltaMax = DBL_MAX);
	virtual ~PidController() = default;

	/**
	 * @brief Set the proportional gain.
	 * @param kp The new gain
	 */
	void setKp(double kp);
	/**
	 * @brief Returns the proportional gain.
	 * @return The proportional gain.
	 */
	double kp() const;
	/**
	 * @brief Set the gain of the integral.
	 * @param ki The new gain.
	 */
	void setKi(double ki);
	/**
	 * @brief Get the gain of the integral.
	 * @return The gain.
	 */
	double ki() const;
	/**
	 * @brief Set the gain of the derivative.
	 * @param kd The new gain.
	 */
	void setKd(double kd);
	/**
	 * @brief Returns the gain of the derivative.
	 * @return The gain of the derivative.
	 */
	double kd() const;
	/**
	 * @brief Set the sample time of the controller.
	 * @param sampleTime The sample time.
	 */
	void setSampleTime(double sampleTime);
	/**
	 * @brief Returns the current configured sample time.
	 * @return The sample time.
	 */
	double sampleTime() const;
	/**
	 * @brief Set the controller set point.
	 * @param setpoint The set point.
	 */
	void setSetpoint(double setpoint);
	/**
	 * @brief Returns the current set point.
	 * @return The current set point.
	 */
	double setpoint() const;
	/**
	 * @brief Limits the maximum value of the output.
	 * @param max The maximum value of the output.
	 */
	void setMaxOutput(double max);
	/**
	 * @brief Returns the maximum value which the output is allowed to have.
	 * @return The maximum limit of the output.
	 */
	double maxOuput() const;
	/**
	 * @brief Set the minimum possible output.
	 * @param min The minimum possible output.
	 */
	void setMinOutput(double min);
	/**
	 * @brief Returns the minimum possible output.
	 * @return The minimum possible output.
	 */
	double minOutput() const;
	/**
	 * @brief Set the max delta of the output between two samples.
	 * @param delta The max delta of the output.
	 */
	void setMaxDelta(double delta);
	/**
	 * @brief Returns Set the max delta of the output between two samples.
	 * @return The max delta of the output.
	 */
	double maxDelta() const;
	/**
	 * @brief Resets the past sampled values which will be used
	 * for the calculation of the integral and derivative.
	 * @param yStart The start value for the integral (should 0 of no integral is used).
	 */
	void reset(double yStart = 0);
	/**
	 * @brief Calculates the output value dependent to the actual value.
	 * This method must be called cyclically at the interval of the set sampling time.
	 * @param currentValue The current value.
	 * @return The new output value of the controller (the input to the plant).
	 */
	virtual double calculate(double currentValue);

private:
	/** Calculate q0 dependent on Kp, Ki, Kd and the sampleTime.*/
	void calculateQ0();
	/** Calculate q0 dependent on Kp, Kd and the sampleTime.*/
	void calculateQ1();
	/** Calculate q0 dependent on Kd and the sampleTime.*/
	void calculateQ2();

	double m_kp;         /**<Current Kp.*/
	double m_ki;         /**<Current Ki.*/
	double m_kd;         /**<Current Kd.*/
	double m_sampleTime; /**< Sample time in seconds.*/
	double m_q0;         /**< Kp+Ki*smapleTime+(Kd/sampleTime)*/
	double m_q1;         /**< -Kp-2*(Kd/sampleTime)*/
	double m_q2;         /**< Kd/sampleTime */
	double m_yMin;       /**< The mininmum output value.*/
	double m_yMax;       /**< The maximum output value.*/
	double m_deltaMax;   /**< The maximum change limit of the output.*/
	double m_setpoint;   /**< The value on which the output is regulated.*/
	double m_yLast1;     /**< y(n-1)*/
	double m_eLast1;     /**< e(n-1)*/
	double m_eLast2;     /**< e(n-2)*/
};
} /* namespace semf */
#endif /* SEMF_UTILS_PROCESSING_PIDCONTROLLER_H_ */
