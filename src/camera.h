#pragma once

#include "ofMain.h"
#include "3DMath.h"

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

// CAMERA ---------------------------------------------------------------------------------------------------------------------------

// Rodrigues formula for orbit
inline Vec3 vec3_rodrigues(const Vec3& v, const Vec3& axis, double angle) {
	double rad = ofDegToRad(angle);
	Vec3 a = vec3_normal(axis); // axis must be unitary
	
	// v_rod = v*cos(θ) + (a*v)*sin(θ) + a*(a.v)*(1-cos(θ))
	Vec3 term1 = vec3_product(v, cos(rad));
	Vec3 term2 = vec3_product(vec3_crossproduct(a, v), sin(rad));
	Vec3 term3 = vec3_product(a, vec3_dotproduct(a, v) * (1 - cos(rad)));
	
	// v_rod
	return vec3_sum(term1, vec3_sum(term2, term3));
}

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
			double d = vec3_subtraction(e, target).magnitude(); // double d = abs(e.z);
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
	
	void orbit_horizontal(double angle) {
		Vec3 relative_eye = vec3_subtraction(e, target);
		// rotate relative_eye around World_up vector
		Vec3 rotated = vec3_rodrigues(relative_eye, Vec3(0, 1, 0), angle);
		e = vec3_sum(target, rotated);
		updateBasis(); // Recalculate u, v, w
	}

	void orbit_vertical(double angle) {
		Vec3 relative_eye = vec3_subtraction(e, target);
		// rotate relative_eye around local u vector
		Vec3 rotated = vec3_rodrigues(relative_eye, u_vec, angle);
		e = vec3_sum(target, rotated);
		updateBasis(); // Recalculate u, v, w
	}
};

