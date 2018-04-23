//Aliyah Flores
//aff13

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#pragma pack(1)

struct BHeader { //create bmp header
	char format_identifier[2];
	int file_size;
	short reserved_value;
	short reserved_value_copy;
	int offset;
} BHeader;

struct DHeader { //create dib header
	int dib_size;
	int image_width;
	int image_height;
	short num_color_panes;
	short bits_per_pixel;
	int compression;
	int image_size;
	int horiz_res;
	int vert_res;
	int num_color_palettes;
	int num_import_colors;
} DHeader;

struct Pixel { //create pixel structure
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

int check_errors(FILE* file) { // check for errors with file
	if((BHeader.format_identifier[0] != 'B' && BHeader.format_identifier[1] != 'M') || DHeader.dib_size != 40 || DHeader.bits_per_pixel != 24){
		if(DHeader.dib_size != 40) {
			printf("Size of DIB is not 40!\n");
		}
		if(DHeader.bits_per_pixel != 24) {
			printf("Bits per pixel is not 24!");
		}
		if((BHeader.format_identifier[0] == 'B' && BHeader.format_identifier[1] == 'M') != 0) {
			printf("The identifier is not BM.");
		} else {
			printf("File format not supported.\n");
		
		}
	}
	return 1;
}

int main(int argc, char* argv[]){//main function
	FILE* file = fopen(argv[2], "rb+");
	int i, j;

	if(file == 0) { //exits program if image is not compatible
		printf("Image not supported. Exiting now...\n");
		return 1;
	}

	fread(&BHeader, 14, 1, file);
	fread(&DHeader, 40, 1, file);
	check_errors(file);
	
	fseek(file, BHeader.offset, SEEK_SET);	
	int padding = 4 - (DHeader.image_width*3 % 4); //fixes padding
	
	if(padding == 4){
		padding = 0;
	}

	if(strcmp(argv[1], "--invert") == 0) { // check to see if user picked invert
		printf("Initializing invert...\n");
	} else if (strcmp(argv[1], "--grayscale") == 0) { // check to see if user picked grayscale
		printf("Initializing grayscale..\n");
	} else {
		printf("Try again. Please specify --invert or --grayscale. Exiting...\n");
		return 1;
	}

	for(i = 0; i < DHeader.image_height; i++){ // loop through all the pixels in the image
		for(j = 0; j < DHeader.image_width; j++){
			struct Pixel pixel;
			fread(&pixel, 3, 1, file);
			
			if(strcmp("--invert", argv[1]) == 0){ //inversion
				pixel.blue = ~pixel.blue;
				pixel.green = ~pixel.green;
				pixel.red = ~pixel.red;
			} else if(strcmp("--grayscale", argv[1]) == 0){ //grayscale
				double blue_to_gray = ((double) pixel.blue)/255.0;
				double green_to_gray = ((double) pixel.green)/255.0;
				double red_to_gray = ((double) pixel.red)/255.0;
				double gray_value = .2126*red_to_gray + .7152*green_to_gray + .0722*blue_to_gray;
				
				if(gray_value <= .0031308){
					blue_to_gray = 12.92*gray_value;
					green_to_gray = 12.92*gray_value;
					red_to_gray = 12.92*gray_value;
				} else{
					blue_to_gray = 1.055*pow(gray_value, 1.0/2.4) - .055;
					green_to_gray = 1.055*pow(gray_value, 1.0/2.4) - .055;
					red_to_gray = 1.055*pow(gray_value, 1.0/2.4) - .055;
				}

				pixel.blue = (unsigned char) (blue_to_gray*255.0);
				pixel.green = (unsigned char) (green_to_gray*255.0);
				pixel.red = (unsigned char) (red_to_gray*255.0);
			}
			
			fseek(file, -3, SEEK_CUR); 
			fwrite(&pixel, 3, 1, file); 
		}
		
		fseek(file, padding, SEEK_CUR);
	}

	printf("Your transformations are complete.\n");

	return 1;
}