//
//  Particle.cpp
//  C++ Tutorial
//
//  Created by Jason Paolasini on 2013-09-29.
//  Copyright (c) 2013 Jason Paolasini. All rights reserved.
//

#include "Car.h"
#include "3DMathLib.h"


vehicle::vehicle(){
    
}
/*The particle class was created to keep track of the attributes each particle has in the system
 */

vehicle::vehicle(point3D inPoint,char inShape,float xScale,float yScale,float zScale,char inMaterial,boundingBox inBoundingBox,bool isSelected){
    location = inPoint;
    scaleX = xScale;
    scaleY = yScale;
    scaleZ = zScale;
    shape = inShape;
    material = inMaterial;
    box = inBoundingBox;
    selected = isSelected;
    
    
    
    
}

