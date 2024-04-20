/**
 * @file pidcontroller.cpp
 * @date 20.03.2018
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/utils/core/debug.h>
#include <semf/utils/processing/pidcontroller.h>
#include <cfloat>

namespace semf
{
PidController::PidController(double kp, double ki, double kd, double sampleTime, double yMin, double yMax, double setpoint, double yStart, double deltaMax)
: m_kp(kp),
  m_ki(ki),
  m_kd(kd),
  m_sampleTime(sampleTime),
  m_yMin(yMin),
  m_yMax(yMax),
  m_deltaMax(deltaMax),
  m_setpoint(setpoint)
{
	calculateQ0();
	calculateQ1();
	calculateQ2();

	// if m_ki>0 init the integral with the start value
	if (m_ki != 0)
		m_yLast1 = yStart;
	else
		m_yLast1 = 0;
}

void PidController::setKp(double kp)
{
	SEMF_INFO("set kp to: %f", kp);
	m_kp = kp;
	calculateQ0();
	calculateQ1();
}

double PidController::kp() const
{
	return m_kp;
}

void PidController::setKi(double ki)
{
	SEMF_INFO("set ki to: %f", ki);
	m_ki = ki;
	calculateQ0();
}

double PidController::ki() const
{
	return m_ki;
}

void PidController::setKd(double kd)
{
	SEMF_INFO("set kd to: %f", kd);
	m_kd = kd;
	calculateQ0();
	calculateQ1();
	calculateQ2();
}

double PidController::kd() const
{
	return m_kd;
}

void PidController::setSampleTime(double sampleTime)
{
	SEMF_INFO("set sample time to: %f", sampleTime);
	m_sampleTime = sampleTime;
	calculateQ0();
	calculateQ1();
	calculateQ2();
}

double PidController::sampleTime() const
{
	return m_sampleTime;
}

void PidController::setSetpoint(double setpoint)
{
	SEMF_INFO("set setpoint to: %f", setpoint);
	m_setpoint = setpoint;
}

double PidController::setpoint() const
{
	return m_setpoint;
}

void PidController::setMaxOutput(double max)
{
	SEMF_INFO("set max output to: %f", max);
	m_yMax = max;
}

double PidController::maxOuput() const
{
	return m_yMax;
}

void PidController::setMinOutput(double min)
{
	SEMF_INFO("set min output to: %f", min);
	m_yMin = min;
}

double PidController::minOutput() const
{
	return m_yMin;
}

void PidController::setMaxDelta(double delta)
{
	SEMF_INFO("set max delta to: %f", delta);
	m_deltaMax = delta;
}

double PidController::maxDelta() const
{
	return m_deltaMax;
}

void PidController::reset(double yStart)
{
	SEMF_INFO("to yStart: %f", yStart);
	// if m_ki>0 init the integral with the start value
	if (m_ki != 0)
		m_yLast1 = yStart;
	else
		m_yLast1 = 0;
	m_eLast1 = 0;
	m_eLast2 = 0;
}

double PidController::calculate(double currentValue)
{
	double e = m_setpoint - currentValue;

	double y = m_yLast1 + m_q0 * e + m_q1 * m_eLast1 + m_q2 * m_eLast2;

	// limit the min and max value of the output
	if (y > m_yMax)
	{
		SEMF_INFO("y (%f) set to max (%f)", y, m_yMax);
		y = m_yMax;
	}
	else if (y < m_yMin)
	{
		SEMF_INFO("y (%f) set to min (%f)", y, m_yMin);
		y = m_yMin;
	}

	// limit the delta from the output
	if ((y - m_yLast1) > m_deltaMax)
	{
		SEMF_INFO("limit delta (%f) to max delta (%f)", y - m_yLast1, m_deltaMax);
		y = m_yLast1 + m_deltaMax;
	}
	else if ((m_yLast1 - y) > m_deltaMax)
	{
		SEMF_INFO("limit delta (%f) to max delta (%f)", m_yLast1 - y, m_deltaMax);
		y = m_yLast1 - m_deltaMax;
	}

	m_yLast1 = y;
	m_eLast2 = m_eLast1;
	m_eLast1 = e;
	SEMF_INFO("current value: %f, y: %f", currentValue, y);
	return y;
}

void PidController::calculateQ0()
{
	m_q0 = m_kp + m_ki * m_sampleTime + (m_kd / m_sampleTime);
	SEMF_INFO("q0: %f", m_q0);
}

void PidController::calculateQ1()
{
	m_q1 = -m_kp - 2 * m_kd / m_sampleTime;
	SEMF_INFO("q1: %f", m_q1);
}

void PidController::calculateQ2()
{
	m_q2 = m_kd / m_sampleTime;
	SEMF_INFO("q2: %f", m_q2);
}
} /* namespace semf */
