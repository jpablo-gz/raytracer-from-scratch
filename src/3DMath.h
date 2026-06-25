#pragma once

#include "ofMain.h"
#include <vector>

// 3D Vector:
struct Vec3 {
public:
	// parameters:
	double x, y, z;
	
	// constructor
	inline Vec3(double nx = 0, double ny = 0, double nz = 0): x(nx), y(ny), z(nz){}
	
	// for iteration ---> used on cube
	const double& operator[](int index) const{
		if (index == 0) return x;
		if (index == 1) return y;
		return z;
	}
	
	// magnitude:
	inline double magnitude(){
		return sqrt(x*x + y*y + z*z);
	}
};

// get the normal of a 3d vector
inline Vec3 vec3_normal(Vec3 v){
	double m = v.magnitude();
	return Vec3(v.x/m, v.y/m, v.z/m);
}

// get a new vector with the sum of 2 vectors
inline Vec3 vec3_sum(Vec3 a, Vec3 b){
	return Vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

// get origin --> destination vector with a substraction: d = destiny, o = origin
inline Vec3 vec3_subtraction(Vec3 d, Vec3 o){
	return Vec3(d.x-o.x, d.y-o.y, d.z-o.z);
}

// product of a vector and a scalar
inline Vec3 vec3_product(Vec3 a, double k){
	return Vec3(k*a.x, k*a.y, k*a.z);
}

// returns the result of the dot product between 2 vectors
inline double vec3_dotproduct(Vec3 a, Vec3 b){
	return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
}

// returns the result vector of the cross product of 2 vectors
inline Vec3 vec3_crossproduct(Vec3 a, Vec3 b){
	return Vec3((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));
}

// class for every ray p(t) = e + td
struct Ray{
public:
	// ray parameters, e = origin, d = direction
	Vec3 e;
	Vec3 d;
	
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
	Vec3 e; // origin
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
	
	// empty constructor
	Camera() = default;
	
	// constructor
	Camera(Vec3 view_point, int width, int heigth, vector<float> rangeX, vector<float> rangeY, bool perspective_on = false){
		e = view_point;
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
			// rd = -d*w + u*u_vec + v*v_vec
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
	// destructor
	inline virtual ~Object3D(){}
	
	// we calculate intersection: every shape calculates its own way
	// return: true if ray intersects object, false if not
	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord& hr) const = 0;
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
		
		// t = (a - e).n / d.n    ---> A = (a - e).n, B = d.n
		double A = vec3_dotproduct(vec3_subtraction(a, ra.e), normal);
		double B = vec3_dotproduct(ra.d, normal);
		
		// if B = 0, the ray its parallel to the plane (they never intersect)
		if(abs(B) > 1e-6){ // we use 1e-6 cause some non 0 but close values can also cause errors
			double t = A/B;
	
			// t is visible
			if(t > t_min and t < t_max){
				
				// give data to HitRecord
				hr.t = t;
				hr.p = ra.evaluate(t);
				hr.normal = vec3_normal(normal);
				hr.color = rgb;
			
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
	inline Sphere(Vec3 center, double radius, ofColor color): c(center), r(radius), rgb(color) {}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		
		// components for general formula
		double A = vec3_dotproduct(ra.d, ra.d);
		double B = (vec3_dotproduct(ra.d, vec3_subtraction(ra.e, c)))*2;
		double C = vec3_dotproduct(vec3_subtraction(ra.e, c), vec3_subtraction(ra.e, c)) - (r*r);
		
		double discriminant = (B*B)-(4*A*C);
		
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
		// n = (p - c)/Radius           Radius == Magnitude(p - c)
		hr.normal = vec3_product(vec3_subtraction(hr.p, c), 1.0/r);
		hr.color = rgb;
		
		// hit
		return true;
		
	}
};

class Ellipsoid : public Object3D{
public:
	Vec3 center; 	// center of the elipsoid
	Vec3 axes;		// (a, b, c): a = length in x, b = length in y, c = length in z
	// if(a = b = c) --> Sphere
	ofColor rgb; 	// color of the ellipsoid
	
	// constructor
	inline Ellipsoid(Vec3 e_center, Vec3 l_xyz, ofColor color) : center(e_center), rgb(color) {
		// axes cant have zeros
		if(l_xyz.x == 0) l_xyz.x = 0.0001;
		if(l_xyz.y == 0) l_xyz.y = 0.0001;
		if(l_xyz.z == 0) l_xyz.z = 0.0001;
		
		axes = l_xyz;
	}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		
		// axes cant have zeros
		if(axes.x < 1e-6 or axes.y < 1e-6 or axes.z < 1e-6){
			return false;
		}
		
		Vec3 w = vec3_subtraction(ra.e, center);
		Vec3 dr = ra.d;
		
		double a2 = 1.0/(axes.x*axes.x);
		double b2 = 1.0/(axes.y*axes.y);
		double c2 = 1.0/(axes.z*axes.z);
		
		double A = (dr.x*dr.x)*a2 + (dr.y*dr.y)*b2 + (dr.z*dr.z)*c2;
		double B = 2*((w.x*dr.x)*a2 + (w.y*dr.y)*b2 + (w.z*dr.z)*c2);
		double C = (w.x*w.x)*a2 + (w.y*w.y)*b2 + (w.z*w.z)*c2 - 1.0;
		
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
		hr.t = t;
		hr.color = rgb;
		hr.p = ra.evaluate(t);
		
		// n = gradient of function
		Vec3 gradient;
		
		gradient.x = (hr.p.x - center.x)*a2;
		gradient.y = (hr.p.y - center.y)*b2;
		gradient.z = (hr.p.z - center.z)*c2;
		
		hr.normal = vec3_normal(gradient);
		
		// hit
		return true;
	}
};

class Cube : public Object3D{
public:
	Vec3 center;
	ofColor rgb;
	// cube sizes:
	double l_x;
	double l_y;
	double l_z;
	Vec3 min_p, max_p;
	
	inline Cube(Vec3 c_center, double x, double y, double z, ofColor color): center(c_center), l_x(x), l_y(y), l_z(z), rgb(color){
		// turn lengths into min/max points
		min_p = vec3_subtraction(center, Vec3(l_x/2.0, l_y/2.0, l_z/2.0));
		max_p = vec3_sum(center, Vec3(l_x/2.0, l_y/2.0, l_z/2.0));
	}
	
	// parameters: Ray: origin --> e, direction ---> d
	virtual bool hit(const Ray &ra, double t_min, double t_max, HitRecord& hr) const override{
		
		double t_near = -INFINITY;
		double t_far = INFINITY;
		
		// helps us know where we hit so we can know the normal
		int hit_axis = 0;
		double d_axis;
		
		double d, e, min, max;
		// check intersections in x(0), y(1) or z(2)
		for(int i = 0; i < 3; i++){
			d = ra.d[i];
			e = ra.e[i];
			min = min_p[i];
			max = max_p[i];
			
			double div_d = 1.0/d; // if(d == 0.0) ---> div_d = inf
			
			// point where ray "enters" the cube
			double t0 = (min - e)*div_d;
			// point where ray "leaves" the cube
			double t1 = (max - e)*div_d;
			
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
		hr.normal = Vec3(0, 0, 0);
		if (ra.d[hit_axis] < 0) {
			if (hit_axis == 0) hr.normal.x = 1;
			else if (hit_axis == 1) hr.normal.y = 1;
			else hr.normal.z = 1;
		} else {
			if (hit_axis == 0) hr.normal.x = -1;
			else if (hit_axis == 1) hr.normal.y = -1;
			else hr.normal.z = -1;
		}

		return true;
	}
};
