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
	
	// Background color
	Background_color = ofColor(146, 219, 212);
	
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
	scene.push_back(new Ellipsoid(Vec3(0, 0.4, 0.4), Vec3(0.2, 0.1, 0.3), ofColor(0, 0, 255)));
	
	/*
	scene.push_back(new Plane(Vec3(0, -1.4, 0), Vec3(0, 1, 0), ofColor(0.5, 0.5, 0.5)));
	scene.push_back(new Sphere(Vec3(1, 1.2, -3), 1.0, ofColor(1, 0, 0)));
	scene.push_back(new Ellipsoid(Vec3(-5, 0, -4), Vec3(2, 3, 1), ofColor(0, 1, 0)));
	scene.push_back(new Cube(Vec3(3.5, 3, -3), 2.0, 2.0, 2.0, ofColor(0, 0, 1)));
	 */
	
	colorPixels.allocate(w, h, OF_PIXELS_RGB);
	texColor.allocate(colorPixels);
	
	// Trace all shapes for the first time
	traceAll();
}

void ofApp::traceAll(){
	// ejecution time measurement
	auto start = chrono::high_resolution_clock::now();
	
	// we go trought every pixel in screen (x, y)
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			// we make a ray for every pixel
			Ray actual_ray = cam.ray_tracing(x, y);
			
			HitRecord hrec;
			HitRecord closest_hit;
			double t_max = INFINITY;
			double t_min = 0.001;
			bool hit_object = false;
			
			for(auto object : scene){
				// if we hit the object in pixel (x, y)
				if(object->hit(actual_ray, t_min, t_max, hrec)){
					hit_object = true;
					// more closer shape will be our limit for t, we dont care about shapes behind
					t_max = hrec.t;
					closest_hit = hrec;
				}
			}
			
			if(hit_object){
				// if theres an object, we color the pixel with the shape color
				colorPixels.setColor(x, y, closest_hit.color);
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
	cout << "Render time: " << diff.count() << " seconds" << endl;
}


//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xffffff);
	texColor.draw(0, 0, w, h);
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
	if(key == 'p'){
		cout << "Projection Mode Activated " << endl;
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
