#pragma once

#include "graphics/ofPolyline.h"
#include "gl/ofVbo.h"
#include "gl/ofShader.h"

class ofxStyledLine : public ofPolyline
{
private:
    
    ofVbo vbo;  // The vertices array that we construct from polyline
    std::vector< unsigned int > indices; // Adjacency informations
    std::vector< ofDefaultColorType > colors; // Color per vertex
    std::vector< float > thicknesses; // Thickness per vertex
    static ofShader shader; // The shader to draw the styled line

public:
    ofxStyledLine(/* args */);
    ~ofxStyledLine();

    void addVertex( const ofDefaultVertexType& p );

    /// \brief Returns whether the line has been modified (vertices, colors, ...).
	//bool hasChanged();
	//void flagHasChanged();

    /// \brief set the color of the line
    void setColor(ofDefaultColorType color);
    /// \brief Get the color for each vertex
    /// You  may access each vertex individually to change its color
    std::vector< ofDefaultColorType > & getColors();
    const std::vector< ofDefaultColorType > & getColors() const;



    void update();
    void draw();

};