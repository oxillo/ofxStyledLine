#include "ofApp.h"


void ofLogoLineaStyle( ofxStyledLine& line){
	line.addVertex( ofPoint( -280.0,  120.0)); //0
	line.addVertex( ofPoint( -160.0,  120.0)); // start of O
	line.addVertex( ofPoint( -240.0,   40.0));
	line.addVertex( ofPoint( -240.0,  -40.0));
	line.addVertex( ofPoint( -160.0, -120.0));
	line.addVertex( ofPoint(  -80.0, -120.0)); //5
	line.addVertex( ofPoint(    0.0,  -40.0));
	line.addVertex( ofPoint(    0.0,   40.0));
	line.addVertex( ofPoint(  -80.0,  120.0)); // end of O
	line.addVertex( ofPoint(   40.0,  120.0)); // start of F
	line.addVertex( ofPoint(   40.0, -120.0)); //10
	line.addVertex( ofPoint(  240.0, -120.0));
	line.addVertex( ofPoint(  120.0,    0.0));
	line.addVertex( ofPoint(  180.0,    0.0));
	line.addVertex( ofPoint(  180.0,   60.0));
	line.addVertex( ofPoint(  120.0,   60.0)); //15
	line.addVertex( ofPoint(  120.0,  120.0)); //
	line.addVertex( ofPoint(  280.0,  120.0)); // end of F
	//line.addVertex( ofPoint(  200.0,  120.0)); //18
}

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(60, 60, 60);
	ofSetVerticalSync(false);

	//ofEnableAlphaBlending();

	ofLogoLineaStyle(sline);
	sline.setColor( ofDefaultColorType(1.,1.,1.)); 
	sline.setThickness(3.0);


	
	/* for( auto i=2; i<8; i++ ){
		sline.getThicknesses()[i] = 5.0;
	}
	for( auto i=10; i<16; i++ ){
		sline.getThicknesses()[i] = 1.0;
	}*/
	//
	
	/* sline.getColors()[0] = ofDefaultColorType(0.0,1.0,0.0,1.0); 
	sline.getColors()[8] = ofDefaultColorType(0.0,1.0,0.0,1.0); 
	sline.getColors()[9] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[10] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[11] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[12] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[13] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[14] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[15] = ofDefaultColorType(0.0,1.0,0.5,1.0); 
	sline.getColors()[16] = ofDefaultColorType(0.0,1.0,0.0,1.0); 
	*/

//	ofEnableDepthTest();
	state = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	if( ofGetElapsedTimef() > 2.0 ){
		ofResetElapsedTimeCounter();
		state = (state + 1) % 6;
		switch( state ){
			case 0 :
				sline.setThickness(3.0);
				sline.setColor( ofDefaultColorType(1.,1.,1.) );
				sline.setPattern( {5.0, 5.0} );
				break;
			case 1 :
				sline.setPattern( {10.0, 10.0} );
				break;
			case 2 :
				sline.setPattern( {30.0, 10.0, 10.0, 10.0} );
				break;
			case 3 :
				sline.getPattern()[2] = 20.0;
				break;
			case 4 :
				sline.setPattern( {} );
				sline.setThickness(10.0);
				break;
			case 5 :
				sline.setPattern( {30.0, 10.0} );
				sline.getColors()[0] = ofDefaultColorType(1.,0.,0.); 
				sline.getColors()[8] = ofDefaultColorType(0.,1.,0.);
				sline.getColors()[16] = ofDefaultColorType(0.,0.,1.);
				break;
			default :
				break;
		}	
	}
	
	/*if( ofGetElapsedTimef() > 6.0f ){
		sline.setPattern( {10.0, 5.0, 20.0, 5.0} );
		sline.updatePatternVertices();
		return;
	}
	if( ofGetElapsedTimef() > 4.0f ){	
		sline.getPattern()[0] = 10.0;
		sline.updatePatternVertices();
		return;
	}
	if( ofGetElapsedTimef() > 2.0f ){
		sline.setPattern( {5.0, 5.0} );
		sline.updatePatternVertices();
	}*/
	//sline.getColors()[8] = ofDefaultColorType(0.5+0.5*cos(ofGetElapsedTimef()),0.,0.);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
		sline.draw();
	ofPopMatrix();

	ofSetColor(255);
	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) , 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
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

