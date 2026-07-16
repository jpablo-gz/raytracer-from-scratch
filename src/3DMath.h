#pragma once

#include "ofMain.h"
#include <vector>

// 3D VECTOR ------------------------------------------------------------------------------------------------------------------------
struct Vec3 {
public:
	// parameters:
	double x, y, z;
	
	// constructor
	inline Vec3(double nx = 0, double ny = 0, double nz = 0): x(nx), y(ny), z(nz){}
	
	// for iteration ---> used on cube
	const double& operator[](int index) const{
		if(index == 0) return x;
		if(index == 1) return y;
		return z;
	}
	
	// magnitude:
	inline double magnitude(){
		return sqrt(x*x + y*y + z*z);
	}
};

// get the normal of a 3d vector
inline Vec3 vec3_normal(const Vec3& v){
	double m = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return Vec3(v.x/m, v.y/m, v.z/m);
}

// get a new vector with the sum of 2 vectors
inline Vec3 vec3_sum(const Vec3& a, const Vec3& b){
	return Vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

// get origin --> destination vector with a substraction: d = destiny, o = origin
inline Vec3 vec3_subtraction(const Vec3& d, const Vec3& o){
	return Vec3(d.x-o.x, d.y-o.y, d.z-o.z);
}

// product of a vector and a scalar
inline Vec3 vec3_product(const Vec3& a, double k){
	return Vec3(k*a.x, k*a.y, k*a.z);
}

// returns the result of the dot product between 2 vectors
inline double vec3_dotproduct(const Vec3& a, const Vec3& b){
	return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
}

// returns the result vector of the cross product of 2 vectors
inline Vec3 vec3_crossproduct(const Vec3& a, const Vec3& b){
	return Vec3((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));
}

// 3D MATRIX ------------------------------------------------------------------------------------------------------------------------

// 4 parameter vector for transformations
struct Vec4 {
public:
	// parameters:
	double x, y, z, w; // w = 0 for direction, w = 1 for transformations
	
	// constructor
	inline Vec4(double nx = 0, double ny = 0, double nz = 0, double nw = 0): x(nx), y(ny), z(nz), w(nw){}
	
	// for iteration ---> used on cube
	const double& operator[](int index) const{
		if(index == 0) return x;
		if(index == 1) return y;
		if(index == 2) return z;
		return w;
	}
	
	// magnitude:
	inline double magnitude(){
		return sqrt(x*x + y*y + z*z + w*w);
	}
};

// 4x4 matriz
struct Mat4 {
	// static array
	double m[4][4];
	
	// constructor, the matrix starts as a identity matriz
	inline Mat4(){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if(i == j){
					m[i][j] = 1.0;
				}
				else {
					m[i][j] = 0.0;
				}
			}
		}
	}
	
	// read values
	inline const double* operator[](int i) const {
		return m[i];
	}

	// edit values
	inline double* operator[](int i) {
		return m[i];
	}
};

// get a product vec4 of a matrix and a vec4
inline Vec4 mat4_vec4_product(const Mat4& mat, const Vec4& vec){
	Vec4 res;
	
	res.x = (mat[0][0]*vec.x) + (mat[0][1]*vec.y) + (mat[0][2]*vec.z) + (mat[0][3]*vec.w);
	res.y = (mat[1][0]*vec.x) + (mat[1][1]*vec.y) + (mat[1][2]*vec.z) + (mat[1][3]*vec.w);
	res.z = (mat[2][0]*vec.x) + (mat[2][1]*vec.y) + (mat[2][2]*vec.z) + (mat[2][3]*vec.w);
	res.w = (mat[3][0]*vec.x) + (mat[3][1]*vec.y) + (mat[3][2]*vec.z) + (mat[3][3]*vec.w);
	
	return res;
}

// get the transpose of a 4x4 matrix
inline Mat4 mat4_transpose(const Mat4& mat) {
	Mat4 res;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i][j] = mat[j][i];
		}
	}
	
	return res;
}

// get the product between 2 4x4 matrix
inline Mat4 mat4_product(const Mat4& A, const Mat4& B) {
	Mat4 res;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				res[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	
	return res;
}

// get the determinant of a matrix 3x3 ---> used for cofactors
inline double mat3_det(double a, double b, double c,
					   double d, double e, double f,
					   double g, double h, double i) {
	
	return (a*(e*i - h*f)) - (b*(d*i - g*f)) + (c*(d*h - g*e));
}

// TRANSFORMATIONS ------------------------------------------------------------------------------------------------------------------

inline Mat4 mat4_translation(double tx, double ty, double tz) {
	Mat4 mat; // starts as an identity matrix
	
	// apply translation
	mat[0][3] = tx;
	mat[1][3] = ty;
	mat[2][3] = tz;
	
	return mat;
}

inline Mat4 mat4_scale(double sx, double sy, double sz) {
	Mat4 mat; // starts as identity matrix
	
	// apply scaling
	mat[0][0] = sx;
	mat[1][1] = sy;
	mat[2][2] = sz;
	mat[3][3] = 1.0;
	
	return mat;
}

// Rotation over x
inline Mat4 mat4_rotationX(double angleX) {
	double rad = ofDegToRad(angleX); // turn degrees to radians
	
	// starts as identity matrix
	Mat4 mat;
	
	// apply rotation on x
	mat[1][1] = cos(rad);
	mat[1][2] = -sin(rad);
	mat[2][1] = sin(rad);
	mat[2][2] = cos(rad);
	
	return mat;
}

// Rotation over y
inline Mat4 mat4_rotationY(double angleY) {
	double rad = ofDegToRad(angleY); // turn degrees to radians
	
	// starts as identity matrix
	Mat4 mat;
	
	// apply rotation on y
	mat[0][0] = cos(rad);
	mat[0][2] = sin(rad);
	mat[2][0] = -sin(rad);
	mat[2][2] = cos(rad);
	
	return mat;
}

// Rotation over z
inline Mat4 mat4_rotationZ(double angleZ) {
	double rad = ofDegToRad(angleZ); // turn degrees to radians
	
	// starts as identity matrix
	Mat4 mat;
	
	// apply rotation on z
	mat[0][0] = cos(rad);
	mat[0][1] = -sin(rad);
	mat[1][0] = sin(rad);
	mat[1][1] = cos(rad);
	
	return mat;
}

double mat4_determinant(const Mat4& mat);
Mat4 mat4_inverse(const Mat4& mat);

