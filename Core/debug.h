/**
 * @file debug.h
 *
 * @date 01.11.2020
 * @author fs
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef CORE_DEBUG_H_
#define CORE_DEBUG_H_

#include <Core/Lists/linkedlist.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef USE_SEMF_DEBUG

#define SEMF_DEBUG_ADD_CLASS(T, level) semf::Debug<T>::debugClass(level)
#define SEMF_DEBUG_ADD_OBJECT(object, level) semf::debugObject<__COUNTER__>(object, level, #object)

#define SEMF_ERROR(...) semf::internalDebug(this, semf::DebugLevel::DebuglevelError, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_WARNING(...) semf::internalDebug(this, semf::DebugLevel::DebuglevelWarning, __PRETTY_FUNCTION__,  ##__VA_ARGS__)
#define SEMF_INFO(...) semf::internalDebug(this, semf::DebugLevel::DebuglevelInfo, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_DEBUG(...) semf::internalDebug(this, semf::DebugLevel::DebuglevelDebug, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_TRACE(...) semf::internalDebug(this, semf::DebugLevel::DebuglevelTrace, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define SEMF_SINGLETON_ERROR(object, ...) semf::internalDebug(object, semf::DebugLevel::DebuglevelError, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_SINGLETON_WARNING(object, ...) semf::internalDebug(object, semf::DebugLevel::DebuglevelWarning, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_SINGLETON_INFO(object, ...) semf::internalDebug(object, semf::DebugLevel::DebuglevelInfo, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_SINGLETON_DEBUG(object, ...) semf::internalDebug(object, semf::DebugLevel::DebuglevelDebug, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define SEMF_SINGLETON_TRACE(object, ...) semf::internalDebug(object, semf::DebugLevel::DebuglevelTrace, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#else
#define SEMF_DEBUG_ADD_CLASS(T, level)
#define SEMF_DEBUG_ADD_OBJECT(object, level)

#define SEMF_ERROR(...)
#define SEMF_WARNING(...)
#define SEMF_INFO(...)
#define SEMF_DEBUG(...)
#define SEMF_TRACE(...)

#define SEMF_SINGLETON_ERROR(object, ...)
#define SEMF_SINGLETON_WARNING(object, ...)
#define SEMF_SINGLETON_INFO(object, ...)
#define SEMF_SINGLETON_DEBUG(object, ...)
#define SEMF_SINGLETON_TRACE(object, ...)
#endif  // USE_SEMF_DEBUG

namespace semf
{
/**@brief Levels for differing debug messages.*/
enum class DebugLevel
{
	/**Error level: used for a problems that makes the application work wrongly but not crashing.*/
	DebuglevelError = 0,
	/**Warning level: used for non-fatal warnings detected by the application.*/
	DebuglevelWarning,
	/**Info level: used for informational records.*/
	DebuglevelInfo,
	/**Debug level: used for a software debugging.*/
	DebuglevelDebug,
	/** Trace level: used for internal code tracking.*/
	DebuglevelTrace,
	/** Ignore level: used to ignore logs.*/
	DebuglevelIgnore = 255
};

/**
 * @brief Class helps debugging by publishing logging information through \c printf function.
 *
 * For registering a class type for debugging use SEMF_DEBUG_ADD_CLASS(T, level).
 * For debugging only single objects, use SEMF_DEBUG_ADD_OBJECT(object, level).
 *
 * For printing information out, use \c SEMF_ERROR, \c SEMF_WARNING,
 * \c SEMF_INFO, \c SEMF_DEBUG and \c SEMF_TRACE macros.
 */
template <class T>
class Debug : public LinkedList<Debug<T>>::Node
{
public:
	/**
	 * @brief Constructor.
	 *
	 * Adds an object to the list to get printed out for debug.
	 *
	 * @param object Object to debug.
	 * @param level Maximum level to debug.
	 * @param name Name of the object.
	 */
	Debug(T& object, DebugLevel level, const char* name);

	/**
	 * @brief Activates debugging for all objects of the same type with a maximum level.
	 * @param maxLevel Maximum level of logs to be not ignored.
	 */
	static void debugClass(DebugLevel maxLevel);

	/**
	 * @brief Enables or disables the debugger output for a specific object.
	 * @param enable \c true for enable, \c false for disable.
	 */
	void setEnabled(bool enable = true);

	/**
	 * @brief Enables or disables the debugger output for a specific object.
	 * @param disable \c true for disable, \c false for enable.
	 */
	void setDisabled(bool disable = true);

	/**
	 * @brief Internal function - do not call directly.
	 *
	 * Use \c SEMF_ERROR, \c SEMF_WARNING, \c SEMF_INFO, \c SEMF_DEBUG and \c SEMF_TRACE macros.
	 *
	 * @param object Reference to the object from where the log is provided.
	 * @param level Level of log message.
	 * @param name Name of the class and function.
	 * @param format Printf string format.
	 * @param args Printf argument list.
	 */
	template <class ...Args>
	static void print(T& object, DebugLevel level, const char* name, const char* format, Args... args);

private:
	/**
	 * @brief Returns the logging level information as string.
	 * @param level Level to return as string.
	 * @return string.
	 */
	static const char* levelText(DebugLevel level);

	/**List of objects to debug*/
	static LinkedList<Debug<T>> m_list;
	/**Flag for all objcts of a class to be printed with global max level.*/
	static bool m_debugClass;
	/**Setting for all logs of a specific class, if enabled.*/
	static DebugLevel m_debugClassMaxLevel;

	/**Reference to object to debug.*/
	T& m_object;
	/**Debug level of obejct to debug.*/
	DebugLevel m_level;
	/**Name of object to debug.*/
	const char* m_name;
	/**Enable flag for one object.*/
	bool m_isEnabled = true;
};

template <class T>
LinkedList<Debug<T>> Debug<T>::m_list;

template <class T>
#ifdef SEMF_DEBUG_ALL
bool Debug<T>::m_debugClass = true;
#else
bool Debug<T>::m_debugClass = false;
#endif

template <class T>
DebugLevel Debug<T>::m_debugClassMaxLevel = DebugLevel::DebuglevelIgnore;

template <class T>
Debug<T>::Debug(T& object, DebugLevel level, const char* name)
	:m_object(object),
	 m_level(level),
	 m_name(name)
{
	m_list.pushFront(*this);
}

template <class T>
void Debug<T>::debugClass(DebugLevel maxLevel)
{
	m_debugClass = true;
	m_debugClassMaxLevel = maxLevel;
}

template <class T>
void Debug<T>::setEnabled(bool enable)
{
	m_isEnabled = enable;
}

template <class T>
void Debug<T>::setDisabled(bool disable)
{
	setEnabled(!disable);
}

template <class T>
template <class ...Args>
void Debug<T>::print(T& object, DebugLevel level, const char* name, const char* format, Args... args)
{
	// Returning for nothing to debug
	if (m_list.empty() && !m_debugClass)
		return;

	// Checking all registered objects
	for (auto &it : m_list)
	{
		if (&object == &it.m_object && level <= it.m_level && it.m_isEnabled)
		{
			printf("%s %s %s: ", name, it.m_name, levelText(level));
			printf(format, args...);
			printf("\n");
			return;
		}
	}

	// Checking whole class debug settings
	if (m_debugClass && level <= m_debugClassMaxLevel)
	{
		printf("%s %s: ", name, levelText(level));
		printf(format, args...);
		printf("\n");
	}
}

template <class T>
const char* Debug<T>::levelText(DebugLevel level)
{
	switch(level)
	{
	case DebugLevel::DebuglevelError:
		return "(e)";
	case DebugLevel::DebuglevelWarning:
		return "(w)";
	case DebugLevel::DebuglevelInfo:
		return "(i)";
	case DebugLevel::DebuglevelDebug:
		return "(d)";
	case DebugLevel::DebuglevelTrace:
	default:
		return "(t)";
	}
}

/**
 * @brief Passes a print message to the \c Debugger class.
 * @tparam T Class type.
 * @param object Pointer to the object to print a message from.
 * @param level Debug level of the message.
 * @param name Name of the class and function.
 * @param format Printf string.
 * @param args Printf argument list.
 */
template <class T, class ...Args>
void internalDebug(T* object, DebugLevel level, const char* name, const char* format, Args... args)
{
	Debug<T>::print(*object, level, name, format, args...);
}

/**
 * @brief Adds an object to the debug objects list by creating a \c Debugger object.
 * @tparam i Internal counter for creating single objects for every object.
 * @tparam T Class type.
 * @param object Pointer to the object to print a message from.
 * @param level Debug level of the message.
 * @param name Name of the class and function.
 */
template <int i, class T>
void debugObject(T& object, DebugLevel level, const char* name)
{
	static Debug<T> debugObj(object, level, name);
}

}  // namespace semf

#endif  /* CORE_DEBUG_H_ */
