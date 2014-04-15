#include <cstdio>
#include <cstdlib>
#include <ctime>

int** create_matrix(int width, int height);
void print_matrix(int **matrix, int width, int height);
int** convolute(int **image, int i_width, int i_height, int **matrix, int m_width, int m_height, int devisor, int offset);
int** fill_matrix(int **matrix, int width, int height, int value);
int** copy_matrix(int **matrix, int width, int height);
int** fill_matrix_random(int **matrix, int widht, int height, int min, int max);
void rotate_90(int **matrix, int width, int height);
void free_matrix(int **matrix, int width, int height);

int** blur(int **image, int width, int height);
int** gauss_blur(int **image, int width, int height);
int** sharpen(int **image, int width, int height);
int** find_contour(int **image, int width, int height);
int** sobel_find_contour(int **image, int width, int height);
int** laplace_find_contour(int **image, int width, int height);
int** emboss_find_contour(int **image, int width, int height);

int** convolute(int **image, int i_width, int i_height, int **matrix, int m_width, int m_height, int devisor = 1, int offset = 0) {
	int **image_copy = copy_matrix(image, i_width, i_height);
	image = image_copy;
	
	for (int i = 0; i < i_height; i++) {
		for (int k = 0; k < i_width; k++) {
			int c_width = m_width/2;
			int c_height = m_height/2;
			int sum = 0;
			sum = image[i][k] * matrix[c_height][c_width];
			for (int ii = 1; ii <= (m_height - 1)/2; ii++) {
				sum+= image[(i+ii) % i_height][k] * matrix[c_height+ii][c_width];
				sum+= image[abs( (i-ii) % i_height )][k] * matrix[c_height-ii][c_width];
				sum+= image[i][(k+ii) % i_width] * matrix[c_height][c_width+ii];
				sum+= image[i][(abs( k-ii) % i_width )] * matrix[c_height][c_width-ii];
				sum+= image[(i+ii) % i_height][(k+ii) % i_width] * matrix[c_height+ii][c_width+ii];
				sum+= image[abs( (i-ii) % i_height )][abs( (k-ii) % i_width )] * matrix[c_height-ii][c_width-ii];
				sum+= image[(i+ii) % i_height][abs( (k-ii) % i_width )] * matrix[c_height+ii][c_width-ii];
				sum+= image[abs( (i-ii) % i_height )][(k+ii) % i_width] * matrix[c_height-ii][c_width+ii];
				for (int kk = ii-1; kk > 0; kk--) {
					sum+= image[(i+ii-kk) % i_height][(k+ii) % i_width] * matrix[c_height+ii-kk][c_width+ii];
					sum+= image[(i+ii) % i_height][(k+ii-kk) % i_width] * matrix[c_height+ii][c_width+ii-kk];
					sum+= image[abs( (i-ii+kk) % i_height )][abs( (k-ii) % i_width )] * matrix[c_height-ii+kk][c_width-ii];
					sum+= image[abs( (i-ii) % i_height )][abs( (k-ii+kk) % i_width )] * matrix[c_height-ii][c_width-ii+kk];
					sum+= image[(i+ii-kk) % i_height][abs( (k-ii) % i_width )] * matrix[c_height+ii-kk][c_width-ii];
					sum+= image[(i+ii) % i_height][abs( (k-ii+kk) % i_width )] * matrix[c_height+ii][c_width-ii+kk];
					sum+= image[abs( (i-ii+kk) % i_height )][(k+ii) % i_width] * matrix[c_height-ii+kk][c_width+ii];
					sum+= image[abs( (i-ii) % i_height )][(k+ii-kk) % i_width] * matrix[c_height-ii][c_width+ii-kk];
				}
			}
			sum/= devisor;
			sum+= offset;
			if (sum > 255) sum = 255;
			image[i][k] = abs(sum);
		}
	}
	return image;
}

int** blur(int **image, int width, int height) {
	int **matrix = create_matrix(3, 3);
	fill_matrix(matrix, 3, 3, 1);
	
	return convolute(image, width, height, matrix, 3, 3, 9, 0);
}

int** gauss_blur(int **image, int width, int height) {
	int **matrix = create_matrix(5, 5);
	
	matrix[0][0] = 2;
	matrix[1][0] = 4;
	matrix[0][1] = 4;
	matrix[0][4] = 2;
	matrix[0][3] = 4;
	matrix[1][4] = 4;
	matrix[4][0] = 2;
	matrix[3][0] = 4;
	matrix[4][1] = 4;
	matrix[4][4] = 2;
	matrix[4][3] = 4;
	matrix[3][4] = 4;
	
	matrix[0][2] = 5;
	matrix[2][0] = 5;
	matrix[2][4] = 5;
	matrix[4][2] = 5;
	
	matrix[2][2] = 12;
	matrix[1][2] = 12;
	matrix[3][2] = 12;
	matrix[2][1] = 12;
	matrix[2][3] = 12;
	
	matrix[1][1] = 9;
	matrix[3][3] = 9;
	matrix[1][3] = 9;
	matrix[3][1] = 9;
	
	int sum = 159;
	
	return convolute(image, width, height, matrix, 5, 5, sum, 0);
}

// only for square
void rotate_90(int **matrix, int width, int height) {
	for (int i = 0; i < width; i++)	{
		for (int j = 0; j < height; j++) {
			matrix[i][j] = matrix[height-1-j][i];
		}
	}
}

void free_matrix(int **matrix, int width, int height) {
	for (int i = 0; i < height; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int** sharpen(int **image, int width, int height) {
	int **matrix = create_matrix(3, 3);
	
	fill_matrix(matrix, 3, 3, -1);
	matrix[1][1] = 9;
	
	return convolute(image, width, height, matrix, 3, 3, 1, 0);
}

int** find_contour(int **image, int width, int height) {
	int **matrix = create_matrix(3, 3);
	
	fill_matrix(matrix, 3, 3, 0);
	
	matrix[1][1] = 1;
	matrix[2][1] = -1;
	int **matrix2 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix3 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix4 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix5 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	for (int i = 0; i < height-1; i++) {
		for (int j = 0; j < width-1; j++) {
			matrix2[i][j] = matrix2[i][j] + matrix3[i][j] + matrix4[i][j] + matrix5[i][j];
			if (abs(matrix2[i][j]) > 255) matrix2[i][j] = 255;
			matrix2[i][j] = abs(matrix2[i][j]);
		}
	}
	
	free(matrix3);
	free(matrix4);
	free(matrix5);
	
	return matrix2;
}

int** sobel_find_contour(int **image, int width, int height) {
	int **matrix = create_matrix(3, 3);
	
	fill_matrix(matrix, 3, 3, 0);
	
	matrix[0][0] = 1;
	matrix[0][1] = 2;
	matrix[0][2] = 1;
	
	matrix[2][0] = -1;
	matrix[2][1] = -2;
	matrix[2][2] = -1;
	
	int **matrix2 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix3 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix4 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix5 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	for (int i = 0; i < height-1; i++) {
		for (int j = 0; j < width-1; j++) {
			matrix2[i][j] = matrix2[i][j] + matrix3[i][j] + matrix4[i][j] + matrix5[i][j];
			if (abs(matrix[i][j]) > 255) matrix[i][j] = 255;
			matrix[i][j] = abs(matrix[i][j]);
		}
	}
	
	free(matrix3);
	free(matrix4);
	free(matrix5);
	
	return matrix2;
}

int** laplace_find_contour(int **image, int width, int height) {
	int **matrix = create_matrix(3, 3);
	
	fill_matrix(matrix, 3, 3, 0);
	
	matrix[0][1] = -1;
	matrix[1][0] = -1;
	matrix[1][2] = -1;
	matrix[2][1] = -1;
	matrix[1][1] = 4;
	
	int **matrix2 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix3 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix4 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	rotate_90(matrix, 3, 3);
	int **matrix5 = convolute(image, width, height, matrix, 3, 3, 1, 0);
	
	for (int i = 0; i < height-1; i++) {
		for (int j = 0; j < width-1; j++) {
			matrix2[i][j] = matrix2[i][j] + matrix3[i][j] + matrix4[i][j] + matrix5[i][j];
			if (abs(matrix[i][j]) > 255) matrix[i][j] = 255;
			matrix[i][j] = abs(matrix[i][j]);
		}
	}
	
	free(matrix3);
	free(matrix4);
	free(matrix5);
	
	return matrix2;
}

int** emboss_find_contour(int **image, int width, int height) {
	int **matrix = create_matrix(3, 3);
	
	fill_matrix(matrix, 3, 3, -1);
	matrix[1][1] = 1;
	matrix[2][1] = -1;
	
	return convolute(image, width, height, matrix, 3, 3, 1, 127);
}

void print_matrix(int **matrix, int width, int height) {
	for (int i = 0; i < height; i++) {
		printf("[ ");
		for (int k = 0; k < width; k++) {
			printf("%d ", matrix[i][k]);
		}
		printf("]\n\n");
	}
}

int** create_matrix(int width = 3, int height = 3) {
	int **matrix = (int**)malloc(sizeof(int*) * height);
	for (int i = 0; i < height; i++) {
		*(matrix + i) = (int*)malloc(sizeof(int) * width);
	}
	return matrix;
}

int** fill_matrix(int **matrix, int width = 3, int height = 3, int value = 0) {
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			matrix[i][k] = value;
		}
	}
	return matrix;
}

int** copy_matrix(int **matrix, int width = 3, int height = 3) {
	int **matrix_copy = create_matrix(width, height);
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			matrix_copy[i][k] = matrix[i][k];
		}
	}
	return matrix_copy;
}

int** fill_matrix_random(int ** matrix, int width, int height, int min, int max) {
	srand(time(NULL));
	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			matrix[i][k] = (rand() % (max+1)) + min;
		}
	}
	return matrix;
}