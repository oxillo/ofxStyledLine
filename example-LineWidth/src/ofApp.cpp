#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 50, 50);
	ofSetVerticalSync(false);
	//ofEnableAlphaBlending();

	float r = ofGetHeight()/2;

	size_t nb_segments = 7;
	for(unsigned int i=0; i<=nb_segments; i++) {
		ofPoint pt;
    	pt.set(ofRandomf() * r, ofRandomf() * r, -10.);
		points.push_back(pt);
		sline.addVertex(pt);
	}
//	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	ofColor(255);

	
	//ofSetColor(0,255,0,255);
	//sline.getColors()[0] = ofDefaultColorType(abs(cos(ofGetElapsedTimef())),0.,0.); 
	sline.setColor( ofDefaultColorType(0.0,0.5*(1+cos(ofGetElapsedTimef())),0.,0.7)); 
	sline.getColors()[2] = ofDefaultColorType(0.0,0.0,abs(cos(ofGetElapsedTimef())),1.0); 
	sline.setThickness( 20.0 );
	sline.getThicknesses()[0] = 10.0;
	sline.getThicknesses()[1] = 1.0;
	sline.draw();

	ofSetColor(255,0,0);
	for(unsigned int i=1; i<points.size(); i++) {
		ofDrawLine(points[i-1], points[i]);
	}

	ofPopMatrix();
	ofSetColor(255);

	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) + "\nPress 's' to toggle shader: " + (doShader ? "ON" : "OFF"), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if( key == 's' ){
		doShader = !doShader;
	}
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

