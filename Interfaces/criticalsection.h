/**
 * @file criticalsection.h
 *
 * @date 08.08.2019
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef INTERFACES_CRITICALSECTION_H_
#define INTERFACES_CRITICALSECTION_H_

namespace semf
{
/**
 * @brief Class for handle CriticalSections
 */
class CriticalSection
{
public:
	CriticalSection();

	virtual ~CriticalSection() = default;

	/**
	 * @brief Must call on enter a critical section
	 */
	static void enter();

	/**
	 * @brief Must call on exit a critical section
	 */
	static void exit();
	/**
	 * @brief Hal implementation for enter a critical section
	 */
	virtual void halEnter() = 0;
	/**
	 * @brief Hal implementation for exit a critical section
	 */
	virtual void halExit() = 0;

private:
	/**
	 * @brief Pointer to the halImplementation of the critical section
	 */
	static CriticalSection* m_criticalSection;
};

}  /* namespace semf */

#endif  /* INTERFACES_CRITICALSECTION_H_ */
