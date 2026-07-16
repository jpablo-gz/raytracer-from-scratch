#include "3DMath.h"

// get the determinant of a matrix 4x4 with cofactors
double mat4_determinant(const Mat4& mat) {
	
	// get cofactors
	double c00 = mat3_det(mat[1][1], mat[1][2], mat[1][3],
						  mat[2][1], mat[2][2], mat[2][3],
						  mat[3][1], mat[3][2], mat[3][3]);

	double c01 = mat3_det(mat[1][0], mat[1][2], mat[1][3],
						  mat[2][0], mat[2][2], mat[2][3],
						  mat[3][0], mat[3][2], mat[3][3]);
	
	double c02 = mat3_det(mat[1][0], mat[1][1], mat[1][3],
						  mat[2][0], mat[2][1], mat[2][3],
						  mat[3][0], mat[3][1], mat[3][3]);
	
	double c03 = mat3_det(mat[1][0], mat[1][1], mat[1][2],
						  mat[2][0], mat[2][1], mat[2][2],
						  mat[3][0], mat[3][1], mat[3][2]);
	
	// get determinant with cofactors
	return (mat[0][0]*c00) - (mat[0][1]*c01) + (mat[0][2]*c02) - (mat[0][3]*c03);
}

// get the inverse of a matrix
Mat4 mat4_inverse(const Mat4& mat){
	double det = mat4_determinant(mat);
	
	// if the determinant is 0, it doesnt have inverse
	if(abs(det) < 1e-9){
		return Mat4();
	}
	
	// make the cofactor matrix
	Mat4 co_mat;
	
	// 0
	co_mat[0][0] = mat3_det(mat[1][1], mat[1][2], mat[1][3],
							mat[2][1], mat[2][2], mat[2][3],
							mat[3][1], mat[3][2], mat[3][3]);
	
	co_mat[0][1] = (mat3_det(mat[1][0], mat[1][2], mat[1][3],
							 mat[2][0], mat[2][2], mat[2][3],
							 mat[3][0], mat[3][2], mat[3][3]))*(-1.0);
	
	co_mat[0][2] = mat3_det(mat[1][0], mat[1][1], mat[1][3],
							mat[2][0], mat[2][1], mat[2][3],
							mat[3][0], mat[3][1], mat[3][3]);
	
	co_mat[0][3] = (mat3_det(mat[1][0], mat[1][1], mat[1][2],
							 mat[2][0], mat[2][1], mat[2][2],
							 mat[3][0], mat[3][1], mat[3][2]))*(-1.0);
	
	// 1
	co_mat[1][0] = (mat3_det(mat[0][1], mat[0][2], mat[0][3],
							mat[2][1], mat[2][2], mat[2][3],
							mat[3][1], mat[3][2], mat[3][3]))*(-1.0);;
	
	co_mat[1][1] = mat3_det(mat[0][0], mat[0][2], mat[0][3],
							mat[2][0], mat[2][2], mat[2][3],
							mat[3][0], mat[3][2], mat[3][3]);
	
	co_mat[1][2] = (mat3_det(mat[0][0], mat[0][1], mat[0][3],
							 mat[2][0], mat[2][1], mat[2][3],
							 mat[3][0], mat[3][1], mat[3][3]))*(-1.0);
	
	co_mat[1][3] = mat3_det(mat[0][0], mat[0][1], mat[0][2],
							mat[2][0], mat[2][1], mat[2][2],
							mat[3][0], mat[3][1], mat[3][2]);
	
	// 2
	co_mat[2][0] = mat3_det(mat[0][1], mat[0][2], mat[0][3],
							mat[1][1], mat[1][2], mat[1][3],
							mat[3][1], mat[3][2], mat[3][3]);
	
	co_mat[2][1] = (mat3_det(mat[0][0], mat[0][2], mat[0][3],
							 mat[1][0], mat[1][2], mat[1][3],
							 mat[3][0], mat[3][2], mat[3][3]))*(-1.0);
	
	co_mat[2][2] = mat3_det(mat[0][0], mat[0][1], mat[0][3],
							mat[1][0], mat[1][1], mat[1][3],
							mat[3][0], mat[3][1], mat[3][3]);
	
	co_mat[2][3] = (mat3_det(mat[0][0], mat[0][1], mat[0][2],
							 mat[1][0], mat[1][1], mat[1][2],
							 mat[3][0], mat[3][1], mat[3][2]))*(-1.0);
	
	// 3
	co_mat[3][0] = (mat3_det(mat[0][1], mat[0][2], mat[0][3],
							 mat[1][1], mat[1][2], mat[1][3],
							 mat[2][1], mat[2][2], mat[2][3]))*(-1.0);
	
	co_mat[3][1] = mat3_det(mat[0][0], mat[0][2], mat[0][3],
							mat[1][0], mat[1][2], mat[1][3],
							mat[2][0], mat[2][2], mat[2][3]);
	
	co_mat[3][2] = (mat3_det(mat[0][0], mat[0][1], mat[0][3],
							 mat[1][0], mat[1][1], mat[1][3],
							 mat[2][0], mat[2][1], mat[2][3]))*(-1.0);
	
	co_mat[3][3] = mat3_det(mat[0][0], mat[0][1], mat[0][2],
							mat[1][0], mat[1][1], mat[1][2],
							mat[2][0], mat[2][1], mat[2][2]);
	
	// get the adjunt matrix ---> Transpose(co_mat)
	Mat4 adj_mat = mat4_transpose(co_mat);
	
	Mat4 inv;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			inv[i][j] = (1.0/det) * adj_mat[i][j];
		}
	}
	
	return inv;
}
