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

#include <math.h>
#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <string.h>
#include <unistd.h>

extern "C" {
#include <GUI_Paint.h>
#include <GUI_BMPfile.h>
}
#include <OLED_0in91.h>

void populateImage1(uint8_t* image)
{
	Paint_SelectImage(image);
	puts("Drawing:page 1");
	Paint_DrawPoint(15, 10, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	Paint_DrawPoint(25, 10, WHITE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	Paint_DrawPoint(35, 10, WHITE, DOT_PIXEL_3X3, DOT_STYLE_DFT);
	Paint_DrawLine(10, 10, 10, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(20, 10, 20, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(30, 10, 30, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	Paint_DrawLine(40, 10, 40, 20, WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	Paint_DrawCircle(70, 16, 15, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawCircle(110, 16, 15, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	Paint_DrawRectangle(60, 6, 80, 26, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawRectangle(100, 6, 120, 26, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
}

void populateImage2(uint8_t* image)
{
	Paint_SelectImage(image);
	puts("Drawing:page 2");
	OLED_0in91::drawText("waveshare", 10, 0, &Font16, image);
	OLED_0in91::drawNumber(123.456789, 10, 18, &Font12, image);
}

void populateImage3(uint8_t* image)
{
	Paint_SelectImage(image);
	puts("Drawing:page 3");
	GUI_ReadBmp("../pic/0in91.bmp", 0, 0);
}

int main()
{
	OLED_0in91 oled;
	puts("0.91inch OLED test demo");
	if(oled.initialise())
	{
		puts("OLED Init...");
		usleep(500000);

		// 0.Create a new image cache
		uint8_t image1[IMAGE_SIZE];
		uint8_t image2[IMAGE_SIZE];
		uint8_t image3[IMAGE_SIZE];

		memset(image1, 0, IMAGE_SIZE);
		memset(image2, 0, IMAGE_SIZE);
		memset(image3, 0, IMAGE_SIZE);

		puts("Drawing images");
		populateImage1(image1);
		populateImage2(image2);
		populateImage3(image3);

		puts("Displaying:page 1");
		oled.display(image1);
		usleep(2000000);

		puts("Displaying:page 2");
		oled.display(image2);
		usleep(2000000);

		puts("Displaying:page 3");
		oled.display(image3);
		usleep(2000000);
	}

    return 0;
}
