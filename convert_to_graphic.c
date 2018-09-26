#include "convert_to_graphic.h"
#include "Pinball.c"




void convert_to_format (const uint32_t matrix[1][8192], unsigned char output_matrix[128][8]) {

	FILE *output_file = fopen("output_file.txt", "w"); //this is to write to file output.

	if (output_file == NULL)
	{
    	printf("Error opening file!\n");
    	exit(1);
	}

	int graphic_counter = 0;
	unsigned char output[128][8];

	for (int column = 0; column < 128; column ++) {//fill list with 0b00000000
		for (int row = 0; row < 8; row++){
			output[column][row] = 0b00000000;
		}
	}

	//unsigned char current_byte = 0b00000000;
	for (int row = 0; row < 8; row ++) {// fill list with data
		for (int bit = 0; bit < 8; bit++){
			for (int column = 0; column < 128; column ++){
				if (matrix[0][graphic_counter] != 0){
					output[column][row] |= (1 << bit);
					printf("1");
				}
				else {
					printf("0");
				}
				graphic_counter ++;
			}
			printf("\n");
		}
	}

	for (int i = 0; i < 128; i++){// copy to output matrix
		for (int j = 0; j < 8; j++){
			output_matrix[i][j] = output[i][j];
		}
	}

	for (int j = 0; j < 8; j++) {
		fprintf(output_file, "{ ");
		for (int i = 0; i < 128; i++) {
			fprintf(output_file, "0b");
			for (int bit = 0; bit < 8; bit++) {
				int before;
				int after;

				before = output[i][j];
				after = output[i][j] |= (1 << bit); 	

				if (before != after) { //if bit k in the byte matrix[i][j] is &-ed with a mask of the byte with 1 at postion K
					fprintf(output_file, "0");
				} 
				else {
					fprintf(output_file, "1");
				}
			}
			fprintf(output_file, ", ");
		}
		fprintf(output_file, "} ");
	}
}


void print_matrix(unsigned char matrix[128][8]){
	int counter = 0;
	for (int j = 0; j < 8; j++){
		for (int k = 0; k < 8; k++){
			for (int i = 0; i < 128; i++) {

				int before;
				int after;

				before = matrix[i][j];
				//printf("before: %d\n", before);
				after = matrix[i][j] |= (1 << k); 		
				//printf("after: %d\n", after);

				if (before != after) { //if bit k in the byte matrix[i][j] is &-ed with a mask of the byte with 1 at postion K
					printf("0");
				} 
				else {
					printf("1");
				}
			}
			printf("\n");
		}
	}
}
