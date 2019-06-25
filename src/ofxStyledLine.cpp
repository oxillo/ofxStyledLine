#include "ofxStyledLine.h"

// Shader is a static member
ofShader ofxStyledLine::shader;

ofxStyledLine::ofxStyledLine(/* args */)
{
    if( !shader.isLoaded() ){
        shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    }
}

ofxStyledLine::~ofxStyledLine()
{
}

void ofxStyledLine::addVertex( const ofDefaultVertexType& p )
{
    ofPolyline::addVertex(p);
    colors.push_back( ofDefaultColorType(1.0,0.5,0.5) );
    if( size() == 1 ) return;
    if( size() == 2 ){
        indices.push_back(0);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(1);
    }
    if( size() > 2 ){
        unsigned int last = size()-1;
        indices.back() = last;  // Update last line adjacency
        // New line adjacency
        indices.push_back( last - 2 );
        indices.push_back( last - 1 );
        indices.push_back( last );
        indices.push_back( last );
    //    vbo.updateVertexData(&getVertices()[0], size());
    //    vbo.updateIndexData(&indices[0], indices.size());
    }
}


void ofxStyledLine::update()
{
    
}


//----------------------------------------------------------
void ofxStyledLine::setColor(ofDefaultColorType color){
    flagHasChanged();
    std::fill(colors.begin(), colors.end(), color);
}

//----------------------------------------------------------
std::vector< ofDefaultColorType > & ofxStyledLine::getColors(){
    flagHasChanged();
	return colors;
}

//----------------------------------------------------------
const std::vector< ofDefaultColorType > & ofxStyledLine::getColors() const {
	return colors;
}

void ofxStyledLine::draw()
{
    if( hasChanged() ){
        vbo.setVertexData( &getVertices()[0], size(), GL_DYNAMIC_DRAW );
        vbo.setIndexData( &indices[0], indices.size(), GL_DYNAMIC_DRAW );
        vbo.setColorData( &colors[0], colors.size() ,GL_DYNAMIC_DRAW );
    }
    // size() returns the number of points. We need at least 2 point to draw a line!!
    if( size() > 1 ){
        shader.begin();
		// set thickness
		shader.setUniform1f("thickness", 20);
		vbo.drawElements(GL_LINES_ADJACENCY, 4*(size()-1));
	shader.end();
    }
    
}

