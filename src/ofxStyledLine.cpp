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
        updatePatternVertices();
        vbo.setColorData( &patternedColor[0], patternedColor.size() ,GL_DYNAMIC_DRAW );
        vbo.setAttributeData(shader.getAttributeLocation("thickness"), &patternedThicknesses[0], 1, patternedThicknesses.size(), GL_DYNAMIC_DRAW);
        hasChanged(); // has getVertices() make ofPolyline think it was modified.
        bHasColorChanged = false;
        bHasThicknessChanged = false;
    }

    // Update colors if needed
    if( bHasColorChanged ){
        updatePatternColors();
        vbo.updateColorData( &patternedColor[0], patternedColor.size() );
        bHasColorChanged = false;
    }

    // Update line thickness if needed
    if( bHasThicknessChanged ){
        updatePatternThicknesses();
        vbo.updateAttributeData( shader.getAttributeLocation("thickness"), &patternedThicknesses[0], patternedThicknesses.size() );
        bHasThicknessChanged = false;
    }

    // Draw the petterned lines
    shader.begin();
		vbo.drawElements( GL_LINES_ADJACENCY, 4*numberOfRenderedElements );
	shader.end();
}

//----------------------------------------------------------
void ofxStyledLine::updatePatternVertices(){
    const float totalLength = getPerimeter(); // Cache the length (or perimeter) of the line
    bool isSolid = (pattern.size() < 2);

    
    // Compute the extremities of each dash on the line by applying the pattern until we reach the end of the line
    std::vector< float > patternedLength;
    patternedLength.clear();

    // Add all the points of the polyline. 
    // Recompute the last index from the perimeter as it may be different if the line is closed (+1) or not.
    size_t indexMax = size_t( getIndexAtLength( totalLength ) );
    for( size_t vertexIndex=0; vertexIndex <= indexMax; vertexIndex++ ){
        patternedLength.push_back( getLengthAtIndex(vertexIndex) );
    }
    if( isClosed() ){
        patternedLength.push_back( totalLength );  // on closed line, getLengthAtIndex() returns 0 at the closing point, we want the perimeter..
    }
    
    // Compute point positions for every dash in the pattern
    // Apply the pattern until the end of the line is reached
    if( !isSolid ){
        float l = 0.0;
        size_t patternIndex = 0;
        do{
            patternedLength.push_back(l);
            l += pattern[patternIndex];
            patternIndex = (patternIndex + 1)%pattern.size();
        }while( l < totalLength );
    }

    // Reorder point positions  and remove duplicates
    std::sort( patternedLength.begin(), patternedLength.end() ); 
    patternedLength.erase( std::unique( patternedLength.begin(), patternedLength.end()),
                                 patternedLength.end() );
    
    // Compute vertices and float indices from their position (length) on the polyline
    patternedVertices.clear();
    patternedFloatIndices.clear();
    for( auto& l : patternedLength ){
        auto fidx = getIndexAtLength(l);
        patternedFloatIndices.push_back(fidx);
        patternedVertices.push_back( getPointAtIndexInterpolated(fidx) );
    }
    
    unsigned int plSize = patternedLength.size();
    patternedIndices.clear();
    
    auto nbSegments = plSize-1;
    bool bLoop = isClosed();
    // First segment
    if( getPatternIndexAtLength( patternedLength[0] )%2==0 ){
        patternedIndices.push_back(bLoop?nbSegments-1:0);
        patternedIndices.push_back(0);
        patternedIndices.push_back(1);
        patternedIndices.push_back(2);
    }
    // Intermediate segments
    for( unsigned int i=1; i<=nbSegments-3; i++ ){
        if( getPatternIndexAtLength( patternedLength[i] )%2==0 ){
            patternedIndices.push_back(i - 1);
            patternedIndices.push_back(i);
            patternedIndices.push_back(i + 1);
            patternedIndices.push_back(i + 2);
        }
    }
    // Segment before last
    if( getPatternIndexAtLength( patternedLength[nbSegments-2] )%2==0 ){
        patternedIndices.push_back(nbSegments-3);
        patternedIndices.push_back(nbSegments-2);
        patternedIndices.push_back(nbSegments-1);
        patternedIndices.push_back(bLoop?0:nbSegments);
    }
    // Last segment
    if( getPatternIndexAtLength( patternedLength[nbSegments-1] )%2==0 ){
        patternedIndices.push_back(nbSegments-2);
        patternedIndices.push_back(nbSegments-1);
        patternedIndices.push_back(bLoop?0:nbSegments); // in closed loop, this is the same as push_back(0) as last vertice = first vertice
        patternedIndices.push_back(bLoop?1:nbSegments);
    }

    // Update the lines definition in the vertex buffer
    vbo.clear();
    vbo.setVertexData( &patternedVertices[0], patternedVertices.size(), GL_DYNAMIC_DRAW );
    vbo.setIndexData( &patternedIndices[0], patternedIndices.size(), GL_DYNAMIC_DRAW );
    numberOfRenderedElements = nbSegments;

    updatePatternColors();
    updatePatternThicknesses();
}

//----------------------------------------------------------
void ofxStyledLine::updateColors(){
    if( colors.empty() ) setColor( ofDefaultColorType(1.,1.,1.));
    while( colors.size() < size() ){
        colors.push_back( colors.back() );
    }
    colors.resize( size() );
}

//----------------------------------------------------------
void ofxStyledLine::updatePatternColors(bool continuous){
    updateColors();
    patternedColor.clear();
    for( auto& idx : patternedFloatIndices ){
        idx = abs(idx);  // To avoid problems with first index that may be slightlty below 0
    
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

//----------------------------------------------------------
void ofxStyledLine::updateThicknesses(){
    if( thicknesses.empty() ) setThickness( 1.);
    while( thicknesses.size() < size() ){
        thicknesses.push_back( thicknesses.back() );
    }
    thicknesses.resize( size() );
}

//----------------------------------------------------------
void ofxStyledLine::updatePatternThicknesses(bool continuous){
    updateThicknesses();
    patternedThicknesses.clear();
    for( auto& idx : patternedFloatIndices ){
        idx = abs(idx);  // To avoid problems with 
    
        float lo = floor( idx );
        size_t iIdx = size_t(lo);
        size_t nextIdx = (iIdx+1)%size();
        float percent = (continuous)?idx-lo:0.0;
        
        //if( iIdx < size() ){
            patternedThicknesses.push_back( (1.0 - percent)*thicknesses[iIdx] + (percent)*thicknesses[nextIdx] );
        //}else{
            //patternedThicknesses.push_back( thicknesses[iIdx] );
        //}
    }
    if( isClosed() ) patternedThicknesses.push_back(thicknesses[0]);
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

//----------------------------------------------------------
unsigned int ofxStyledLine::getPatternIndexAtLength( float length){
    if( pattern.size() <2 ) return 0;
    
    // Compute pattern length
    float patternLength = 0;
    for( auto& l : pattern ){
        patternLength += l;
    }
    while( length >= patternLength ){
        length -= patternLength;
    }
    
    patternLength = 0;
    for( unsigned int idx = 0; idx < pattern.size(); idx++){
        patternLength += pattern[idx];
        if( patternLength > length ){
            return idx;
        }
    }
    return 0;
}