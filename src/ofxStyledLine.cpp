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
        vbo.clear();
        updatePatternVertices();
        vbo.setVertexData( &patternedVertices[0], patternedVertices.size(), GL_DYNAMIC_DRAW );
        vbo.setColorData( &patternedColor[0], patternedColor.size() ,GL_DYNAMIC_DRAW );
        vbo.setIndexData( &patternedIndices[0], patternedIndices.size(), GL_DYNAMIC_DRAW );
        vbo.setAttributeData(shader.getAttributeLocation("thickness"), &patternedThicknesses[0], 1, patternedThicknesses.size(), GL_DYNAMIC_DRAW);
        numberOfRenderedElements = 4*(patternedVertices.size() - 1);
        hasChanged(); // has getVertices() make ofPolyline think it was modified.
        bHasColorChanged = false;
        bHasThicknessChanged = false;
    }

    // Update colors if needed
    if( bHasColorChanged ){
        if( pattern.size() <= 1 ){
            vbo.updateColorData( &colors[0], colors.size() );
        }else{
            updatePatternColors();
            vbo.updateColorData( &patternedColor[0], patternedColor.size() );
        }
        bHasColorChanged = false;
    }

    // Update line thickness if needed
    if( bHasThicknessChanged ){
        if( pattern.size() <= 1 ){
            vbo.updateAttributeData( shader.getAttributeLocation("thickness"), &thicknesses[0], thicknesses.size() );
        }else{
            updatePatternThicknesses();
            vbo.updateAttributeData( shader.getAttributeLocation("thickness"), &patternedThicknesses[0], patternedThicknesses.size() );
        }
        bHasThicknessChanged = false;
    }

    // Draw the petterned lines
    shader.begin();
		vbo.drawElements( GL_LINES_ADJACENCY, numberOfRenderedElements );
	shader.end();
}

//----------------------------------------------------------
void ofxStyledLine::updatePatternVertices(){
    float l = 0.0;
    size_t vertexIndex = 0;
    size_t patternIndex = 0;
    std::vector<size_t> patternVertexIndices;
    bool isSolidPart;

    patternedVertices.clear();
    patternedIndices.clear();
    patternedFloatIndices.clear();
    
    // Compute the extremities of each dash on the line by applying the pattern until we reach the end of the line
    std::vector< float > patternedLength;
    patternedLength.clear();

    // Add all the points of the polyline
    for( vertexIndex=0; vertexIndex < size(); vertexIndex++ ){
        patternedFloatIndices.push_back( float(vertexIndex) );
        patternedLength.push_back( getLengthAtIndex(vertexIndex) );
    }
    
    // Compute extra points that are extremities of patttern dashes
    float perimeter = getPerimeter(); // Cache the perimeter value
    if( pattern.size() >=2 ){
        do{
            float fIndex = getIndexAtLength(l);
            patternedFloatIndices.push_back( fIndex );
            patternedLength.push_back(l);
            //ofLogError() << l << "  fidx " << fIndex << " point : "<< getPointAtIndexInterpolated(fIndex);
            l += pattern[patternIndex];
            patternIndex = (patternIndex + 1)%pattern.size();
        }while( l < perimeter );
    }

    // Reorder points and remove duplicates
    std::sort( patternedLength.begin(), patternedLength.end() ); 
    patternedLength.erase( std::unique( patternedLength.begin(), patternedLength.end()),
                                 patternedLength.end() );
    std::sort( patternedFloatIndices.begin(), patternedFloatIndices.end() ); 
    patternedFloatIndices.erase( std::unique( patternedFloatIndices.begin(), patternedFloatIndices.end()),
                                 patternedFloatIndices.end() );
    
    // Compute vertices from their index on the polyline
    for( auto& fidx : patternedFloatIndices ){
        patternedVertices.push_back( getPointAtIndexInterpolated(fidx) );
    }
    
    unsigned int plSize = patternedLength.size();
    for( unsigned int i=0; i<plSize-1; i++ ){
        unsigned int iminus1 = (i + plSize - 1)%plSize;
        unsigned int iplus1 = (i + plSize + 1)%plSize;
        unsigned int iplus2 = (i + plSize + 2)%plSize;

        if( getPatternIndexAtLength( patternedLength[i] )%2==0 ){
            patternedIndices.push_back(iminus1);
            patternedIndices.push_back(i);
            patternedIndices.push_back(iplus1);
            patternedIndices.push_back(iplus2);
        }
    }
    
    if( !isClosed() ){
        patternedIndices[0]=0;
        patternedIndices.back()=patternedIndices[ patternedIndices.size() - 2 ];
    }

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