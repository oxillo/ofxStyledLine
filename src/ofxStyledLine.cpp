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
    thicknesses.push_back( 10.0);
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
    }
}


void ofxStyledLine::update()
{
    
}


//----------------------------------------------------------
void ofxStyledLine::setColor(ofDefaultColorType color){
    flagHasChanged();
    std::fill( colors.begin(), colors.end(), color );
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

//----------------------------------------------------------
void ofxStyledLine::setThickness( float thickness ){
    flagHasChanged();
    std::fill( thicknesses.begin(), thicknesses.end(), thickness );
}

//----------------------------------------------------------
std::vector< float > & ofxStyledLine::getThicknesses(){
    flagHasChanged();
    return thicknesses;
}

//----------------------------------------------------------
const std::vector< float > & ofxStyledLine::getThicknesses() const{
    return thicknesses;
}

//----------------------------------------------------------
void ofxStyledLine::setPattern( patternDefinition patterndef ){
    pattern = patterndef;
    flagHasChanged();
}

//----------------------------------------------------------
patternDefinition& ofxStyledLine::getPattern(){
    flagHasChanged();
    return pattern;
}

//----------------------------------------------------------
const patternDefinition& ofxStyledLine::getPattern() const{
    return pattern;
}



//----------------------------------------------------------
void ofxStyledLine::draw(){
    if( size() <= 1) return;  // size() returns the number of points. We need at least 2 point to draw a line!!
    if( hasChanged() ){
        if( pattern.size() <= 1 ){
            vbo.setVertexData( &getVertices()[0], size(), GL_DYNAMIC_DRAW );
            vbo.setIndexData( &indices[0], indices.size(), GL_DYNAMIC_DRAW );
            vbo.setColorData( &colors[0], colors.size() ,GL_DYNAMIC_DRAW );
            vbo.setAttributeData(shader.getAttributeLocation("thickness"), &thicknesses[0], 1, thicknesses.size(), GL_DYNAMIC_DRAW);
            numberOfRenderedElements = 4*(size() - 1);
        }else{
            vbo.setVertexData( &patternedVertices[0], patternedVertices.size(), GL_DYNAMIC_DRAW );
            vbo.setColorData( &patternedColor[0], patternedColor.size() ,GL_DYNAMIC_DRAW );
            vbo.setIndexData( &patternedIndices[0], patternedIndices.size(), GL_DYNAMIC_DRAW );
            vbo.setAttributeData(shader.getAttributeLocation("thickness"), &patternedThicknesses[0], 1, patternedThicknesses.size(), GL_DYNAMIC_DRAW);
            numberOfRenderedElements = 4*(patternedVertices.size() - 1);
        }
    }
    
    shader.begin();
		vbo.drawElements( GL_LINES_ADJACENCY, numberOfRenderedElements );
	shader.end();
}

//----------------------------------------------------------
void ofxStyledLine::updatePatternVertices(){
    float l = 0.0;
    size_t vertexIndex = 1;
    size_t patternIndex = 0;
    std::vector<size_t> patternVertexIndices;
    bool isSolidPart;

    patternedVertices.clear();
    patternedIndices.clear();
    patternedColor.clear();
    patternedVertices.push_back( getVertices()[0] );

    while( l < getPerimeter() ){ // Perimeter is also the length for a non close polyline...
        l += pattern[patternIndex];
        l = ofClamp( l, 0, getPerimeter() );

        patternVertexIndices.clear();
        isSolidPart = ((patternIndex+1) % 2);  // Solid part is for even indexes : index=2n => (2n+1)%2 = 1 => true
        if( isSolidPart ) {
            patternVertexIndices.push_back( patternedVertices.size()-1 );
        }
        
        while( (vertexIndex < size()) 
            && (l >= getLengthAtIndex( vertexIndex)) ){
            patternedVertices.push_back( getVertices()[vertexIndex] );
            patternedColor.push_back(ofDefaultColorType(0.0,1.0,0.5));
            patternedThicknesses.push_back(10.0);
            if( isSolidPart ) {
                patternVertexIndices.push_back( patternedVertices.size()-1 );
            }
            vertexIndex++;
        }
        if( l > getLengthAtIndex( vertexIndex-1 )){
            patternedVertices.push_back( getPointAtLength(l) );
            patternedColor.push_back(ofDefaultColorType(0.0,1.0,0.5));
            patternedThicknesses.push_back(10.0);
            if( isSolidPart ) patternVertexIndices.push_back( patternedVertices.size()-1 );
        }
        patternIndex = (patternIndex + 1)%pattern.size();
        
        for( auto & i : computeAdjacency(patternVertexIndices) ){
            patternedIndices.push_back( i );
        }
    }    
}

//----------------------------------------------------------
std::vector< unsigned int > ofxStyledLine::computeAdjacency( std::vector< unsigned int > vertexIndices ){
    // from definition of line A-B-C-D, it should return A-A-B-C, A-B-C-D, B-C-D-D
    
    std::vector< unsigned int > adjacency;
    // We need at least 2 vertices to produce an adjacency array, return an empty array if not enough  vertices
    if( vertexIndices.size() < 2 ) return adjacency;

    // for n vertices, we should produce n-1 line adjacencies
    for( size_t i=0; i<vertexIndices.size()-1; i++){
        adjacency.push_back( (i>0)?vertexIndices[i-1]:vertexIndices[0] );
        adjacency.push_back( vertexIndices[i] );
        adjacency.push_back( vertexIndices[i+1] );
        adjacency.push_back( (i+2<vertexIndices.size())?vertexIndices[i+2]:vertexIndices[i+1] );
    }
    return adjacency;
}