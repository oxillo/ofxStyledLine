#include "ofxStyledLine.h"

// Shader is a static member
ofShader ofxStyledLine::shader;

ofxStyledLine::ofxStyledLine(/* args */){
    if( !shader.isLoaded() ){
        shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
    }
    bHasColorChanged = false;
    bHasThicknessChanged = false;
}

ofxStyledLine::~ofxStyledLine(){
}

/* void ofxStyledLine::addVertex( const ofDefaultVertexType& p ){
    ofPolyline::addVertex(p);
    //colors.push_back( ofDefaultColorType(1.0,0.5,0.5) );
    //thicknesses.push_back( 10.0);
    
    //indices = computeAdjacency(size());
}*/


void ofxStyledLine::update(){
    
}


//----------------------------------------------------------
void ofxStyledLine::setColor(ofDefaultColorType color){
    bHasColorChanged = true;
    colors.resize( size() );
    std::fill( colors.begin(), colors.end(), color );
}

//----------------------------------------------------------
std::vector< ofDefaultColorType > & ofxStyledLine::getColors(){
    bHasColorChanged = true;
    return colors;
}

//----------------------------------------------------------
const std::vector< ofDefaultColorType > & ofxStyledLine::getColors() const {
    return colors;
}

//----------------------------------------------------------
void ofxStyledLine::setThickness( float thickness ){
    bHasThicknessChanged = true;
    thicknesses.resize( size() );
    std::fill( thicknesses.begin(), thicknesses.end(), thickness );
}

//----------------------------------------------------------
std::vector< float > & ofxStyledLine::getThicknesses(){
    bHasThicknessChanged = true;
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
        ofLogError() << "hasChanged" << ofGetFrameNum(); 
        vbo.clear();
        if( pattern.size() <= 1 ){
            vbo.setVertexData( &getVertices()[0], size(), GL_DYNAMIC_DRAW );
            vbo.setIndexData( &indices[0], indices.size(), GL_DYNAMIC_DRAW );
            vbo.setColorData( &colors[0], colors.size() ,GL_DYNAMIC_DRAW );
            vbo.setAttributeData(shader.getAttributeLocation("thickness"), &thicknesses[0], 1, thicknesses.size(), GL_DYNAMIC_DRAW);
            numberOfRenderedElements = 4*(size() - 1);
        }else{
	        updatePatternVertices();
            vbo.setVertexData( &patternedVertices[0], patternedVertices.size(), GL_DYNAMIC_DRAW );
            vbo.setColorData( &patternedColor[0], patternedColor.size() ,GL_DYNAMIC_DRAW );
            vbo.setIndexData( &patternedIndices[0], patternedIndices.size(), GL_DYNAMIC_DRAW );
            vbo.setAttributeData(shader.getAttributeLocation("thickness"), &patternedThicknesses[0], 1, patternedThicknesses.size(), GL_DYNAMIC_DRAW);
            ofLogError() << "Vertices : "<< patternedVertices.size() << " - Indexes : " << patternedIndices.size() << " - Colors : " << patternedColor.size();
            numberOfRenderedElements = 4*(patternedVertices.size() - 1);
        }
        hasChanged(); // has getVertices() make ofPolyline think it was modified.
        bHasColorChanged = false;
        bHasThicknessChanged = false;
    }
    
    if( bHasColorChanged ){
        if( pattern.size() <= 1 ){
            vbo.updateColorData( &colors[0], colors.size() );
        }else{
            updatePatternColors();
            vbo.updateColorData( &patternedColor[0], patternedColor.size() );
        }
        bHasColorChanged = false;
    }
    if( bHasThicknessChanged ){
        if( pattern.size() <= 1 ){
            vbo.updateAttributeData( shader.getAttributeLocation("thickness"), &thicknesses[0], thicknesses.size() );
        }else{
            updatePatternThicknesses();
            vbo.updateAttributeData( shader.getAttributeLocation("thickness"), &patternedThicknesses[0], patternedThicknesses.size() );
        }
        bHasThicknessChanged = false;
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
    patternedFloatIndices.clear();
    patternedVertices.push_back( getVertices()[0] );
    patternedFloatIndices.push_back( 0.0 );

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
            patternedFloatIndices.push_back( float(vertexIndex) );
            if( isSolidPart ) {
                patternVertexIndices.push_back( patternedVertices.size()-1 );
            }
            vertexIndex++;
        }
        if( l > getLengthAtIndex( vertexIndex-1 )){
            patternedVertices.push_back( getPointAtLength(l) );
            patternedFloatIndices.push_back( getIndexAtLength(l) );
            if( isSolidPart ) patternVertexIndices.push_back( patternedVertices.size()-1 );
        }
        patternIndex = (patternIndex + 1)%pattern.size();
        
        for( auto & i : computeAdjacency(patternVertexIndices) ){
            patternedIndices.push_back( i );
        }
    }
    updatePatternColors();
    updatePatternThicknesses();
}

void ofxStyledLine::updateColors(){
    if( colors.empty() ) setColor( ofDefaultColorType(1.,1.,1.));
    while( colors.size() < size() ){
        colors.push_back( colors.back() );
    }
    colors.resize( size() );
}

void ofxStyledLine::updatePatternColors(bool continuous){
    updateColors();
    patternedColor.clear();
    for( auto& idx : patternedFloatIndices ){
        idx = abs(idx);  // To avoid problems with 
    
        float lo = floor( idx );
        float iIdx = size_t(lo);
        float percent = (continuous)?idx-lo:0.0;
        
        if( iIdx < size() ){
            patternedColor.push_back( (1.0 - percent)*colors[iIdx]+(percent)*colors[iIdx+1] );
        }else{
            patternedColor.push_back( colors[iIdx] );
        }
    }
}

void ofxStyledLine::updateThicknesses(){
    if( thicknesses.empty() ) setThickness( 1.);
    while( thicknesses.size() < size() ){
        thicknesses.push_back( thicknesses.back() );
    }
    thicknesses.resize( size() );
}

void ofxStyledLine::updatePatternThicknesses(bool continuous){
    updateThicknesses();
    patternedThicknesses.clear();
    for( auto& idx : patternedFloatIndices ){
        idx = abs(idx);  // To avoid problems with 
    
        float lo = floor( idx );
        float iIdx = size_t(lo);
        float percent = (continuous)?idx-lo:0.0;
        
        if( iIdx < size() ){
            patternedThicknesses.push_back( (1.0 - percent)*thicknesses[iIdx] + (percent)*thicknesses[iIdx+1] );
        }else{
            patternedThicknesses.push_back( thicknesses[iIdx] );
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

//----------------------------------------------------------
std::vector< unsigned int > ofxStyledLine::computeAdjacency( size_t nbElements ){
    std::vector< unsigned int> ind(nbElements);
    for( unsigned int i=0; i<ind.size();++i){ //0,1,2,3,... nbElements-1
        ind[i]=i;
    }
    return computeAdjacency( ind );
}