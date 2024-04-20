/**
 * @file stm32f4usbvcp.cpp
 * @date 09.03.2020
 * @author aa
 * @copyright Copyright (C) querdenker engineering GmbH - All Rights Reserved
 *            For detailed information, read the license file in
 *            the project root directory.
 */

#include <semf/hardwareabstraction/stm32f4/stm32f4usbvcp.h>

#if defined(STM32F4) && defined(HAL_PCD_MODULE_ENABLED)
#include <usbd_desc.h>
#include <usbd_cdc_if.h>
#include <algorithm>
namespace semf
{
Stm32F4UsbVcp::CallbackList* Stm32F4UsbVcp::m_callbackList = nullptr;
Stm32F4UsbVcp::Stm32F4UsbVcp(USBD_HandleTypeDef& usbHandle, uint8_t usbRxCache[], size_t usbRxCacheSize):
		m_handle(&usbHandle), m_usbRxCache(usbRxCache), m_usbRxCacheSize(usbRxCacheSize)
{
	m_listMember.member = this;
	m_listMember.next = nullptr;

	if (m_callbackList == nullptr)
	{
		m_callbackList =&m_listMember;
	}
	else
	{
		CallbackList* curr = m_callbackList;
		while (curr->next !=nullptr)
		{
			curr = curr->next;
		}
		curr->next = &m_listMember;
	}
}

void Stm32F4UsbVcp::init()
{
	m_interfaceCallbackClass = USBD_CDC;

	// replace some pointers
	m_interfaceCallbackClass.Init = cdcInitCallback;
	m_interfaceCallbackClass.DeInit = cdcDeInitCallback;
	m_interfaceCallbackClass.DataIn = cdcTxCallback;
	m_interfaceCallbackClass.DataOut = cdcRxCallback;
	reinterpret_cast<USBD_CDC_ItfTypeDef *>(m_handle->pUserData)->Control = cdcControlCallback;

	// stop if it is started to register the correct interfaces, it is recommended not to initiate it in the main.c
	if (USBD_Stop(m_handle) != USBD_OK)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_Stop)));
	}
	if (USBD_DeInit(m_handle) != USBD_OK)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_Deinit)));
	}
	if (USBD_Init(m_handle, &FS_Desc, DEVICE_FS) != USBD_OK)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_Init)));
	}
	if (USBD_RegisterClass(m_handle, &m_interfaceCallbackClass) != USBD_OK)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_RegisterClass)));
	}
	if (USBD_CDC_RegisterInterface(m_handle, &USBD_Interface_fops_FS) != USBD_OK)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_RegisterInterface)));
	}
	if (USBD_Start(m_handle) != USBD_OK)
	{
		onError(Error(kSemfClassId, static_cast<uint8_t>(ErrorCode::Init_Start)));
	}
}

void Stm32F4UsbVcp::deinit()
{
	// TODO(AA) not implemented
	return;
}
void Stm32F4UsbVcp::stopRead()
{
	// TODO(AA) not implemented
	return;
}
void Stm32F4UsbVcp::stopWrite()
{
	// TODO(AA) not implemented
	return;
}

void Stm32F4UsbVcp::setFormat(uint8_t bits, WireMode wire, Parity par, StopBits stop, FlowControl flow)
{
	// TODO(AA) not implemented
	return;
}

void Stm32F4UsbVcp::setBaud(uint32_t baud)
{
	// TODO(AA) not implemented
	return;
}

uint8_t Stm32F4UsbVcp::cdcInitCallback(USBD_HandleTypeDef* usb, uint8_t cfgidx)
{
	uint8_t ret = 0U;
	USBD_CDC_HandleTypeDef   *hcdc;

	if (usb->dev_speed == USBD_SPEED_HIGH)
	{
		/* Open EP IN */
		USBD_LL_OpenEP(usb, CDC_IN_EP, USBD_EP_TYPE_BULK,
				CDC_DATA_HS_IN_PACKET_SIZE);

		usb->ep_in[CDC_IN_EP & 0xFU].is_used = 1U;

		/* Open EP OUT */
		USBD_LL_OpenEP(usb, CDC_OUT_EP, USBD_EP_TYPE_BULK,
				CDC_DATA_HS_OUT_PACKET_SIZE);

		usb->ep_out[CDC_OUT_EP & 0xFU].is_used = 1U;
	}
	else
	{
		/* Open EP IN */
		USBD_LL_OpenEP(usb, CDC_IN_EP, USBD_EP_TYPE_BULK,
				CDC_DATA_FS_IN_PACKET_SIZE);

		usb->ep_in[CDC_IN_EP & 0xFU].is_used = 1U;

		/* Open EP OUT */
		USBD_LL_OpenEP(usb, CDC_OUT_EP, USBD_EP_TYPE_BULK,
				CDC_DATA_FS_OUT_PACKET_SIZE);

		usb->ep_out[CDC_OUT_EP & 0xFU].is_used = 1U;
	}
	/* Open Command IN EP */
	USBD_LL_OpenEP(usb, CDC_CMD_EP, USBD_EP_TYPE_INTR, CDC_CMD_PACKET_SIZE);
	usb->ep_in[CDC_CMD_EP & 0xFU].is_used = 1U;

	for(auto i = m_callbackList; i != nullptr; i = i->next)
	{
		if (i->member->m_handle == usb)
		{
			usb->pClassData = reinterpret_cast<void*>(&i->member->m_cdcHandle);
			USBD_CDC_SetRxBuffer(i->member->m_handle, i->member->m_usbRxCache);
			break;
		}
	}

	if (usb->pClassData == NULL)
	{
		ret = 1U;
	}
	else
	{
		hcdc = reinterpret_cast<USBD_CDC_HandleTypeDef*>(usb->pClassData);

		/* Init  physical Interface components */
		reinterpret_cast<USBD_CDC_ItfTypeDef *>(usb->pUserData)->Init();

		CallbackList* curr = m_callbackList;
		while(curr != nullptr)
		{
			curr->member->cdcInit(usb);
			curr = curr->next;
		}

		/* Init Xfer states */
		hcdc->TxState = 0U;
		hcdc->RxState = 0U;

		if (usb->dev_speed == USBD_SPEED_HIGH)
		{
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(usb, CDC_OUT_EP, hcdc->RxBuffer,
					CDC_DATA_HS_OUT_PACKET_SIZE);
		}
		else
		{
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(usb, CDC_OUT_EP, hcdc->RxBuffer,
					CDC_DATA_FS_OUT_PACKET_SIZE);
		}
	}
	return ret;
}

uint8_t Stm32F4UsbVcp::cdcDeInitCallback(USBD_HandleTypeDef *usb, uint8_t cfgidx)
{
	/* Close EP IN */
	USBD_LL_CloseEP(usb, CDC_IN_EP);
	usb->ep_in[CDC_IN_EP & 0xFU].is_used = 0U;

	/* Close EP OUT */
	USBD_LL_CloseEP(usb, CDC_OUT_EP);
	usb->ep_out[CDC_OUT_EP & 0xFU].is_used = 0U;

	/* Close Command IN EP */
	USBD_LL_CloseEP(usb, CDC_CMD_EP);
	usb->ep_in[CDC_CMD_EP & 0xFU].is_used = 0U;

	/* DeInit  physical Interface components */
	if(usb->pClassData != NULL)
	{
		(reinterpret_cast<USBD_CDC_ItfTypeDef*>(usb->pUserData))->DeInit();
		usb->pClassData = NULL;
	}

	return 0;
}

uint8_t Stm32F4UsbVcp::cdcTxCallback(USBD_HandleTypeDef* usb, uint8_t epnum)
{
	uint8_t retVal = USBD_CDC.DataIn(usb, epnum);
	if ( 	(retVal == USBD_OK) &&
			((reinterpret_cast<USBD_CDC_HandleTypeDef*>(usb->pClassData))->TxState == 0U)
	)
	{
		CallbackList* curr = m_callbackList;
		while(curr != nullptr)
		{
			curr->member->cdcTx(usb);
			curr = curr->next;
		}
	}
	return retVal;
}

uint8_t Stm32F4UsbVcp::cdcRxCallback(USBD_HandleTypeDef* usb, uint8_t epnum)
{
	CallbackList* curr = m_callbackList;
	while(curr != nullptr)
	{
		curr->member->cdcRx(usb, epnum);

		curr = curr->next;
	}
	return USBD_OK;
}

int8_t Stm32F4UsbVcp::cdcControlCallback(uint8_t cmd, uint8_t pbuf[], uint16_t size)
{
	CallbackList* curr = m_callbackList;
	while(curr != nullptr)
	{
		curr->member->cdcControl(cmd, pbuf, size);
		curr = curr->next;
	}
	return (USBD_OK);
}

void Stm32F4UsbVcp::cdcInit(USBD_HandleTypeDef* usb)
{
	if (m_handle == usb)
	{
		m_usbRxCacheWritePos = 0;
		m_usbRxCacheReadPos = 0;
	}
}

void Stm32F4UsbVcp::cdcTx(USBD_HandleTypeDef* usb)
{
	if (m_handle == usb)
	{
		onDataWritten();
	}
}

void Stm32F4UsbVcp::cdcRx(USBD_HandleTypeDef* usb, uint8_t epnum)
{
	if (m_handle == usb)
	{
		size_t numOfRxData = USBD_LL_GetRxDataSize(usb, epnum);
		if (numOfRxData > 0)
		{
			memcpy(&m_rxData[m_rxDataAvailable], &m_usbRxCache[m_usbRxCacheReadPos],
					std::min(numOfRxData, m_rxDataSize - m_rxDataAvailable));
			m_usbRxCacheReadPos += std::min(numOfRxData, m_rxDataSize - m_rxDataAvailable);
			m_usbRxCacheWritePos += numOfRxData;
			m_rxDataAvailable += std::min(numOfRxData, m_rxDataSize);
			if (m_rxDataAvailable == m_rxDataSize)
				onDataAvailable();
		}

		/** prevents overflow of rx cache */
		if(usb->dev_speed == USBD_SPEED_HIGH)
		{
			if (m_usbRxCacheSize < (m_usbRxCacheWritePos + CDC_DATA_HS_OUT_PACKET_SIZE))
			{
				m_usbRxCacheWritePos = 0;
				m_usbRxCacheReadPos = 0;
			}
		}
		else
		{
			if (m_usbRxCacheSize < (m_usbRxCacheWritePos + CDC_DATA_FS_OUT_PACKET_SIZE))
			{
				m_usbRxCacheWritePos = 0;
				m_usbRxCacheReadPos = 0;
			}
		}

		USBD_CDC_SetRxBuffer(m_handle, &m_usbRxCache[m_usbRxCacheWritePos]);
		USBD_CDC_ReceivePacket(usb);
	}
}

void Stm32F4UsbVcp::cdcControl(uint8_t cmd, uint8_t pbuf[], uint16_t size)
{
	switch (cmd)
	{
	case CDC_SEND_ENCAPSULATED_COMMAND:
		break;
	case CDC_GET_ENCAPSULATED_RESPONSE:
		break;
	case CDC_SET_COMM_FEATURE:
		break;
	case CDC_GET_COMM_FEATURE:
		break;
	case CDC_CLEAR_COMM_FEATURE:
		break;
	case CDC_SET_LINE_CODING:
		m_lineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
				(pbuf[2] << 16) | (pbuf[3] << 24));
		m_lineCoding.format     = pbuf[4];
		m_lineCoding.paritytype = pbuf[5];
		m_lineCoding.datatype   = pbuf[6];
		break;
	case CDC_GET_LINE_CODING:
		pbuf[0] = (uint8_t)(m_lineCoding.bitrate);
		pbuf[1] = (uint8_t)(m_lineCoding.bitrate >> 8);
		pbuf[2] = (uint8_t)(m_lineCoding.bitrate >> 16);
		pbuf[3] = (uint8_t)(m_lineCoding.bitrate >> 24);
		pbuf[4] = m_lineCoding.format;
		pbuf[5] = m_lineCoding.paritytype;
		pbuf[6] = m_lineCoding.datatype;
		break;
	case CDC_SET_CONTROL_LINE_STATE:
		break;
	case CDC_SEND_BREAK:
		break;
	default:
		break;
	}
}

void Stm32F4UsbVcp::writeHardware(const uint8_t data[], size_t size)
{
	// check if usb is connected
	if (m_handle->dev_state == USBD_STATE_CONFIGURED)
	{
		if (CDC_Transmit_FS(const_cast<uint8_t*>(data), size) != USBD_OK)
		{
			// must called, while this is a uart simulation, and a uart has no connection state
			onDataWritten();
		}
	}
	else
	{
		// must called, while this is a uart simulation, and a uart has no connection state
		onDataWritten();
	}
}

void Stm32F4UsbVcp::readHardware(uint8_t data[], size_t size)
{
	m_rxData = data;
	m_rxDataSize = size;
	m_rxDataAvailable = 0;

	if (m_usbRxCacheWritePos > m_usbRxCacheReadPos)
	{
		memcpy(m_rxData, &m_usbRxCache[m_usbRxCacheReadPos],
				std::min(m_usbRxCacheWritePos - m_usbRxCacheReadPos, m_rxDataSize));
		m_rxDataAvailable = std::min(m_usbRxCacheWritePos - m_usbRxCacheReadPos, m_rxDataSize);
		m_usbRxCacheReadPos += std::min(m_usbRxCacheWritePos - m_usbRxCacheReadPos, m_rxDataSize);

		if (m_rxDataAvailable == m_rxDataSize)
			onDataAvailable();
	}

	if (m_usbRxCacheWritePos <= m_usbRxCacheReadPos)
	{
		m_usbRxCacheWritePos = 0;
		m_usbRxCacheReadPos = 0;
		USBD_CDC_SetRxBuffer(m_handle, m_usbRxCache);
	}
}
} /* namespace semf */
#endif
