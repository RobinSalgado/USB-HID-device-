/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "composite.h"

#include "hid_keyboard.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static usb_status_t USB_DeviceHidKeyboardAction(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static usb_device_composite_struct_t *s_UsbDeviceComposite;
static usb_device_hid_keyboard_struct_t s_UsbDeviceHidKeyboard;

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_KeyboardBuffer[USB_HID_KEYBOARD_REPORT_LENGTH];
extern EventGroupHandle_t Events_Hanlder; /**Handler for keyboard and mouse*/
extern BaseType_t *Mouse_priority;
extern BaseType_t *KeyBoard_priority;

static usb_status_t USB_DeviceHidKeyboardAction(void) {
	static int x = 0U;

	enum {
		Ejecutar,
		PaintText,
		B,
		Exclamation,
		EnterForPaint,
		H,
		O,
		L,
		A,
		M,
		U,
		N,
		D,
		O2,
		Windows,
		Enter,
		Copy,
		paste,
		Space,
		Text,
		Windows2,
		B2,
		Enter2,
		ventanaIzq,
		Finish

	};

	static uint8_t dir = Ejecutar;
	s_UsbDeviceHidKeyboard.buffer[2] = 0x00U;
	s_UsbDeviceHidKeyboard.buffer[3] = 0x00U;
	s_UsbDeviceHidKeyboard.buffer[4] = 0x00U;
	s_UsbDeviceHidKeyboard.buffer[5] = 0x00U;
	s_UsbDeviceHidKeyboard.buffer[6] = 0x00U;

	uint8_t TextCounter = 0U;
	uint8_t select1 = 0;
	switch (dir) {
	case Ejecutar:
		x++;
		if (x > 50U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_RIGHT_GUI;
			s_UsbDeviceHidKeyboard.buffer[3] = KEY_R;
			x = 0;
			dir = PaintText;
		}
		break;

	case PaintText:
		x++;
		if (x > 50U) {
			if (x > 60) {
				s_UsbDeviceHidKeyboard.buffer[2] = KEY_M;
				if (x > 120) {
					s_UsbDeviceHidKeyboard.buffer[3] = KEY_S;
					if (x > 180) {
						s_UsbDeviceHidKeyboard.buffer[4] = KEY_P;
						if (x > 140) {
							s_UsbDeviceHidKeyboard.buffer[5] = KEY_A;
							if (x > 200) {
								s_UsbDeviceHidKeyboard.buffer[6] = KEY_I;
								if (x > 260) {
									s_UsbDeviceHidKeyboard.buffer[7] = KEY_N;
									if (x > 270) {
										s_UsbDeviceHidKeyboard.buffer[7] = 0U;

										x = 0;
										dir = EnterForPaint;
									}
								}
							}
						}
					}
				}
			}
		}

		break;

	case EnterForPaint:
		x++;
		if (x > 100) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_T;
			s_UsbDeviceHidKeyboard.buffer[3] = KEY_ENTER;

			if (x > 200U) {

				dir = Windows;
			}
		}
		break;
	case Windows:
		x++;
		if (x > 1500U) {
			s_UsbDeviceHidKeyboard.buffer[0] = MODIFERKEYS_LEFT_CTRL;
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_ESCAPE;

			x = 0;
			dir = B;
		}
		break;

	case B:
		x++;
		if (x > 300U) {
			s_UsbDeviceHidKeyboard.buffer[0] = 0x00U;
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_B;
			x = 0;
			dir = Enter;
		}

		break;

	case H:
		x++;
		if (x > 300U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_H;
			x = 0;
			dir = O;
		}
		break;

	case O:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_O;
			if (select1 == 0) {
				dir = L;
				select1=1;
			} else if (select1 == 1) {
				select1 = 0;
				dir = Exclamation;
			}
		}

		break;

	case L:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_L;
			dir = A;
		}
		break;

	case A:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_A;
			dir = Space;
		}
		break;

	case Enter:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_ENTER;
			dir = H;

		}
		break;

	case ventanaIzq:
		x++;

		if (x > 30) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_RIGHT_GUI;
			s_UsbDeviceHidKeyboard.buffer[3] = KEY_R;
//                       s_UsbDeviceHidKeyboard.buffer[3] = KEY_LEFTARROW;
			dir = H;
		}

		break;

	case Space:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_SPACEBAR;
			dir = M;
		}
		break;

	case M:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_M;
			dir = U;
		}
		break;

	case U:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_U;
			dir = N;
		}
		break;

	case N:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_N;
			dir = D;
		}
		break;

	case D:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_D;
			select1 = 1;
			dir = O2;
		}
		break;
	case O2:
			x++;
			if (x > 30U) {
				s_UsbDeviceHidKeyboard.buffer[2] = KEY_O;
				dir = Text;
			}
			break;
	case Exclamation:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_KEYPAD_EXCLAMATION_MARK;
			x = 0;
			dir = Text;
		}
		break;

	case Text:
		x++;
		if (x > 30U) {
			if (TextCounter == 0) {
				s_UsbDeviceHidKeyboard.buffer[0] = MODIFERKEYS_RIGHT_SHIFT;
			}
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTARROW;
			if (x == 300) {
				dir = Copy;
				x = 0;
			}
		}
		break;

	case Copy:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[0] = MODIFERKEYS_LEFT_CTRL;
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_C;
			dir = Windows2;
		}
		break;

	case paste:
		x++;
		if (x > 70U) {
			s_UsbDeviceHidKeyboard.buffer[0] = MODIFERKEYS_LEFT_CTRL;
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_V;
			x=0;
     dir = 50;
		}
		break;

	case Finish:
			x++;
			if (x > 30U) {
				s_UsbDeviceHidKeyboard.buffer[2] = KEY_RIGHT_GUI;
				s_UsbDeviceHidKeyboard.buffer[3] = KEY_LEFTARROW;
				x=0;
	     dir = 50;
			}
			break;
	case Windows2:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[0] = MODIFERKEYS_LEFT_CTRL;
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_ESCAPE;
			x = 0;
			dir = B2;
		}
		break;
	case B2:
		x++;
		if (x > 50U) {
			s_UsbDeviceHidKeyboard.buffer[0] = 0x00U;
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_B;
			x = 0;
			dir = Enter2;
		}

		break;
	case Enter2:
		x++;
		if (x > 30U) {
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_ENTER;
			x=0;
			dir = paste;
		}
		break;
	default:
		s_UsbDeviceHidKeyboard.buffer[0] = 0x00U;
		s_UsbDeviceHidKeyboard.buffer[2] = 0x00U;
		break;
	}
	return USB_DeviceHidSend(s_UsbDeviceComposite->hidKeyboardHandle,
			USB_HID_KEYBOARD_ENDPOINT_IN, s_UsbDeviceHidKeyboard.buffer,
			USB_HID_KEYBOARD_REPORT_LENGTH);
}

usb_status_t USB_DeviceHidKeyboardCallback(class_handle_t handle,
		uint32_t event, void *param) {
	usb_status_t error = kStatus_USB_Error;

	switch (event) {
	case kUSB_DeviceHidEventSendResponse:
		if (s_UsbDeviceComposite->attach) {
			return USB_DeviceHidKeyboardAction();
		}
		break;
	case kUSB_DeviceHidEventGetReport:
	case kUSB_DeviceHidEventSetReport:
	case kUSB_DeviceHidEventRequestReportBuffer:
		error = kStatus_USB_InvalidRequest;
		break;
	case kUSB_DeviceHidEventGetIdle:
	case kUSB_DeviceHidEventGetProtocol:
	case kUSB_DeviceHidEventSetIdle:
	case kUSB_DeviceHidEventSetProtocol:
		break;
	default:
		break;
	}

	return error;
}

usb_status_t USB_DeviceHidKeyboardSetConfigure(class_handle_t handle,
		uint8_t configure) {
	if (USB_COMPOSITE_CONFIGURE_INDEX == configure) {
		return USB_DeviceHidKeyboardAction(); /* run the cursor movement code */
	}
	return kStatus_USB_Error;
}

usb_status_t USB_DeviceHidKeyboardSetInterface(class_handle_t handle,
		uint8_t interface, uint8_t alternateSetting) {
	if (USB_HID_KEYBOARD_INTERFACE_INDEX == interface) {
		return USB_DeviceHidKeyboardAction(); /* run the cursor movement code */
	}
	return kStatus_USB_Error;
}

usb_status_t USB_DeviceHidKeyboardInit(
		usb_device_composite_struct_t *deviceComposite) {
	s_UsbDeviceComposite = deviceComposite;
	s_UsbDeviceHidKeyboard.buffer = s_KeyboardBuffer;
	return kStatus_USB_Success;
}
