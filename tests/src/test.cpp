#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#define OFXSTYLEDLINE_UNITTESTING 1
#include "ofxStyledLine.h"

class ofApp: public ofxUnitTestsApp{
	void testPatternVertices(){
		float w = 100., h=50.;
		ofxStyledLine line;
		std::vector< ofDefaultVertexType > vertices;

		line.addVertex(0,0);
		line.addVertex(0,h);
		line.addVertex(w,h);
		line.addVertex(w,0);
		ofxTestEq(line.getPerimeter(),h+w+h,"Perimeter of non closed");
		
		line.setPattern({20.,30.});
		//line.updatePatternVertices();
		vertices = line.getPatternedVertices();
		ofxTestEq(vertices.size(),9,"{20,30} pattern shall produce 9 vertices :");

		line.setPattern({20.,20.});
		//line.updatePatternVertices();
		vertices = line.getPatternedVertices();
		ofxTestEq(vertices.size(),13,"{20,20} pattern shall produce 13 vertices :");

		line.setPattern({30.,30.});
		//line.updatePatternVertices();
		vertices = line.getPatternedVertices();
		ofxTestEq(vertices.size(),9,"{30,30} pattern shall produce 9 vertices :");

		line.setPattern({10.,10.,20.,5.});
		//line.updatePatternVertices();
		vertices = line.getPatternedVertices();
		ofxTestEq(vertices.size(),21,"{10.,10.,20.,5.} pattern shall produce 21 vertices :");
	}

	void run(){
		testPatternVertices();
	}
};

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setGLVersion(3,3);
	settings.setSize(1400,768);
	shared_ptr<ofAppBaseWindow> window =ofCreateWindow(settings);

	shared_ptr<ofApp> app(new ofApp);
    ofRunApp(window, app);
    return ofRunMainLoop();

}
