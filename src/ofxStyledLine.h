#pragma once

#include "graphics/ofPolyline.h"
#include "gl/ofVbo.h"
#include "gl/ofShader.h"

using patternDefinition = std::vector< float >;

class ofxStyledLine : public ofPolyline
{
private:
    static ofShader shader; // The shader to draw the styled line

    ofVbo vbo;  // The vertices array that we construct from polyline
    size_t numberOfRenderedElements;
    
    std::vector< unsigned int > indices; // Adjacency informations
    std::vector< ofDefaultColorType > colors; // Color per vertex
    std::vector< float > thicknesses; // Thickness per vertex
    patternDefinition pattern; // If empty or one element, solid line. Else alternate of solid /empty
    std::vector< ofDefaultVertexType > patternedVertices; // Include extra vertex due to stipple definition
    std::vector< unsigned int > patternedIndices;
    std::vector< float > patternedFloatIndices;  // For each vertices in patternVertices, gives the float index (for example 5.75)
    std::vector< ofDefaultColorType > patternedColor;
    std::vector< float > patternedThicknesses; // Thickness per vertex

    bool bHasColorChanged;
    bool bHasThicknessChanged;

    /// \brief Compute adjacency array from a list of vertices index
    std::vector< unsigned int > computeAdjacency( std::vector< unsigned int > vertexIndices );

    /// \brief Compute adjacency array from consecutive elements [0,1,2,3,...]
    std::vector< unsigned int > computeAdjacency( size_t nbElements );

    /// \brief makes the color array size matches the polyline size.
    /// If empty, fill with white color
    /// If too small, extend with the color of the last element
    /// If too large, discard extra elements.
    void updateColors();

    /// \brief makes the thicknesses array size matches the polyline size.
    /// If empty, fill with 1.0 thickness
    /// If too small, extend with the thickness of the last element
    /// If too large, discard extra elements.
    void updateThicknesses();

    unsigned int getPatternIndexAtLength( float length);

public:
    ofxStyledLine(/* args */);
    ~ofxStyledLine();

    void updatePatternVertices();
    void updatePatternColors( bool continuous = false );
    void updatePatternThicknesses( bool continuous = true );

    //void addVertex( const ofDefaultVertexType& p );

    /// \brief Returns whether the line has been modified (vertices, colors, ...).
	//bool hasChanged();
	//void flagHasChanged();

    /// \brief set the color of the line
    void setColor(ofDefaultColorType color);
    /// \brief Get the color for each vertex
    /// You  may access each vertex individually to change its color
    std::vector< ofDefaultColorType > & getColors();
    const std::vector< ofDefaultColorType > & getColors() const;

    /// \brief set the thickness/width of the line
    void setThickness( float thickness );
    /// \brief Get the thickness for each vertex
    /// You  may access each vertex individually to change its thickness
    std::vector< float > & getThicknesses();
    const std::vector< float > & getThicknesses() const;

    /// \brief Set the pattern. even elements are draw parts, odd elements are left blank
    /// so [2.0,2.0] results in a regular dashed line, [5.0,2.0] results in long dash
    // and [5.0,2.0,2.0,2.0] in an alternate long / short dash
    // Patterns with 0 or 1 length will result in solid line.
    void setPattern( patternDefinition patterndef );
    /// \brief Get the pattern in use
    /// You  may edit each element of the pattern with []
    patternDefinition & getPattern();
    const patternDefinition & getPattern() const;



    void update();
    void draw();

#ifdef OFXSTYLEDLINE_UNITTESTING
    const std::vector< ofDefaultVertexType >& getPatternedVertices() {
        updatePatternVertices();
        return patternedVertices;
    }
#endif

};