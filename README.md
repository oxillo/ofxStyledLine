ofxStyledLine
=============

Introduction
------------
ofxStyledLine is an Open Frameworks addon that extends ofPolyline to :
    - set the width/thickness of the line
    - control the colors of the line on each segment
    - control the pattern (dash, dot, )

We can use the ofPolyline functions (addVertex, lineTo, curveTo, close... ) to build the skeleton of the line. 
This skeleton is then styled with setColor, setThickness or setPattern.

ofxStyledLine use the programmable GL renderer and geometry shader to render lines.


Licence
-------
This addon is distributed under the [MIT License](https://en.wikipedia.org/wiki/MIT_License).


Installation
------------
Just drop the folder into your addons folder (generally `openFrameworks/addons/`).

Dependencies
------------
ofxStyledLine do not require any additional addon to run. However, tests use `ofxUnitTests` addon that should already be inside your addon folder.

Compatibility
------------
This addon has been tested only on OF 0.10.1 on MSYS2

Known issues
------------
ofxStyledLine is at its very early stage. Please report issues.

Version history
------------
