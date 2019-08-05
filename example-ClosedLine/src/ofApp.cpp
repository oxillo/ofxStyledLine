#include "ofApp.h"


void ofLogoLineaStyle( ofxStyledLine& line){
	line.addVertex( ofPoint( -280.0,  120.0)); //0
	 // end of O
	
	line.addVertex( ofPoint(  280.0,  120.0)); // end of F
	//line.addVertex( ofPoint(  200.0,  120.0)); //18
}

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(60, 60, 60);
	ofSetVerticalSync(false);

	// Draw O letter
	oLetter.addVertex( ofPoint(  -40.0,  120.0)); // start of O
	oLetter.addVertex( ofPoint( -120.0,   40.0));
	oLetter.addVertex( ofPoint( -120.0,  -40.0));
	oLetter.addVertex( ofPoint(  -40.0, -120.0));
	oLetter.addVertex( ofPoint(   40.0, -120.0)); //5
	oLetter.addVertex( ofPoint(  120.0,  -40.0));
	oLetter.addVertex( ofPoint(  120.0,   40.0));
	oLetter.addVertex( ofPoint(   40.0,  120.0));
	oLetter.close(); 
	oLetter.setColor( ofDefaultColorType(1.,1.,1.)); 
	oLetter.setThickness(10.0);

	// Draw F letter
	fLetter.addVertex( ofPoint(   40.0,  120.0)); // start of F
	fLetter.addVertex( ofPoint(   40.0, -120.0)); //10
	fLetter.addVertex( ofPoint(  240.0, -120.0));
	fLetter.addVertex( ofPoint(  120.0,    0.0));
	fLetter.addVertex( ofPoint(  180.0,    0.0));
	fLetter.addVertex( ofPoint(  180.0,   60.0));
	fLetter.addVertex( 120.0, 60.0 ); //15
	fLetter.lineTo( 120.0, 120.0 ); //
	fLetter.close();
	fLetter.setPattern({10.0,10.0});
	fLetter.setColor( ofDefaultColorType(1.,1.,1.)); 
	fLetter.setThickness(10.0);

	rectangle.addVertex( -280 ,-160 );
	rectangle.lineTo( -280 ,160 );
	rectangle.lineTo( 280 ,160 );
	rectangle.lineTo( 280 ,-160 );
	rectangle.close();
	rectangle.setColor( ofDefaultColorType(1.,.6,.6)); 
	rectangle.setThickness(20.0);
	rectangle.setPattern({10.0,10.0});

	circle.arc(ofPoint(0,0), 40, 40, 0, 360, 360);
    circle.close();
	circle.setColor( ofDefaultColorType(1.,0.,1.,0.5)); 
	circle.setThickness(25.0);
	//circle.setPattern({40.0,10.0});

	state = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	oLetter.setThickness( 5*( 1+cos(2*ofGetElapsedTimef()) ));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
		ofPushMatrix();
			ofTranslate(-120, 0, 0);
			oLetter.draw();
			circle.draw();
		ofPopMatrix();	
		fLetter.draw();
		rectangle.draw();
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

