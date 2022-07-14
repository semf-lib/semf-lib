/**
 * @file stm32f4usbvcp.h
 *
 * @date 09.03.2020
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#ifndef HARDWAREABSTRACTION_STM32F4_STM32F4USBVCP_H_
#define HARDWAREABSTRACTION_STM32F4_STM32F4USBVCP_H_

#include <HardwareAbstraction/Stm32/stm32.h>

#if defined(STM32F4) && defined(HAL_PCD_MODULE_ENABLED)
#include <usbd_def.h>
#include <usbd_cdc.h>

/*
 *  Header files for the interface classes of Middleware.
 */
#include <Interfaces/uarthardware.h>

namespace semf
{
/**
 * @brief Class for using USB Virtual Communication Port (VCP) with Stm32F4.
 */
class Stm32F4UsbVcp: public UartHardware
{
public:
	/**
	 * @brief Error codes for this class. Error ID identify a unique error() / onError call (excluding transferring).
	 */
	enum class ErrorCode : uint8_t
	{
		Init_Stop = 0,
		Init_Deinit,
		Init_Init,
		Init_RegisterClass,
		Init_RegisterInterface,
		Init_Start
	};
	/**
	 * @brief Constructor.
	 * @param usbHandle The USB HAL handle from the library.
	 * @note It initializes/fills up automatically the static list \c cbList.
	 */
	explicit Stm32F4UsbVcp(USBD_HandleTypeDef& usbHandle, uint8_t* usbRxCache, size_t usbRxCacheSize);
	/**
	 * @copydoc UartHardware::init()
	 * @throws Init_Stop If \c USBD_Stop() fails.
	 * @throws Init_Deinit If \c USBD_DeInit() fails.
	 * @throws Init_Init If \c USBD_Init() fails.
	 * @throws Init_RegisterClass If \c USBD_RegisterClass() fails.
	 * @throws Init_RegisterInterface If \c USBD_CDC_RegisterInterface() fails.
	 * @throws Init_Start If \c USBD_Start() fails.
	 */
	void init();
	// not implemented
	void deinit();
	void stopRead();
	void stopWrite();
	void setFormat(uint8_t bits, WireMode wire, Parity par, StopBits stop, FlowControl flow);
	void setBaud(uint32_t baud);
private:
	/**
	 * @brief A structure of the list, that includes all the usb connected devices.
	 */
	struct CallbackList
	{
		/** Pointer to a current Stm32F4 member.*/
		Stm32F4UsbVcp* member = nullptr;
		/** Pointer to the next member in the CallbackList structure list.*/
		CallbackList* next = nullptr;
	};

	/** The head of the list. It is initialized in ".cpp" file.*/
	static CallbackList* m_callbackList;
	static uint8_t cdcInitCallback(USBD_HandleTypeDef *usb, uint8_t cfgidx);
	static uint8_t cdcDeInitCallback(USBD_HandleTypeDef *usb, uint8_t cfgidx);

	/**
	 * @brief Interrupt service routine for the sent data.
	 * @param usb The USB HAL handle from the library.
	 * @note This static function replaces the original static function from the middleware layer, which is done
	 * in the "init" function.
	 */
	static uint8_t cdcTxCallback(USBD_HandleTypeDef* usb, uint8_t epnum);

	/**
	 * @brief Interrupt service routine for the received data.
	 * @param usb The USB HAL handle from the library.
	 * @note This static function replaces the original static function from the middleware layer, which is done
	 * in the "init" function.
	 */
	static uint8_t cdcRxCallback(USBD_HandleTypeDef* usb, uint8_t epnum);
	static int8_t cdcControlCallback(uint8_t cmd, uint8_t pbuf[], uint16_t size);
	void cdcInit(USBD_HandleTypeDef* usb);

	/**
	 * @brief Interrupt service routine for the sent data.
	 * @param usb The USB HAL handle from the library.
	 * @note This function is called in the static callback function "usbTx".
	 */
	void cdcTx(USBD_HandleTypeDef* usb);

	/**
	 * @brief Interrupt service routine for the received data.
	 * @param usb The USB HAL handle from the library.
	 * @note This function is called in the static callback function "usbRx".
	 */
	void cdcRx(USBD_HandleTypeDef* usb, uint8_t epnum);

	/**
	 * @brief  Manage the CDC class requests
	 * @param  cmd Command code
	 * @param  pbuf Data containing command data (request parameters)
	 * @param  size Number of data to be sent (in bytes)
	 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
	 */
	void cdcControl(uint8_t cmd, uint8_t pbuf[], uint16_t size);
	void writeHardware(const uint8_t data[], size_t size);
	void readHardware(uint8_t data[], size_t size);
	/** Hardware handle*/
	USBD_HandleTypeDef* m_handle;
	/** Handle for the CDC Driver*/
	USBD_CDC_HandleTypeDef m_cdcHandle;
	/** This variable replaces the original "USBD_ClassTypeDef" instance (USBD_CDC) in the initialization process. */
	USBD_ClassTypeDef  m_interfaceCallbackClass;
	/** Member in the static list, it's head is \c cbList .*/
	CallbackList m_listMember;
	/**The current settings of the cdc interface*/
	USBD_CDC_LineCodingTypeDef m_lineCoding =
	{
	  115200, /* baud rate*/
	  0x00,   /* stop bits-1*/
	  0x00,   /* parity - none*/
	  0x08    /* nb. of bits 8*/
	};

	/** Cached the received data before copying it to the rxData */
	uint8_t* m_usbRxCache = nullptr;
	/** Size of the usb rx cache */
	size_t m_usbRxCacheSize = 0;
	/** Pointer to the last written entry in the cache */
	size_t m_usbRxCacheWritePos = 0;
	/** Pointer to the last already copied entry in the cache*/
	size_t m_usbRxCacheReadPos = 0;
	/** Pointer to the rx_data from the higher level*/
	uint8_t* m_rxData = nullptr;
	/** The size of the rxData */
	size_t m_rxDataSize = 0;
	/** Number of bytes already read.*/
	size_t m_rxDataAvailable = 0;
	/**Class ID for error tracing.*/
	static constexpr Error::ClassID kSemfClassId = Error::ClassID::Stm32F4UsbVcp;
};

} /* namespace semf */

#endif
#endif /* HARDWAREABSTRACTION_STM32F4_STM32F4USBVCP_H_ */
