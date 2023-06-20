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

#ifndef OLED_0IN91_H_
#define OLED_0IN91_H_

#include <string>
#include <I2C.h>
extern "C"
{
#include "Fonts/fonts.h"
}

/* OLED width */
static const uint8_t OLED_0in91_WIDTH  = 128;
/* OLED height */
static const uint8_t OLED_0in91_HEIGHT =  32;
/* The size of an image that can be displayed */
static const uint16_t IMAGE_SIZE = OLED_0in91_WIDTH / 8 * OLED_0in91_HEIGHT;

class OLED_0in91
{
public:
	/**
	 * Class constructor, initialises all variables.
	 *  @param deviceId the ID of the OLED device on I2C bus.
	 */
	explicit OLED_0in91(const uint8_t deviceId = 0x3c);

	/**
	 * Class destructor, turns the OLED display off.
	 */
	virtual ~OLED_0in91();

	/**
	 * Initialises the OLED display.
	 *  @param device the address of the I2C device
	 */
	bool initialise(const std::string& device = "/dev/i2c-1");

	/**
	 * Clears the display by resetting it to black.
	 */
	void clear() const;

	/**
	 * Draws a text onto an image.
	 *  @param text the text to be drawn.
	 *  @param xStart the X position on the display where text should start.
	 *  @param yStart the Y position on the display where text should start.
	 *  @param font the font to use when drawing @p text.
	 *  @param[in/out] image an image onto which the @p text should be drawn.
	 */
	static void drawText(const std::string& text, const uint16_t xStart, const uint16_t yStart,
			const sFONT* font, uint8_t image[IMAGE_SIZE]);

	/**
	 * Draws a number onto an image.
	 *  @param xStart the X position on the display where the number should start.
	 *  @param yStart the Y position on the display where the number should start.
	 *  @param font the font to use when drawing the @p number.
	 *  @param[in/out] image an image onto which the @p number should be drawn.
	 */
	static void drawNumber(const double number, const uint16_t xStart, const uint16_t yStart,
			const sFONT* font, uint8_t image[IMAGE_SIZE]);

	/**
	 * Displays provided image.
	 *  @image an image to display.
	 */
	void display(const uint8_t image[IMAGE_SIZE]) const;

private:
	/** OLED device ID. */
	uint8_t mDeviceId;
	/** An interface for I2C communication. */
	I2C mI2C;
	/** A temporary buffer for sending data in a block rather than byte by byte. */
	mutable uint8_t mBuffer[OLED_0in91_WIDTH + 1];
};

#endif /* OLED_0IN91_H_ */
