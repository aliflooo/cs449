// Aliyah Flores
// aff13
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long (*compressBound)(unsigned long length);
int (*compress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);
int (*uncompress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);

int main(int argc, char* argv[]){
	if (argc < 3) {
		printf("Couldn't load the library!\n");
		return 0;
	}	
	// load the zlib library and the 3 functions you need as described below
	void* lib = dlopen("libz.so", RTLD_NOW);
	
	if(lib == NULL) {
		printf("You've got an error, yo!");
		return 1;
	}
	
	compressBound = dlsym(lib, "compressBound");
	if (compressBound == NULL) {
		printf("You've got an error, yo!");
		return 0;
	}
	compress = dlsym(lib, "compress");
	if (compress == NULL) {
		printf("You've got an error, yo!");
		return 0;
	}
	uncompress = dlsym(lib, "uncompress");
	if (uncompress == NULL) {
		printf("You've got an error, yo!");
		return 0;
	}
	
	//open file argv[2] for binary reading
	FILE* file = fopen(argv[2], "rb+");
	void* fbufferu;
	void* fbufferc;
	unsigned long fsizeu;
	unsigned long fsizec;

	fseek(file, 0, SEEK_END); //fseek to end of file
	fsizeu = ftell(file);  // this is the size!
	fseek(file, 0, SEEK_SET); //fseek back to beginning of file
	
	if (strcmp(argv[1], "-c") == 0) {
		// read entire file into an uncompressed buffer that you malloc
		fbufferu = malloc(fsizeu);
		fread(fbufferu, sizeof(fbufferu), 1, file);
		// make an compressed buffer using malloc
		fsizec = compressBound(fsizeu);
		fbufferc = malloc(fsizec); 
		//fprintf(stdout, "%s", fsizec);
		// use compress() to compress the uncompressed buffer into the compressed buffer
		//int (*compress)(void *dest, unsigned long* destLen, const void* source, unsigned long sourceLen);
		compress(fbufferc, &fsizec, fbufferu, fsizeu);
		//frwrite three things to stdout	
		fwrite(&fsizeu, sizeof(unsigned long), 1, stdout); 
		fwrite(&fsizec, sizeof(fsizec), 1, stdout); 
		fwrite(fbufferc, fsizec, 1, stdout);
		return 0;
	} else if (strcmp(argv[1], "-d") == 0) {
		// fread into two things: uncompressed size and compressed size
		fread(&fsizeu, sizeof(fsizeu), 1, file);
		fread(&fsizec, sizeof(fsizec), 1, file);
		// malloc a compressed buffer big enough to hold the compressed data
		fbufferc = malloc(fsizec);
		//fread the rest of data into that buffer
		fread(fbufferc, sizeof(fbufferc), 1, file);
		//malloc an uncompressed buffer big enough to hold the uncompressed data
		fbufferu = malloc(fsizeu);
		// use uncompress() to decompress the compressed data into the uncompressed buffer
		//int (*uncompress)(void *dest2, unsigned long* destLen2, const void* source2, unsigned long sourceLen2);
		uncompress(fbufferu, &fsizeu, fbufferc, fsizec);
		//fwrite the uncompressed buffer to stdout
		fwrite(fbufferu, sizeof(fbufferu), 1, stdout);
		return 0;
	} else {
	 	printf("Sorry, there has been an error."); // complain and exit the program
	 	return 0;
	}
	free(fbufferu);
	free(fbufferc);
}