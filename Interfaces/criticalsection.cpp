/**
 * @file criticalsection.cpp
 *
 * @date 08.08.2019
 * @author nk
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 * 			  For detailed information, read the license file in the project
 * 			  root directory.
 */

#include <Interfaces/criticalsection.h>

namespace semf
{
CriticalSection* CriticalSection::m_criticalSection = nullptr;
CriticalSection::CriticalSection()
{
	m_criticalSection = this;
}

void CriticalSection::enter()
{
	m_criticalSection->halEnter();
}

void CriticalSection::exit()
{
	m_criticalSection->halExit();
}

}  /* namespace semf */
