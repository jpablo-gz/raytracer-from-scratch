#pragma once

#include "ofMain.h"
#include "camera.h"

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
