#pragma once

#include "ofMain.h"
#include "3DMath.h"
#include "camera.h"
#include "object.h"
#include <vector>

class ofApp : public ofBaseApp{
public:
	
	void setup();
	void update();
	void draw();
	void traceAll();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofTexture 				texColor;
	ofPixels 				colorPixels;
	
	string 					r_time; //render time
		
	// camera settings
	int 					w, h;
	Vec3					viewpoint;
	Camera					cam;
	const vector<string> 	cam_info = {"Parallel mode", "Perspective mode"};
	
	// in screen objects
	vector<Object3D*> 		scene;
	ofColor					Background_color;
	
	// object selection
	int 					selectedIdx;
	
	// lightning
	Vec3					li_position;
	ofColor					li_color;
	double					ka; // ambient lightning
};
