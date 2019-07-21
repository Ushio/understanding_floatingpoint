#include "ofApp.h"

#include "float_binary32.hpp"
#include "peseudo_random.hpp"

using namespace binary32;

// FLOATING POINT VISUALLY EXPLAINED
// http://fabiensanglard.net/floating_point_visually_explained/

//--------------------------------------------------------------
void ofApp::setup() {
	_camera.setNearClip(0.1f);
	_camera.setFarClip(100.0f);
	_camera.setDistance(5.0f);
}
void ofApp::exit() {

}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);

	_camera.begin();

	ofSetLineWidth(5);

	ofPushMatrix();
	ofRotateYDeg(90.0f);
	ofSetColor(64);
	ofDrawGridPlane(1, 100);
	ofPopMatrix();

	ofDrawAxis(50);
	ofSetLineWidth(1);

	ofSetSphereResolution(4);
	ofSetColor(255);

	rt::Xoshiro128StarStar random;
	for (int i = 0; i < 10000; ++i) {
		float f = random.uniform(-10.0f, 10.0f);
		int32_t exp = (int32_t)get_exponent(f) - 127;
		glm::vec3 p = glm::vec3(f, exp, 0.0f);
		
		ofDrawSphere(p, 0.01f);
	}

	_camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
