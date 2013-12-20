//
//  wavefrontLoader.h
//  Assignment 3
//
//  Created by Jason Paolasini on 12/20/2013.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#ifndef __Assignment_3__wavefrontLoader__
#define __Assignment_3__wavefrontLoader__

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

#include <OpenGl/OpenGL.h>

//ObjLoader
typedef struct
{
    float x;
    float y;
    float z;
}
Vector;

typedef struct
{
    int v1, v2, v3;
    int vn1, vn2, vn3;
}
Face;


class WFObject
{
private:
    // Dynamic variables to keep our object data in
    vector<Vector> vertices;
    vector<Vector> normals;
    vector<Face> faces;
    
    void parseLine(char *line);
    
    void parseVertex(char *line);
    void parseNormal(char *line);
    void parseFace(char *line);
    
public:
    WFObject();
    ~WFObject();
    
    int load(char *filename);
    void draw();
};



#endif /* defined(__Assignment_3__wavefrontLoader__) */
