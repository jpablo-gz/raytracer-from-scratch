#include "ofApp.h"
#include <vector>

using namespace std;

// SHAPES CONSTRUCTIONS ------------------------------------------------------------------------------------------------------

// Plane (Vec3 start_point, Vec3 normal_direction, ofColor color)
// Sphere (Vec3 center, double radius, ofColor color)
// Ellipsoid (Vec3 center, Vec3 lengths_abc, ofColor color)
// Cube (Vec3 center, double lenght x, y, z, ofColor color)

void ofApp::setup() {
	
	// RAYTRACING SETTINGS ---------------------------------------------------------------------------------------------------
	
	// number of pixels: w x h
	w = 640;
	h = 480;
	
	// where our rays start on the perspective view
	viewpoint = {0, 0, 1};
	
	// light settings: position, color and intensity
	li_position = {0, 5, 0};
	li_color = ofColor(255, 255, 255);
	ka = 0.2; // ambient light
	
	// Background color
	Background_color = ofColor(0, 0, 0);
	
	// screen ranges for x and y
	vector<float> range_x = {-2, 2};
	vector<float> range_y = {-1.5, 1.5};
	
	// camera: parallel = false, perspective = true
	cam = Camera(viewpoint, w, h, range_x, range_y, true);

	// SHAPES IN SCENE -------------------------------------------------------------------------------------------------------
	
	// Plane: Gray (0.5, 0.5, 0.5) in y = -1.4
	scene.push_back(new Plane(Vec3(0, -1.4, 0), Vec3(0, 1, 0), ofColor(128, 128, 128)));

	// Sphere: Red (1, 0, 0) in z < 0 and inside range x:[-2,2], y:[-1.5,1.5]
	scene.push_back(new Sphere(Vec3(1.0, 0.5, -2.0), 1.0, ofColor(255, 0, 0)));

	// Cube (Extra): Green (0, 1, 0) in z < 0 and inside range x:[-2,2], y:[-1.5,1.5]
	scene.push_back(new Cube(Vec3(-1.2, 0.0, -3.0), 1.5, 1.5, 1.5, ofColor(0, 255, 0)));

	// Ellipsoid: Blue (0, 0, 1) in z > 0 between z=1 and view plane z = 0
	//scene.push_back(new Ellipsoid(Vec3(0, 0.4, 0.4), Vec3(0.2, 0.1, 0.3), ofColor(0, 0, 255)));
	
	// Ellipsoid: Blue (0, 0, 1) in z > 0 and outside range x:[-2,2], y:[-1.5,1.5]
	scene.push_back(new Ellipsoid(Vec3(-4, 2, -3), Vec3(1, 2, 1), ofColor(0, 0, 255)));
	
	colorPixels.allocate(w, h, OF_PIXELS_RGB);
	texColor.allocate(colorPixels);
	
	// Trace all shapes for the first time
	traceAll();
}

// Render
void ofApp::traceAll(){
	// ejecution time measurement
	auto start = chrono::high_resolution_clock::now();
	
	// we go trought every pixel in screen (x, y)
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			// we make a ray for every pixel
			Ray actual_ray = cam.ray_tracing(x, y);
			
			HitRecord l_hrec;
			HitRecord closest_hit;
			double t_max = INFINITY;
			double t_min = 0.001;
			bool hit_object = false;
			
			for(auto object : scene){
				// if we hit the object in pixel (x, y)
				if(object->hit(actual_ray, t_min, t_max, l_hrec)){
					hit_object = true;
					// more closer shape will be our limit for t, we dont care about shapes behind
					t_max = l_hrec.t;
					closest_hit = l_hrec;
				}
			}
			
			// apply light or shadow to the pixel depending of the point of intersection and the normal in that point
			// data in HitRecord -------> I * (ka + kd * max(0, n.l))
			if(hit_object){
				// if theres an object, we color the pixel with the shape color + light
				
				// trace a vector from hit point, to light and normalize it
				Vec3 li_dir = vec3_subtraction(li_position, closest_hit.p);
				Vec3 l = vec3_normal(li_dir);
				
				float li_dist = li_dir.magnitude();
				// shadow rays --------------
				Ray shadow_ray(closest_hit.p, l, true);
				bool shadow = false;
				HitRecord s_hrec;
				
				// go throught all objects to see if our shadow ray hit other object
				for(auto object : scene){
					// if we hit the object we apply shadow in that point
					if(object->hit(shadow_ray, 0.001, li_dist, s_hrec)){
						shadow = true;
						break; // we just need to know if 1 object hit
					}
				}
				
				float intensity = ka;
				
				if(!shadow){
					// normal from the hit point in the shape
					Vec3 n = closest_hit.normal;
					
					float nl_dot = vec3_dotproduct(n, l);
					// diffuse gives us a proportion for how many ligth should we have
					float diffuse = fmax(0.0f, nl_dot); // we cannot have negative lightning
					
					intensity += diffuse;
				}
				
				// we ajust the pixel color depending on the intensity
				ofColor pixel_color;
				pixel_color.r = ofClamp(closest_hit.color.r * intensity, 0, li_color[0]);
				pixel_color.g = ofClamp(closest_hit.color.g * intensity, 0, li_color[1]);
				pixel_color.b = ofClamp(closest_hit.color.b * intensity, 0, li_color[2]);
					
				colorPixels.setColor(x, y, pixel_color);
				
			}
			else{
				// if there isnt an object, we color the pixel as the background
				colorPixels.setColor(x, y, Background_color);
			}
		}
	}
	
	texColor.loadData(colorPixels);
	
	// we end time measurement
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> diff = end - start;
	r_time = to_string(diff.count());
}


//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xffffff);
	texColor.draw(0, 0, w, h);
	
	// text color
	ofSetColor(ofColor::white);
		
	// render time and hardware info displayed on screen
	string full_info = "render time: " + r_time + " s. " + "\nHardware: Apple M1 (8-Core) / 8 GB RAM";
	
	// show camera mode info on screen
	if(cam.p_mode){
		full_info += "\n" + cam_info[1];
	}
	else{
		full_info += "\n" + cam_info[0];
	}
	
	// show render time and Hardware info (string, x, y)
	ofDrawBitmapString(full_info, 10, 10);

}

//--------------------------------------------------------------
void ofApp::exit() {
	for (auto object : scene) {
		delete object;
	}
	scene.clear();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// CAMERA --------------------------------------------------
	const double p_step = 0.1; // pan steps
	const double o_step = 2.0; // orbit steps
	bool cam_changed = false;
	
	// toggle between perspective and parallel rays
	if(key == 'p'){
		// alternate mode
		cam.p_mode = !cam.p_mode;
		
		traceAll();
	}
	
	// CAMERA PAN
	
	// move up
	else if (key == 'w') {
		 cam.e = vec3_sum(cam.e, vec3_product(cam.v_vec, p_step));
		 cam.target = vec3_sum(cam.target, vec3_product(cam.v_vec, p_step));
		 cam_changed = true;
	}
	// move down
	else if (key == 's') {
		cam.e = vec3_sum(cam.e, vec3_product(cam.v_vec, -p_step));
		cam.target = vec3_sum(cam.target, vec3_product(cam.v_vec, -p_step));
		cam_changed = true;
	}
	// move left
	else if (key == 'a') {
		cam.e = vec3_sum(cam.e, vec3_product(cam.u_vec, -p_step));
		cam.target = vec3_sum(cam.target, vec3_product(cam.u_vec, -p_step));
		cam_changed = true;
	}
	// move right
	else if (key == 'd') {
		cam.e = vec3_sum(cam.e, vec3_product(cam.u_vec, p_step));
		cam.target = vec3_sum(cam.target, vec3_product(cam.u_vec, p_step));
		cam_changed = true;
	}
	// move forward
	else if (key == 'q') {
		cam.e = vec3_sum(cam.e, vec3_product(cam.w_vec, p_step));
		cam.target = vec3_sum(cam.target, vec3_product(cam.w_vec, p_step));
		cam_changed = true;
	}
	// move backward
	else if (key == 'e') {
		cam.e = vec3_sum(cam.e, vec3_product(cam.w_vec, -p_step));
		cam.target = vec3_sum(cam.target, vec3_product(cam.w_vec, -p_step));
		cam_changed = true;
	}
	
	// CAMERA ORBIT
	
	// move up
	else if (key == 'i') {
		cam.orbit_vertical(-o_step);
		cam_changed = true;
	}
	// move down
	else if (key == 'k') {
		cam.orbit_vertical(o_step);
		cam_changed = true;
	}
	// move left
	else if (key == 'j') {
		cam.orbit_horizontal(-o_step);
		cam_changed = true;
	}
	// move right
	else if (key == 'l') {
		cam.orbit_horizontal(o_step);
		cam_changed = true;
	}
	
	// reset camera
	else if (key == 'R') {
		cam.e = viewpoint;
		cam.target = Vec3(0, 0, 0);
		cam.world_up = Vec3(0, 1, 0);
		cam.updateBasis();
		traceAll();
	}
	
	// update camera movement
	 if (cam_changed) {
		 cam.updateBasis();
		 traceAll();
	 }
	
	// TRANSFORMATIONS -------------------------------------------
	
	// object transformations
	if (selectedIdx != -1) {
		const double t_step = 0.1;  // translation steps
		const double s_step = 0.1;  // scaling steps
		const double r_step = 5.0; // rotation steps
		bool changed = false;
		
		// TRANSLATION
		
		// Move to the left
		if (key == OF_KEY_LEFT) {
			scene[selectedIdx]->tx -= t_step;
			changed = true;
		}
		// Move to the right
		else if (key == OF_KEY_RIGHT) {
			scene[selectedIdx]->tx += t_step;
			changed = true;
		}
		// Move up
		else if (key == OF_KEY_UP) {
			scene[selectedIdx]->ty += t_step;
			changed = true;
		}
		// Move down
		else if (key == OF_KEY_DOWN) {
			scene[selectedIdx]->ty -= t_step;
			changed = true;
		}
		// move front
		else if (key == 'f'){
			scene[selectedIdx]->tz += t_step;
			changed = true;
		}
		// move back
		else if (key == 'b'){
			scene[selectedIdx]->tz -= t_step;
			changed = true;
		}
		
		// SCALING
		
		// Increase scale
		else if (key == 'n') {
			scene[selectedIdx]->sx += s_step;
			scene[selectedIdx]->sy += s_step;
			scene[selectedIdx]->sz += s_step;
			changed = true;
		}
		// Decrease scale
		else if (key == 'm') {
			// scale must stay positive
			scene[selectedIdx]->sx = max(0.1, scene[selectedIdx]->sx - s_step);
			scene[selectedIdx]->sy = max(0.1, scene[selectedIdx]->sy - s_step);
			scene[selectedIdx]->sz = max(0.1, scene[selectedIdx]->sz - s_step);
			changed = true;
		}
		
		// ROTATION
		
		// rotation over x
		else if (key == 'x'){
			scene[selectedIdx]->rx += r_step;
			changed = true;
		}
		// rotation over y
		else if (key == 'y'){
			scene[selectedIdx]->ry += r_step;
			changed = true;
		}
		// rotation over z
		else if (key == 'z'){
			scene[selectedIdx]->rz += r_step;
			changed = true;
		}
		
		
		// inverse rotation over x
		else if (key == 'X'){
			scene[selectedIdx]->rx -= r_step;
			changed = true;
		}
		// inverse rotation over y
		else if (key == 'Y'){
			scene[selectedIdx]->ry -= r_step;
			changed = true;
		}
		// inverse rotation over z
		else if (key == 'Z'){
			scene[selectedIdx]->rz -= r_step;
			changed = true;
		}
		
		// RESET
		
		// reset all transformations
		else if (key == 'r'){
			// reset translation
			scene[selectedIdx]->tx = scene[selectedIdx]->itx;
			scene[selectedIdx]->ty = scene[selectedIdx]->ity;
			scene[selectedIdx]->tz = scene[selectedIdx]->itz;
			
			// reset scaling
			scene[selectedIdx]->sx = scene[selectedIdx]->isx;
			scene[selectedIdx]->sy = scene[selectedIdx]->isy;
			scene[selectedIdx]->sz = scene[selectedIdx]->isz;
			
			// reset rotation
			scene[selectedIdx]->rx = scene[selectedIdx]->irx;
			scene[selectedIdx]->ry = scene[selectedIdx]->iry;
			scene[selectedIdx]->rz = scene[selectedIdx]->irz;
			
			changed = true;
		}
		
		// update transformations and re-render
		if (changed) {
			scene[selectedIdx]->Transformation();
			traceAll();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// trace a ray where mouse clicked
	Ray click_ray = cam.ray_tracing(x, y);
	HitRecord hrec;
	double t_min = 0.001;
	double t_max = INFINITY;
	int index = 0;
	selectedIdx = -1; // -1 = no object selected, <= 0 = object selected

	// check if we it an object with our "click ray"
	for (auto object : scene) {
		if (object->hit(click_ray, t_min, t_max, hrec)) {
			t_max = hrec.t;
			selectedIdx = index;
		}
		index++;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
