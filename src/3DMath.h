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

// get the determinant of a matrix 4x4 with cofactors
inline double mat4_determinant(const Mat4& mat) {
	
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
inline Mat4 mat4_inverse(const Mat4& mat){
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
	
	// inv_A = 1/|A| * adj(A);
	Mat4 inv;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			inv[i][j] = (1.0/det) * adj_mat[i][j];
		}
	}
	
	return inv;
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

// RAYTRACING -----------------------------------------------------------------------------------------------------------------------

// class for every ray p(t) = e + td
struct Ray{
public:
	// ray parameters, e = origin, d = direction
	Vec3 e;
	Vec3 d;
	
	// constructor for local-coordinates, we dont normalize direction
	inline Ray(Vec3 origin, Vec3 direction) : e(origin), d(direction) {}
	
	// direction == false --> normalize vector e --> s to keep perspective
	inline Ray(Vec3 origin, Vec3 sd, bool direction){
		e = origin;
		if(direction){
			d = vec3_normal(sd);
		} else{
			d = vec3_normal(vec3_subtraction(sd, e));
		}
	}
	
	// we return the point of the ray in t
	inline Vec3 evaluate(double t) const{
		// p(t) = e + td
		Vec3 p_t = vec3_sum(e, vec3_product(d, t));
		return p_t;
	}
};

// camera is a struct so we can alternate between parallel and perspective
struct Camera{
	Vec3 e; // origin/eye
	Vec3 d; // direction
	int nx; // number of pixels = width
	int ny; // number of pixels = height
	
	// base vectors: unit vectors representing plane directions
	Vec3 w_vec; // oposite of the view direction
	Vec3 u_vec; // x transversal
	Vec3 v_vec; // y transversal
	
	// range for x
	float left;
	float right;
	
	// range for y
	float top;
	float bottom;
	
	// Parallel = false / Perspective = true
	bool p_mode;
	
	//world ubication
	Vec3 world_up;
	Vec3 target;
	
	// empty constructor
	Camera() = default;
	
	void updateBasis() {
		// w: opossite to view direction
		w_vec = vec3_normal(vec3_subtraction(e, target));
		// u: x
		u_vec = vec3_normal(vec3_crossproduct(world_up, w_vec));
		// v: y
		v_vec = vec3_crossproduct(w_vec, u_vec);
	}
	
	// constructor
	Camera(Vec3 view_point, int width, int heigth, vector<float> rangeX, vector<float> rangeY, bool perspective_on = false){
		e = view_point;
		target = Vec3(0, 0, 0); // look at origin by default
		world_up = Vec3(0, 1, 0);
		
		nx = width;
		ny = heigth;
		
		left = rangeX[0];
		right = rangeX[1];
		bottom = rangeY[0];
		top = rangeY[1];
		
		w_vec = {0, 0, 1};
		u_vec = {1, 0, 0};
		v_vec = {0, 1, 0};
		
		p_mode = perspective_on;
		
		updateBasis();
	}
	
	//trace a ray from viewpoint to infinity, in parallel, or perspective
	// i = x, j = y
	inline Ray ray_tracing(int i, int j){
		
		// ajust pixel to the range
		double u = left + (right - left) * (i + 0.5) / nx;
		double v = top - (top - bottom) * (j + 0.5) / ny;
		
		// perspective view
		if(p_mode){
			// ro = e
			Vec3 ro = e;
			// rd = -w + u*u_vec + v*v_vec
			double d = abs(e.z);
			Vec3 rd = vec3_sum(vec3_product(vec3_product(w_vec, -1.0), d), vec3_sum(vec3_product(u_vec, u), vec3_product(v_vec, v)));
						
			return Ray(ro, rd, true);
			
		// parallel view
		}else{
			// ro = e + u*u_vec + v*v_vec
			Vec3 ro(vec3_sum(e, vec3_sum(vec3_product(u_vec, u), vec3_product(v_vec, v))));
			// rd = -w
			Vec3 rd = vec3_product(w_vec, -1.0);
			
			return Ray(ro, rd, true);
		}
	}
};

// 3D OBJECTS -----------------------------------------------------------------------------------------------------------------------

struct HitRecord{
public:
	double t; // "point/distance" where a ray intersects with and object
	Vec3 p;   // exact 3d point where we hit the object
	Vec3 normal; // used for shadows, normal.ligth will tell us where does the sphere recieves ligth
	ofColor color; // color of the object we hit
};

class Object3D{
public:
	// Transformations:
	Mat4 T;
	Mat4 T_inv;
	Mat4 N_mat; // transpose(T_inv)
	double tx, ty, tz; // translation
	double rx, ry, rz; // rotation
	double sx, sy, sz; // scaling
	
	// initial values of shape for reset
	double itx, ity, itz;
	double irx, iry, irz;
	double isx, isy, isz;
	
	inline Object3D() {
		tx = ty = tz = 0.0;
		rx = ry = rz = 0.0;
		sx = sy = sz = 1.0;
		T = Mat4();
		T_inv = Mat4();
		N_mat = Mat4();
	}
	
	// destructor
	inline virtual ~Object3D(){}
	
	//save initial state of shape
	void save_InitialState() {
		// translation
		itx = tx;
		ity = ty;
		itz = tz;
		
		// rotation
		irx = rx;
		iry = ry;
		irz = rz;
		
		// scaling
		isx = sx;
		isy = sy;
		isz = sz;
	}
	
	// intersection: every shape calculates its own way
	// return: true if ray intersects object, false if not
	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord& hr) const = 0;
	
	void Transformation(){
		// individual matrix for each transformation type
		Mat4 translation = mat4_translation(tx, ty, tz);
		Mat4 rotX = mat4_rotationX(rx);
		Mat4 rotY = mat4_rotationY(ry);
		Mat4 rotZ = mat4_rotationZ(rz);
		Mat4 scale = mat4_scale(sx, sy, sz);
		
		// Transformation = T * R * S ---> in that order
		T = mat4_product(translation, mat4_product(rotZ, mat4_product(rotY, mat4_product(rotX, scale))));
		
		T_inv = mat4_inverse(T);
		N_mat = mat4_transpose(T_inv);
	}
};

// 3D SHAPES ------------------------------------------------------------------------------------------------------------------------

class Plane : public Object3D{
public:
	Vec3 a; 			// vector where our plane starts
	Vec3 normal; 		// normal: viewing direction of our plane
	ofColor rgb; 		// color of our plane
	
	// constructor
	inline Plane(Vec3 start_point, Vec3 normal_direction, ofColor color): a(start_point), normal(normal_direction), rgb(color) {}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		// give shape its own local coordinates
		Vec4 l_origin = mat4_vec4_product(T_inv, Vec4(ra.e.x, ra.e.y, ra.e.z, 1.0));
		Vec4 l_dir = mat4_vec4_product(T_inv, Vec4(ra.d.x, ra.d.y, ra.d.z, 0.0));
		Ray l_ray(Vec3(l_origin.x, l_origin.y, l_origin.z), Vec3(l_dir.x, l_dir.y, l_dir.z));
		
		// t = (a - e).n / d.n    ---> A = (a - e).n, B = d.n
		double A = vec3_dotproduct(vec3_subtraction(a, l_ray.e), normal);
		double B = vec3_dotproduct(l_ray.d, normal);
		
		// if B = 0, the ray its parallel to the plane (they never intersect)
		if(abs(B) > 1e-6){ // we use 1e-6 cause some non 0 but close values can also cause errors
			double t = A/B;
	
			// t is visible
			if(t > t_min and t < t_max){
				
				// give data to HitRecord
				hr.t = t;
				hr.p = ra.evaluate(t);
				hr.color = rgb;
				
				// give data to local coordinates
				Vec4 world_n_vec4 = mat4_vec4_product(N_mat, Vec4(normal.x, normal.y, normal.z, 0.0));
				hr.normal = vec3_normal(Vec3(world_n_vec4.x, world_n_vec4.y, world_n_vec4.z));
								
				return true;
			}
		}
		return false;
	}
};

class Sphere : public Object3D {
public:
	// parameters: sphere center ----> c, radius ---> r, color ----> rgb
	Vec3 c;
	double r;
	ofColor rgb;
	
	// constructor
	inline Sphere(Vec3 center, double radius, ofColor color): c(center), r(radius), rgb(color) {
		// translation
		tx = center.x;
		ty = center.y;
		tz = center.z;
		
		// scaling
		sx = radius;
		sy = radius;
		sz = radius;
		
		save_InitialState();
		Transformation();
	}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		// give shape its own local coordinates
		Vec4 l_origin = mat4_vec4_product(T_inv, Vec4(ra.e.x, ra.e.y, ra.e.z, 1.0));
		Vec4 l_dir = mat4_vec4_product(T_inv, Vec4(ra.d.x, ra.d.y, ra.d.z, 0.0));
		
		Ray l_ray(Vec3(l_origin.x, l_origin.y, l_origin.z), Vec3(l_dir.x, l_dir.y, l_dir.z));
		
		// components for general formula
		double A = vec3_dotproduct(l_ray.d, l_ray.d);
		double B = 2.0 * vec3_dotproduct(l_ray.d, l_ray.e);
		double C = vec3_dotproduct(l_ray.e, l_ray.e) - 1.0;
		
		double discriminant = (B*B)-(4.0*A*C);
		
		// if discriminant is < 0, then we dont hit the sphere
		if(discriminant < 0){
			return false;
		}
		
		// if discriminant is 0 or greater, that means we hit the sphere
		
		// viewpoint is outside the sphere
		double t = (-B - sqrt(discriminant)) / (2.0 * A);
		
		if(t < t_min or t > t_max){
			// viewpoint is inside the sphere
			t = (-B + sqrt(discriminant)) / (2.0 * A);
			if(t < t_min or t > t_max){
				// sphere isnt in a visible range
				return false;
			}
		}
		
		// give data to HitRecord
		hr.t = t;
		hr.p = ra.evaluate(t);
		hr.color = rgb;
		
		// n = (p - c)/Radius           Radius == Magnitude(p - c)
		Vec3 l_p = l_ray.evaluate(t);
		Vec3 l_normal = l_p;
			
		Mat4 n_mat = N_mat; // local normal matrix
		Vec4 world_n_v4 = mat4_vec4_product(n_mat, Vec4(l_normal.x, l_normal.y, l_normal.z, 0.0)); // world normal
		
		// give world normal to hitrecord for shadows
		hr.normal = vec3_normal(Vec3(world_n_v4.x, world_n_v4.y, world_n_v4.z));
		
		// hit
		return true;
		
	}
};

class Ellipsoid : public Object3D{
public:
	ofColor rgb; 	// color of the ellipsoid
	
	// constructor
	inline Ellipsoid(Vec3 e_center, Vec3 l_xyz, ofColor color) : rgb(color) {
		// translation
		tx = e_center.x;
		ty = e_center.y;
		tz = e_center.z;
		
		// scaling
		sx = l_xyz.x;
		sy = l_xyz.y;
		sz = l_xyz.z;
		
		save_InitialState();
		Transformation();
	}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		// give shape its own local coordinates
		Vec4 l_origin = mat4_vec4_product(T_inv, Vec4(ra.e.x, ra.e.y, ra.e.z, 1.0));
		Vec4 l_dir = mat4_vec4_product(T_inv, Vec4(ra.d.x, ra.d.y, ra.d.z, 0.0));
		
		Ray l_ray(Vec3(l_origin.x, l_origin.y, l_origin.z), Vec3(l_dir.x, l_dir.y, l_dir.z));
		
		// we treat the ellipsoid as an unitary sphere
		double A = vec3_dotproduct(l_ray.d, l_ray.d);
		double B = 2.0 * vec3_dotproduct(l_ray.d, l_ray.e);
		double C = vec3_dotproduct(l_ray.e, l_ray.e) - 1.0;
		
		double discriminant = (B*B)-(4*A*C);
		
		// if discriminant is < 0, then we dont hit the ellipsoid
		if(discriminant < 0){
			return false;
		}
		
		// if discriminant is 0 or greater, that means we hit the ellipsoid
		
		// viewpoint is outside the ellipsoid
		double t = (-B - sqrt(discriminant)) / (2.0 * A);
		
		if(t < t_min or t > t_max){
			// viewpoint is inside the ellipsoid
			t = (-B + sqrt(discriminant)) / (2.0 * A);
			if(t < t_min or t > t_max){
				// ellipsoid isnt in a visible range
				return false;
			}
		}
		
		// give data to HitRecord
		hr.t = t; // same in both local and world
		hr.color = rgb;
		hr.p = ra.evaluate(t);
		
		Vec3 local_p = l_ray.evaluate(t);
		Vec3 local_normal = local_p;
				
		Mat4 n_mat = N_mat;
		Vec4 world_n_v4 = mat4_vec4_product(n_mat, Vec4(local_normal.x, local_normal.y, local_normal.z, 0.0));
				
		// world normal for shadows
		hr.normal = vec3_normal(Vec3(world_n_v4.x, world_n_v4.y, world_n_v4.z));
		
		// hit
		return true;
	}
};

class Cube : public Object3D{
public:
	ofColor rgb;
	// cube sizes:
	Vec3 min_p, max_p;
	
	inline Cube(Vec3 c_center, double x, double y, double z, ofColor color): rgb(color){
		// initial position as traslation
		tx = c_center.x;
		ty = c_center.y;
		tz = c_center.z;
		
		// turn lengths into min/max points
		min_p = Vec3(-0.5, -0.5, -0.5);
		max_p = Vec3(0.5, 0.5, 0.5);
		
		// scale of cube
		sx = x;
		sy = y;
		sz = z;
		
		min_p = Vec3(-0.5, -0.5, -0.5);
		max_p = Vec3(0.5, 0.5, 0.5);
		
		save_InitialState();
		Transformation();
	}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		// give shape its own local coordinates
		Vec4 l_origin = mat4_vec4_product(T_inv, Vec4(ra.e.x, ra.e.y, ra.e.z, 1.0));
		Vec4 l_dir = mat4_vec4_product(T_inv, Vec4(ra.d.x, ra.d.y, ra.d.z, 0.0));
		
		Ray l_ray(Vec3(l_origin.x, l_origin.y, l_origin.z), Vec3(l_dir.x, l_dir.y, l_dir.z));
		
		double t_near = -INFINITY;
		double t_far = INFINITY;
		
		// helps us know where we hit so we can know the normal
		int hit_axis = 0;
		
		// check intersections in x(0), y(1) or z(2)
		for (int i = 0; i < 3; i++) {
			double d = l_ray.d[i];
			double e = l_ray.e[i];
			double min_v = min_p[i];
			double max_v = max_p[i];

			
			double div_d = 1.0/d; // if(d == 0.0) ---> div_d = inf
			
			// point where ray "enters" the cube
			double t0 = (min_v - e) * div_d;
			// point where ray "leaves" the cube
			double t1 = (max_v - e) * div_d;
			
			// we are "inside" or "infront" of the cube
			if(div_d < 0.0) swap(t0, t1);
			
			// we look for the values of t0, t1: where the rays hit
			if(t0 > t_near){
				t_near = t0;
				hit_axis = i;
			}
			
			if(t1 < t_far){
				t_far = t1;
			}
			
			// ray doesnt hit the cube
			if(t_near > t_far) return false;
		}
		
		// cube isnt in the visible range
		if(t_near < t_min or t_near > t_max) return false;
		
		// give data to HitRecord
		hr.t = t_near;
		hr.p = ra.evaluate(t_near);
		hr.color = rgb;
		
		// normal depends on the plane where the ray hit
		Vec3 l_normal(0, 0, 0);
		if (l_ray.d[hit_axis] < 0) {
			if (hit_axis == 0) l_normal.x = 1;
			else if (hit_axis == 1) l_normal.y = 1;
			else l_normal.z = 1;
		} else {
			if (hit_axis == 0) l_normal.x = -1;
			else if (hit_axis == 1) l_normal.y = -1;
			else l_normal.z = -1;
		}
		
		// give data to local coordinates
		Vec4 world_n_vec4 = mat4_vec4_product(N_mat, Vec4(l_normal.x, l_normal.y, l_normal.z, 0.0));
		hr.normal = vec3_normal(Vec3(world_n_vec4.x, world_n_vec4.y, world_n_vec4.z));

		return true;
	}
};
