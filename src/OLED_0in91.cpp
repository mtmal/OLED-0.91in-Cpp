////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2023 Mateusz Malinowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "OLED_0in91.h"
extern "C"
{
#include "GUI/GUI_Paint.h"
}

/**Display's commands address */
static const uint8_t IIC_CMD = 0X00;
/** Display's RAM address */
static const uint8_t IIC_RAM = 0X40;

OLED_0in91::OLED_0in91(const uint8_t deviceId) : mDeviceId(deviceId)
{
	mBuffer[0] = IIC_RAM;
	memset(mBuffer + 1, 0, OLED_0in91_WIDTH);
	// populate image properties
	Paint_NewImage(nullptr, OLED_0in91_HEIGHT, OLED_0in91_WIDTH, 90, BLACK);
}

OLED_0in91::~OLED_0in91()
{
	mI2C.writeByte(mDeviceId, IIC_CMD, 0xae); // turn display off
	mI2C.closeSerialPort();
}

bool OLED_0in91::initialise(const std::string& device)
{
	bool flag = mI2C.openSerialPort(device.c_str());

	if (flag)
	{
		//Set the initialization register
		mI2C.writeByte(mDeviceId, IIC_CMD, 0xAE);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0x40);	//---set low column address
		mI2C.writeByte(mDeviceId, IIC_CMD, 0xB0);	//---set high column address

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xC8);	//-not offset

		mI2C.writeByte(mDeviceId, IIC_CMD, 0x81);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0xff);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xa1);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xa6);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xa8);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0x1f);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xd3);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0x00);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xd5);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0xf0);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xd9);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0x22);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xda);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0x02);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0xdb);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0x49);

		mI2C.writeByte(mDeviceId, IIC_CMD, 0x8d);
		mI2C.writeByte(mDeviceId, IIC_CMD, 0x14);
	    usleep(200000);

		//Turn on the OLED display
		mI2C.writeByte(mDeviceId, IIC_CMD, 0xaf);
	}
	return flag;
}

void OLED_0in91::clear() const
{
	uint8_t page;
	memset(mBuffer + 1, 0, OLED_0in91_WIDTH);
    for (page = 0; page < OLED_0in91_HEIGHT/8; ++page)
    {
        mI2C.writeByte(mDeviceId, IIC_CMD, 0xb0 + page);	// Set page address
        mI2C.writeByte(mDeviceId, IIC_CMD, 0x00);			// Set display position - column low address
        mI2C.writeByte(mDeviceId, IIC_CMD, 0x10);			// Set display position - column high address
        mI2C.writeData(mDeviceId, OLED_0in91_WIDTH + 1, mBuffer);
    }
}

void OLED_0in91::drawText(const std::string& text, const uint16_t xStart, const uint16_t yStart, const sFONT* font,
		uint8_t image[IMAGE_SIZE])
{
	Paint_SelectImage(image);
	Paint_DrawString_EN(xStart, yStart, text.c_str(), const_cast<sFONT*>(font), WHITE, WHITE);
}

void OLED_0in91::drawNumber(const double number, const uint16_t xStart, const uint16_t yStart, const sFONT* font,
		uint8_t image[IMAGE_SIZE])
{
	Paint_SelectImage(image);
	Paint_DrawNum(xStart, yStart, number, const_cast<sFONT*>(font), 4, WHITE, WHITE);
}

void OLED_0in91::display(const uint8_t image[IMAGE_SIZE]) const
{
	uint8_t page;
	uint8_t column;
    for (page = 0; page < OLED_0in91_HEIGHT/8; ++page)
    {
        mI2C.writeByte(mDeviceId, IIC_CMD, 0xb0 + page);	// Set page address
        mI2C.writeByte(mDeviceId, IIC_CMD, 0x00);			// Set display position - column low address
        mI2C.writeByte(mDeviceId, IIC_CMD, 0x10);			// Set display position - column high address
        for (column = 0; column < OLED_0in91_WIDTH; ++column)
        {
        	mBuffer[1 + column] = image[(3 - page) + column * 4];
        }
        mI2C.writeData(mDeviceId, OLED_0in91_WIDTH + 1, mBuffer);
    }
}
