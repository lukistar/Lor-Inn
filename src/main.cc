#include <cstdio>
#include <cstdlib>

int** create_matrix(int width, int height);
int print_matrix(int **matrix, int width, int height);
int** iterate(int **image, int i_width, int i_height, int **matrix, int m_width, int m_height, int devisor, int offset);

int main() {
	int **matrix = create_matrix(5, 5);
	int k = 1;
    for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			matrix[i][j] = k%10;
			k++;
		}
	}
	print_matrix(matrix, 5, 5);
	return 0;
}

int print_matrix(int **matrix, int width, int height) {
	for (int i = 0; i < height; i++) {
		printf("[ ");
		for (int k = 0; k < width; k++) {
			printf("%d ", matrix[i][k]);
		}
		printf("]\n\n");
	}
	return 0;
}

int** create_matrix(int width = 3, int height = 3) {
	int ** matrix = (int**)malloc(sizeof(int*) * height);
	for (int i = 0; i < height; i++) {
		*(matrix + i) = (int*)malloc(sizeof(int) * width);
	}
	return matrix;
}

int** iterate(int **image, int i_width, int i_height, int **matrix, int m_width, int m_height, int devisor = 1, int offset = 1) {
	int **image_copy = create_matrix(i_width, i_height);
	for (int i = 0; i < i_height; i++) {
		for (int k = 0; k < i_width; k++) {
			image_copy[i][k] = image[i][k];
		}
	}
	image = image_copy;
	
	for (int i = 0; i < i_height; i++) {
		for (int k = 0; k < i_width; k++) {
			int c_width = m_width/2 + 1;
			int c_height = m_height/2 + 1;
			int sum = image[i][k] * matrix[c_height][c_width];
			for (int ii = 0; ii < (m_height - 1)/2; ii++) {
				sum+= image[i+ii][k] * matrix[c_height+ii][c_width];
				sum+= image[i-ii][k] * matrix[c_height-ii][c_width];
				sum+= image[i][k+ii] * matrix[c_height][c_width+ii];
				sum+= image[i][k-ii] * matrix[c_height][c_width-ii];
				sum+= image[i+ii][k+ii] * matrix[c_height+ii][c_width+ii];
				sum+= image[i-ii][k-ii] * matrix[c_height-ii][c_width-ii];
				sum+= image[i+ii][k-ii] * matrix[c_height+ii][c_width-ii];
				sum+= image[i-ii][k+ii] * matrix[c_height-ii][c_width+ii];
				for (int kk = ii-1; kk > 0; kk--) {
					sum+= image[i+ii-kk][k+ii] * matrix[c_height+ii-kk][c_width+ii];
					sum+= image[i+ii][k+ii-kk] * matrix[c_height+ii][c_width+ii-kk];
					sum+= image[i-ii+kk][k-ii] * matrix[c_height-ii+kk][c_width-ii];
					sum+= image[i-ii][k-ii+kk] * matrix[c_height-ii][c_width-ii+kk];
					sum+= image[i+ii-kk][k-ii] * matrix[c_height+ii-kk][c_width-ii];
					sum+= image[i+ii][k-ii+kk] * matrix[c_height+ii][c_width-ii+kk];
					sum+= image[i-ii+kk][k+ii] * matrix[c_height-ii+kk][c_width+ii];
					sum+= image[i-ii][k+ii-kk] * matrix[c_height-ii][c_width+ii-kk];
				}
			}
			sum/= devisor;
			image[i][k] = sum;
		}
	}
	return 0;
}