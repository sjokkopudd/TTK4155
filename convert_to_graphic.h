#ifndef CONVERT_TO_GRAPHIC_H
#define CONVERT_TO_GRAPHIC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void convert_to_format(const uint32_t matrix[1][8192], unsigned char output_matrix[128][8]);
void print_matrix(unsigned char matrix[128][8]);

#endif