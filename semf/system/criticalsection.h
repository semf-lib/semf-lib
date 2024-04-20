/**
 * @file criticalsection.h
 * @date 08.08.2019
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef SEMF_SYSTEM_CRITICALSECTION_H_
#define SEMF_SYSTEM_CRITICALSECTION_H_

namespace semf
{
/**
 * @brief Class for handle CriticalSections
 */
class CriticalSection
{
public:
	/**Constructor.*/
	CriticalSection();
	virtual ~CriticalSection() = default;

	/**Must call on enter a critical section.*/
	static void enter();
	/**Must call on exit a critical section.*/
	static void exit();
	/**Hal implementation for enter a critical section.*/
	virtual void halEnter() = 0;
	/**Hal implementation for exit a critical section.*/
	virtual void halExit() = 0;

private:
	/**Pointer to the halImplementation of the critical section.*/
	static CriticalSection* m_criticalSection;
};
} /* namespace semf */
#endif /* SEMF_SYSTEM_CRITICALSECTION_H_ */
