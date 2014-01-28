/*
 * vga.c
 *
 *	Created: 2014-01-18
 *		Author: Brittaney Geisler, Isaac Cheng, Kelvin Au, Kevin Hui
 */

#include "screen.h"

alt_up_pixel_buffer_dma_dev *pixel_buffer;
int i = 0;

//initialises the screen buffer
void initScreen() {
	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + PIXEL_BUFFER_SPAN / 2;// Set the 1st buffer address

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(PIXEL_BUFFER_DMA_NAME);

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr2);

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
}

//clears the screen
void clearScreen() {
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);
}

//swaps the background buffer to the foreground
void updateScreen() {
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	//printf("is it white?"); 	for (count = 0; count < wait; ++count) {	}
}

//TODO: Print the whole field without lag
//prints the field to the screen
void updateField() {
	// Draw a rectangle for the field
	//alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, 0, 160, SCREEN_WIDTH
	//	- 1, SCREEN_HEIGHT - 1, 0xFFFF, 1);
	int i;
	int j;// = SCREEN_HEIGHT*7/10+1; //TEMPORARY ONLY PRINT ONE LINE OF FIELD
	for (j = SCREEN_HEIGHT * 7 / 10; j < SCREEN_HEIGHT * 7 / 10 + 2; j++) {
		for (i = 0; i < SCREEN_WIDTH; i++) {
			if (field[j][i] == GROUND)
				alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, i, j);
		}
	}

}

//prints the prints a player to the screen
void updatePlayer(int x, int y, int deg, int colour) {
	// Draw a rectangle for the field
	alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer, x, y, x + TANK_LENGTH,
			y + TANK_HEIGHT, colour, 1);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x + TANK_LENGTH/2, y,
			x + TANK_LENGTH / 2 + getTurretWidth(deg),
			y - getTurretHeight(deg), colour,
			1);
}

//prints a bullet to the screen
//TODO: make the bullet not one pixel
void updateBullet(int x, int y) {
	alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, x, y);
}

//prints lines
void printLines() {
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(PIXEL_BUFFER_DMA_NAME);
	// Set the background buffer address � Although we don�t use the
	//background, they only provide a function to change the background
	//buffer address, so
	// we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			PIXEL_BUFFER_BASE);
	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	// Wait for the swap to complete while
	(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	// Draw a white line to the foreground buffer
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF, 0);
}

//Prints string
void printString() {
	// Initialize
	alt_up_char_buffer_dev *char_buffer;
	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	// Write some text
	alt_up_char_buffer_string(char_buffer, "EECE 381", 40, 30);
}
