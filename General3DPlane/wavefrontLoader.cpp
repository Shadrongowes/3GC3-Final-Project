//
//  wavefrontLoader.cpp
//  Assignment 3
//
//  Created not by Jason Paolasini on 12/20/2013.
//http://www.tutorialized.com/tutorial/Write-a-WaveFront-OpenGL-3D-object-loader-in-C/59679

#include "wavefrontLoader.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>


WFObject::WFObject()
{
    
}

WFObject::~WFObject()
{
    
}

int WFObject::load(char *filename)
{
    fstream objFile;
    objFile.open(filename);
    
    if(objFile.is_open())
    {
        char line[255];
        
        // Parse object file line by line
        while(objFile.good())
        {
            objFile.getline(line, 255);
            parseLine(line);
        }
        
        objFile.close();
    }
    else
    {
        cout << "Could not open WFObject file '" << filename << "'\n";
        return false;
    }
    
    return true;
}


void WFObject::parseLine(char *line)
{
    if(!strlen(line))       // If empty string, don't do anything with it
    {
        return;
    }
    
    char *lineType;
    lineType = strtok(strdup(line), " ");
    
    // Decide what to do
    if(!strcmp(lineType, "v"))          // Vertex
    {
        parseVertex(line);
    }
    else if(!strcmp(lineType, "vn"))    // Normal
    {
        parseNormal(line);
    }
    else if(!strcmp(lineType, "f"))     // Face
    {
        parseFace(line);
    }
    
    return;
}


void WFObject::draw()
{
    glBegin(GL_TRIANGLES);
    
    for(int f = 0; f < faces.size(); f++)
    {
        glNormal3f(normals[faces[f].vn1 - 1].x, normals[faces[f].vn1 - 1].y, normals[faces[f].vn1 - 1].z);
        glVertex3f(vertices[faces[f].v1 - 1].x, vertices[faces[f].v1 - 1].y, vertices[faces[f].v1 - 1].z);
        
        glNormal3f(normals[faces[f].vn2 - 1].x, normals[faces[f].vn2 - 1].y, normals[faces[f].vn2 - 1].z);
        glVertex3f(vertices[faces[f].v2 - 1].x, vertices[faces[f].v2 - 1].y, vertices[faces[f].v2 - 1].z);
        
        glNormal3f(normals[faces[f].vn3 - 1].x, normals[faces[f].vn3 - 1].y, normals[faces[f].vn3 - 1].z);
        glVertex3f(vertices[faces[f].v3 - 1].x, vertices[faces[f].v3 - 1].y, vertices[faces[f].v3 - 1].z);
    }
    
    glEnd();
}

