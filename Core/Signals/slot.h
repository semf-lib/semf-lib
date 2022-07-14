/**
 * @file slot.h
 * 
 * @date 08.09.2021
 * @author jh
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory
 */

#ifndef CORE_SIGNALS_SLOT_H_
#define CORE_SIGNALS_SLOT_H_

#include <utility>

namespace semf
{
/**
 * @brief Handles the invocation of the connected callback. A Slot can only handle a single callback.
 * @tparam Arguments parameter types of the connected callback.
 */
template<typename... Arguments>
class Slot
{
public:
	/**
	 * @brief Constructs an empty Slot.
	 */
	Slot() = default;
	/**
	 * @brief Constructs a Slot connected to \c object->function(args...).
	 * @tparam X Object type.
	 * @tparam Y Member type.
	 * @param object Object for calling the member function.
	 * @param function Member function to call.
	 */
	template<typename X, typename Y>
	Slot(Y* object, void (X::* function)(Arguments...));
	/**
	 * @brief Constructs a Slot connected to \c object->function(args...).
	 * @tparam X Member type.
	 * @tparam Y Object type.
	 * @param object Object for calling the member function.
	 * @param function Member function to call.
	 */
	template<typename X, typename Y>
	Slot(Y* object, void (X::* function)(Arguments...) const);
	/**
	 * @brief Constructs a Slot connected to \c function.
	 * @param function 
	 */
	explicit Slot(void (*function)(Arguments...));
	/**
	 * @brief Triggers the invocation of the connected callback. If the slot is empty nothing happens.
	 * @tparam EmitArgument Argument types.
	 * @param arguments Arguments for the connected callback.
	 */
	template<typename... EmitArgument>
	void emitSlot(EmitArgument&&... arguments) const;
	/**
	 * @brief Checks whether the slot is connected to a callback or not.
	 * @return \c true if connected, \c false otherwise.
	 */
	bool connected() const;
	/**
	 * @brief Removes an existing connection from the slot.
	 */
	void disconnect();
	/**
	 * @brief Compare operator for checking whether \c other is connected to the same callback.
	 * @param other Other slot.
	 * @return \c true if equal, false otherwise.
	 */
	bool operator==(Slot<Arguments...>& other);
private:
#ifdef _MSC_VER
	class __virtual_inheritance GenericClass;
#else
	class GenericClass;
#endif
	/**
	 * @brief Magical cast function. It can cast anything by using an enum.
	 * @tparam Out Output type.
	 * @tparam In Input type.
	 * @param in Input value.
	 * @return Output value.
	 */
	template<typename Out, typename In>
	static Out magic_cast(In in);
	/**
	 * @brief Converts \c object and \c function to their real type and calls \c function.
	 * @tparam X Member type.
	 * @tparam Y Object type.
	 * @param object Object used for calling \c function.
	 * @param function Callback function.
	 * @param arguments Arguments of \c function.
	 */
	template<typename X, typename Y>
	static void memberFunctionCaller(GenericClass* object, void (GenericClass::* function)(Arguments...), Arguments... arguments);
	/**
	 * @brief Union for performing the \c magic_cast.
	 * @tparam In Input type.
	 * @tparam Out Output type.
	 */
	template<typename In, typename Out>
	union MagicUnion
	{
		/**Input value.*/
		In in;
		/**Output value.*/
		Out out;
	};
	/**Object used for calling a member function.*/
	GenericClass* m_object = nullptr;
	/**Member function to call.*/
	void (GenericClass::* m_memberFunction)(Arguments...) = nullptr;
	/**Union for storing the invocation function. C-Style and static functions are called directly.*/
	union Callback
	{
		/**
		 * @brief Construct a Callback pointing to \c nullptr.
		 */
		Callback();
		/**
		 * @brief Constructs a Callback pointing to \c staticFunc.
		 * @param staticFunction C-Style/static function callback.
		 */
		Callback(void (*staticFunction)(Arguments...));
		/**
		 * @brief Constructs a Callback pointing to \c member.
		 * @param member Member invoking function callback.
		 */
		Callback(void (*member)(GenericClass*, void (GenericClass::*)(Arguments...), Arguments...));
		/**C-Style/static function callback.*/
		void (*staticFunction)(Arguments...);
		/**Member invoking function callback.*/
		void (*memberFunctionCaller)(GenericClass*, void (GenericClass::*)(Arguments...), Arguments...);
	};
	/**Stores the invocation function for invoking the callback.*/
	Callback m_caller;
};

template<typename... Arguments>
template<typename X, typename Y>
Slot<Arguments...>::Slot(Y* object, void (X::* function)(Arguments...))
: m_object{reinterpret_cast<GenericClass*>(object)},
  m_memberFunction{magic_cast<void (GenericClass::*)(Arguments...)>(function)},
  m_caller(memberFunctionCaller<X, Y>)
{
	static_assert(sizeof(function) <= sizeof(m_memberFunction), "The Slot class is not compatible with your compiler!");
}

template<typename... Arguments>
template<typename X, typename Y>
Slot<Arguments...>::Slot(Y* object, void (X::* function)(Arguments...) const)
: m_object{reinterpret_cast<GenericClass*>(object)},
  m_memberFunction{magic_cast<void (GenericClass::*)(Arguments...)>(function)},
  m_caller(memberFunctionCaller<X, Y>)
{
	static_assert(sizeof(function) <= sizeof(m_memberFunction), "The Slot class is not compatible with your compiler!");
}

template<typename... Arguments>
Slot<Arguments...>::Slot(void(*function)(Arguments...))
: m_object{nullptr},
  m_memberFunction{nullptr},
  m_caller(function)
{}

template<typename... Arguments>
template<typename... EmitArgument>
void Slot<Arguments...>::emitSlot(EmitArgument&&... arguments) const
{
	if (!connected())
		return;

	if (m_object != nullptr && m_memberFunction != nullptr)
		m_caller.memberFunctionCaller(m_object, m_memberFunction, std::forward<EmitArgument>(arguments)...);
	else
		m_caller.staticFunction(std::forward<EmitArgument>(arguments)...);
}

template<typename... Arguments>
bool Slot<Arguments...>::connected() const
{
	return m_caller.staticFunction != nullptr;
}

template<typename... Arguments>
void Slot<Arguments...>::disconnect()
{
	m_caller.staticFunction = nullptr;
}

template<typename... Arguments>
bool Slot<Arguments...>::operator==(Slot<Arguments...>& other)
{
	return m_object == other.m_object &&
	       m_memberFunction == other.m_memberFunction &&
	       m_caller.staticFunction == other.m_caller.staticFunction;
}

template<typename... Arguments>
template<typename Out, typename In>
Out Slot<Arguments...>::magic_cast(In in)
{
	MagicUnion<In, Out> wizard = {in};
	return wizard.out;
}

template<typename... Arguments>
template<typename X, typename Y>
void Slot<Arguments...>::memberFunctionCaller(GenericClass* object, void (GenericClass::* function)(Arguments...), Arguments... arguments)
{
	(reinterpret_cast<Y*>(object)->*(magic_cast<void(X::*)(Arguments...)>(function)))(std::forward<Arguments>(arguments)...);
}

template<typename... Arguments>
Slot<Arguments...>::Callback::Callback()
: staticFunction{nullptr}
{}

template<typename... Arguments>
Slot<Arguments...>::Callback::Callback(void (*staticFunction)(Arguments...))
: staticFunction{staticFunction}
{}

template<typename... Arguments>
Slot<Arguments...>::Callback::Callback(void (*member)(GenericClass*, void (GenericClass::*)(Arguments...), Arguments...))
: memberFunctionCaller{member}
{}
}  // namespace semf
#endif  // CORE_SIGNALS_SLOT_H_
