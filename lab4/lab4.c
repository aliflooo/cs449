// Aliyah Flores
// aff13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*PREDICATE)(const void*);

// ------------------------------------------------------

int filter(void* output, const void* input, int length, int item_size, PREDICATE pred)
{
	// FILL ME IN!
	/* Takes a const void* which points to a value from the array
	** Returns an integer
	** If it's 0, it means it's false and you should ignore the item
	** If it's nonzero, it means it's true and you put the item in the output array */

	/* For each item in the INPUT array:
	** Call the pred funtion with a pointer to that element
	** If true, use memcpy to copy that item from the input array to the OUTPUT array
	** Keep a count of how many items "passed the test" (counter?)
	** Return that count

	** NOTE: memcpy(dest, src, length);

	** To move a void* over by n bytes, you have to:
	** Cast it to a char*
	** Add n to that 
	** Store it back into the void
	** CAN BE DONE IN ONE LINE
	*/
	int count = 0;
	char* pointer = (char*)input; // This casts the void* input into a char*
	char* pointer_out = (char*)output; // Also casts into a char*; 

	for(int i = 0; i < length; i++) { //INPUT ARRAY
		if(pred(input)) {
			//memcpy item output array
			memcpy(output, input, item_size);
			count++;
			pointer = pointer + 4; // Add n (4 bytes for a float because results in main() are float values)
			pointer_out = pointer_out + 4;
			input = (void*)pointer;
			output = (void*)pointer_out;
		}
		else {
			pointer = pointer + 4;
			input = (void*)pointer;
		}
	}
	//move input pointer down by item_size bytes
	return count;
}

int less_than_50(const void* p)
{
	// FILL ME IN!
	/* Pointer to a float
	** Need to cast parameter to a different pointer type
	** Return a "true" (nonzero) value if it's less than 50
	*/
	int value = 0;
	float pointer_p = *(float*)p;
	if(pointer_p < 50) {
		value = pointer_p;
	}
	return value;
}

// ------------------------------------------------------
// you shouldn't have to change the stuff below here.
// you can for testing, but please put it back the way it was before you submit.

#define NUM_VALUES 10

float float_values[NUM_VALUES] =
{
	31.94, 61.50, 36.10,  1.00,  6.35,
	20.76, 69.30, 19.60, 79.74, 51.29,
};

int main()
{
	float filtered[NUM_VALUES];
	int filtered_len = filter(filtered, float_values, NUM_VALUES, sizeof(float), &less_than_50);

	printf("there are %d numbers less than 50:\n", filtered_len);

	for(int i = 0; i < filtered_len; i++)
		printf("\t%.2f\n", filtered[i]);

	return 0;
}