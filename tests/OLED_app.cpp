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

extern "C" {
#include <GUI_Paint.h>
#include <GUI_BMPfile.h>
}
#include <OLED_0in91.h>

void  Handler(int signo)
{
    //System Exit
    printf("\nHandler:exit\n");
    DEV_ModuleExit();
    exit(0);
}

int OLED_0in91_test(void)
{
	OLED_0in91 oled;
	printf("0.91inch OLED test demo\n");
	if(DEV_ModuleInit() != 0) {
		return -1;
	}

	printf("OLED Init...\r\n");
	oled.initialise();
	DEV_Delay_ms(500);
	oled.clear();

	// 0.Create a new image cache
	UBYTE *BlackImage;
	UWORD Imagesize = ((OLED_0in91_WIDTH%8==0)? (OLED_0in91_WIDTH/8): (OLED_0in91_WIDTH/8+1)) * OLED_0in91_HEIGHT;
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory...\n");
			return -1;
	}
	printf("Paint_NewImage\n");
	Paint_NewImage(BlackImage, OLED_0in91_HEIGHT, OLED_0in91_WIDTH, 90, BLACK);

	printf("Drawing\n");
	//1.Select Image
	Paint_SelectImage(BlackImage);
	DEV_Delay_ms(500);
	Paint_Clear(BLACK);
	{
		// 2.Drawing on the image
		printf("Drawing:page 1\n");
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
		// 3.Show image on page1
		oled.display(BlackImage);
		DEV_Delay_ms(2000);
		Paint_Clear(BLACK);

		// Drawing on the image
		printf("Drawing:page 2\n");
		Paint_DrawString_EN(10, 0, "waveshare", &Font16, WHITE, WHITE);
		Paint_DrawNum(10, 18, 123.456789, &Font12, 4, WHITE, WHITE);
		// Show image on page2
		oled.display(BlackImage);
		DEV_Delay_ms(2000);
		Paint_Clear(BLACK);

		// Drawing on the image
		printf("Drawing:page 3\n");
		GUI_ReadBmp("../pic/0in91.bmp", 0, 0);
		// Show image on page4
		oled.display(BlackImage);
		DEV_Delay_ms(2000);
		Paint_Clear(BLACK);
		DEV_Delay_ms(2000);
	}
	oled.clear();
	return 0;
}

int main(int argc, char *argv[])
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    OLED_0in91_test();
    return 0;
}
