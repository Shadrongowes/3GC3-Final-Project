/*Who Loves Ray Casting? by Jason Paolasini
 Student Number 1162915
 3GC3 Assignment 3
 */


#include <stdlib.h>
//#include <GL/glut.h>
//#include <GL/glu.h>
//#include <GL/gl.h>
#include <GLUT/glut.h>
//#include <OpenGL/OpenGL.h>
#include "3DMathLib.h"
#include <math.h>
#include <iostream>
#include <vector>
#include "Object.h"
#include <ctime>
#include <time.h>
#include "Car.h"

#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}




/*Important*** Particle lifespan is not time based,In order to keep a fluent animation
 the lifespan of a particle is based on its Y value or Bounce count. Bomb fragments are 
 based on bounce count.
 */



//Global Variables


float camPos[] = {0, 10, 30};
point3D origin(0,0,0);
vec3D normalPlane(0,1,0);
char itemToMake[]={'c','s','t','o','y'};
char currentItem = 'c';
int item = 1;
bool carSelect = true;
int carRotation = 0;


//Lighting Values

float light_pos[] = {0.0, 20.0, 150.0, .10};
float light_pos1[] = {-40.0,20.0,130.0,0.10};

float amb0[4]  = {1, 1, 1, 1};
float diff0[4] = {1, 1, 1, 1};
float spec0[4] = {1, 1, 1, 1};

// Mouse Ray

ray mouseRay;


/*Using Lighting, it was important to store the materials that we would be using in a type of datastore
 The Struct was used to store all material properties*/

char currentMaterial = 'g';

typedef struct materialStruct {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
} materialStruct;

materialStruct redPlastic = {
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    {0.0, 0.0, 0.0, 0.0},
    32.0};

materialStruct water = {
    {0.0,0.0,0.3},
    {0.0,0.0,0.6},
    {0.6,0.6,0.8},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct blackPlastic = {
    {0.03,0.03,0.03},
    {0.4,0.4,0.4},
    {0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct whitePlastic = {
    {0.3,.3,0.3},
    {0.9,0.9,0.9},
    {0.1,0.7,0.7},
    {0.0,0.0,0.0,0.0},
    100.0};
materialStruct yellowPlastic = {
    {1,1,0},
    {0.9,0.9,0.9},
    {0.1,0.7,0.7},
    {0.0,0.0,0.0,0.0},
    100.0};

materialStruct gold = {
    {0.33, 0.22, 0.03, 1.0},
    {0.78, 0.57, 0.11, 1.0},
    {0.0, 0.0, 0.0, 0.0},
    {0.99, 0.91, 0.81, 1.0},
    100.8};







/*Generate the vector that stores all the particles. A vector was used because it is a dynamic datastructure that
can be modified as we add particles*/

std::vector<object> cars;



boundingBox standardBox(){
    
    //initialize the norms for all of the planes
    
    vec3D rightN(-1,0,0);
    vec3D leftN(1,0,0);
    vec3D topN(0,1,0);
    vec3D bottomN(0,-1,0);
    vec3D frontN(0,0,1);
    vec3D backN(0,0,-1);
    
    //initialize origins
    
    point3D rightO(0.5,0,0);
    point3D leftO(-0.5,0,0);
    point3D topO(0,0.5,0);
    point3D bottomO(0,-0.5,0);
    point3D frontO(0,0,0.5);
    point3D backO(0,0,-0.5);
    
    

    plane3D rightP(rightN,rightO);
    
    plane3D leftP(leftN,leftO);
    
    plane3D topP(topN,topO);
    
    plane3D bottomP(bottomN,bottomO);
    
    plane3D frontP(frontN,frontO);
    
    plane3D backP(backN,backO);
    
    boundingBox box(frontP, backP, leftP, rightP, topP, bottomP);

    
    return box;
}




object createObject(){
    
    object newObject(origin, currentItem, 1,1,1, 'b',standardBox(), true);

    
    return newObject;
}





void notesToTheTA(void){
    
    printf("Use MOUSE to CLick cars Press a to add your first object\n-------------------------------------------\nup= Camera Pan +Z\ndown= Camera Pan -Z\nleft= Camera Pan -X\nright= Camera Pan +X\nF1= Camera Pan +Y\nF2= Camera Pan -Y\nq to quit\nr = Reset\ny=translation up\nalt+y=translation down\nx=translation right\nalt+x=translation left\n\nz=translation forwards\nalt+z=translation into the screen\n\n-------------------------------------------\nScaling\ni=positive x\nalt+i = negative x\nj=positive y\nalt+j = negative y\n\nk=positive z\nalt+k = negative z\n\nChange Material colours numbers 1-5\n Change Shape od selected object SPACE BAR\n-------------------------------------------\n6= light Pan +X\n7= Light Pan -X\n8= Light Pan -Y\n9= Light Pan +Y\n0= Light Pan +Z\n-= Camera Pan -Z\n)");}



void deselectAll(){
    
    for(int i = 0; i<cars.size();i++){
        cars[i].selected = false;
    }
}



void timer(int id){
    glutPostRedisplay();
}





object translateFunction(object vehicle,float x, float y, float z){
    
   
     
            
            vec3D trans(x,y,z);
            
            vehicle.location = vehicle.location.shift(trans);
            
            
            //shift all bounding boxes
            vehicle.box.left.origin =  vehicle.box.left.origin.shift(trans);
            vehicle.box.right.origin = vehicle.box.right.origin.shift(trans);
            vehicle.box.top.origin =  vehicle.box.top.origin.shift(trans);
            vehicle.box.bottom.origin = vehicle.box.bottom.origin.shift(trans);
            vehicle.box.back.origin = vehicle.box.back.origin.shift(trans);
            vehicle.box.front.origin = vehicle.box.front.origin.shift(trans);
            
    return vehicle;
    
    
    
    
}
void scaleFunction(float x, float y, float z){
    
    for(int i = 0;i<cars.size();i++){
        if(cars[i].selected){
            
             cars[i].scaleX = cars[i].scaleX+x;
             cars[i].scaleY = cars[i].scaleY+y;
             cars[i].scaleZ = cars[i].scaleZ+z;
            
            
            
            //shift all bounding boxes
            //this is done by calculating the distance between the center of the object and the far edges
            //This value is then multiplied by half the scaling factor to increase the bounding box size
            
            
            cars[i].box.left.origin = cars[i].location.shift(createVector(cars[i].location, cars[i].box.left.origin).scalarMultiply(1+x));
            
            cars[i].box.right.origin = cars[i].location.shift(createVector(cars[i].location, cars[i].box.right.origin).scalarMultiply(1+x));
            
            cars[i].box.bottom.origin = cars[i].location.shift(createVector(cars[i].location, cars[i].box.bottom.origin).scalarMultiply(1+y));
            
            cars[i].box.top.origin = cars[i].location.shift(createVector(cars[i].location, cars[i].box.top.origin).scalarMultiply(1+y));
            
            cars[i].box.front.origin = cars[i].location.shift(createVector(cars[i].location, cars[i].box.front.origin).scalarMultiply(1+z));
            
            cars[i].box.back.origin = cars[i].location.shift(createVector(cars[i].location, cars[i].box.back.origin).scalarMultiply(1+z));
            
        
        }
    }
    
    
}


void rotationRunction(float x,float y, float z,float angle){
    
    
    
}




void clearcars(object object1,int i){
    
    
    cars.erase(cars.begin()+i);
    
    
}






void reset(){
    for(int i = 0; i<cars.size();i++){
        
        clearcars(cars[i], i);
        
    
    //reinitialize global variables
        light_pos1[0] =-40.0;
        light_pos1[1]=20.0;
        light_pos1[2]=130.0;
    camPos[0] =20;
    camPos[1] =10;
    camPos[2] =30;
    }
    
}






//All keyboard controls


void keyboard(unsigned char key, int x, int y)
{
    
    int mod  = glutGetModifiers();

	if (key=='q' || key==27)
        {
			exit (0);
            
        }
			
    if(key ==32){
        if(item<5){
        currentItem =itemToMake[item];
            item++;}
        else{
            item = 0;
        }
        
        
    }
    
    //Generates a new object
    
    else if(key=='a'){
            deselectAll();
            object newObj = createObject();
            cars.push_back(newObj);
        }
    else if(key=='x'){
        
        if(mod == GLUT_ACTIVE_ALT){
           
        }
        
        else{
          
        }
    }
        else if(key == 'y'){
            
            if(mod == GLUT_ACTIVE_ALT){
              
            }
            
            else{
              
            }
        }

        else if(key == 'z'){
            
            if(mod == GLUT_ACTIVE_ALT){
                
            }
            
            else{
               
            }
        }
        else if(key == 'i'){
            
            if(mod == GLUT_ACTIVE_ALT){
                scaleFunction(-0.1, 0, 0);
            }
            
            else{
                scaleFunction(0.1, 0, 0);
            }
        }
        else if(key == 'j'){
            
            if(mod == GLUT_ACTIVE_ALT){
                scaleFunction(0, -0.1, 0);
            }
            
            else{
                scaleFunction(0, 0.1, 0);
            }
        }
        else if(key == 'k'){
            
            if(mod == GLUT_ACTIVE_ALT){
                scaleFunction(0, 0, -0.1);
            }
            
            else{
                scaleFunction(0, 0, 0.1);
            }
        }
     else if(key == 'r'){
         while (cars.size()>0){
         reset();
        
         }
        }
     else if(key == '1'){
         currentMaterial = 'r';
         
     }
    
     else if(key == '2'){
         currentMaterial = 'w';
         
     }
     else if(key == '3'){
         
         currentMaterial ='g';
     }
     else if(key == '4'){
         currentMaterial ='b';
         
         
     }
     else if(key == '5'){
         
         currentMaterial = 'B';
     }
     else if(key == '6'){
        light_pos1[0]=light_pos1[0]+5;
         light_pos[0]=light_pos[0]+5;
         
     }
     else if(key == '7'){
         light_pos1[0]=light_pos1[0]-5;
         light_pos[0]=light_pos[0]-5;
         
     }
     else if(key == '8'){
         
         light_pos1[1]=light_pos1[1]+5;
         light_pos[1]=light_pos[1]+5;
     }
     else if(key == '9'){
         light_pos1[1]=light_pos1[1]-5;
         light_pos[1]=light_pos[1]-5;
         
     }
     else if(key == '0'){
         
         light_pos1[2]=light_pos1[2]+5;
         light_pos[2]=light_pos[2]+5;
     }
     else if(key == '-'){
         
         light_pos1[2]=light_pos1[2]-5;
         light_pos[2]=light_pos[2]-5;
     }

}








void init(void)
{
	glClearColor(0.25, 0.25, 0.25, 0); //sets backgorund to grey
	//---------------------------------------------
    glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage);
 //---------------------------------------------------------------


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 500);

    
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec0);
   
    
}




// In-order to quickly call material changes, The glMaterial calls are quickly accessed by key value

void setMaterial(char key){
    
    switch (key) {
        case 'r':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,redPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, redPlastic.shininess);
            
            break;
            
        case 'b':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,water.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, water.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, water.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, water.shininess);
            
            break;
            
        case 'B':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,blackPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, blackPlastic.shininess);
            
            break;
            
        case 'w':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,whitePlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whitePlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whitePlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, whitePlastic.shininess);
            
            break;
        
        case 'y':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,yellowPlastic.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowPlastic.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowPlastic.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,yellowPlastic.shininess);
            
            break;
            
            case 'g':
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,gold.ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold.diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold.specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,gold.shininess);
            break;
        
        default:
            break;
    }
    
    
    
    
}



//Used an online tutorial to build this method to creat the ray vector
//http://nehe.gamedev.net/article/using_gluunproject/16013/

vec3D GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return vec3D(posX, posY, posZ);
    

}




void drawRoom(){
    setMaterial('w');
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, 1, 0); //set the normal for stage lighting
    
    
    
   glVertex3f(500, -3, -500 );
    glVertex3f(500, -3, 500 );
    glVertex3f(-500,-3, 500 );
    glVertex3f(-500, -3, -500 );
    
    glEnd();
    
   glNormal3f(-0.707, 0, 0);
    glBegin(GL_QUADS);
    
    glVertex3f(500,5,-500);
    glVertex3f(500,-3,-500);
    glVertex3f(500,-3,500);
    glVertex3f(500,5,500);
    glEnd();
    
    glNormal3f(0.707, 0, 0);
    
    glBegin(GL_QUADS);
    
    glVertex3f(-500,5,-500);
    glVertex3f(-500,-3,-500);
    glVertex3f(-500,-3,500);
    glVertex3f(-500,5,500);
    glEnd();
    
    glNormal3f(0, 0, 0.707);
    
    glBegin(GL_QUADS);
    
    glVertex3f(500,5,-500);
    glVertex3f(500,-3,-500);
    glVertex3f(-500,-3,-500);
    glVertex3f(-500,5,-500);
    glEnd();
}











void special(int key, int x, int y)
{
	//arrow key presses move the camera
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=2.5;
			break;
            
		case GLUT_KEY_RIGHT:
			camPos[0]+=2.5;
			break;
            
		case GLUT_KEY_UP:
			camPos[2] -= 2.5;
			break;
            
		case GLUT_KEY_DOWN:
			camPos[2] += 2.5;
			break;
            
		case GLUT_KEY_F1:
			camPos[1] += 2.5;
			break;
            
		case GLUT_KEY_F2:
			camPos[1] -= 2.5;
			break;
        
            
	}
	
}



//Continually generates particles at the users selected rate set by ballCount

void idle(){

    

    
}



point3D intersectionPoint(plane3D aPlane, ray aRay){
    
    point3D origin(0,0,0);
    point3D pointInt(NULL,NULL,NULL);
    
    


    
    if (aRay.p1.dot(aPlane.norm)!=0){
 point3D pointInt;
        
        point3D p1(aRay.p1.x,aRay.p1.y,aRay.p1.z);
    
    
        pointInt = aRay.p0.shift(createVector(aRay.p0, p1).scalarMultiply(((aPlane.norm.dot(createVector(aRay.p0, aPlane.origin)))/(aPlane.norm.dot(createVector(aRay.p0,p1))))));
       
        return pointInt;
        
    }
    
    else{
       
    
         return pointInt;
    }
    
    
}



//In order to keep track of all intersection planes they were subdivided, rather than overwriting them
//This was done to debug, and work with the plane intersection test

bool intersectObject(object object, ray aRay){
    
    
    
    bool intersection = false;
    point3D pointOfIntersection1;
    point3D pointOfIntersection2;
    point3D pointOfIntersection3;
    point3D pointOfIntersection4;
    point3D pointOfIntersection5;
    point3D pointOfIntersection6;
    
  
   
    pointOfIntersection1 = intersectionPoint(object.box.front, aRay);
    pointOfIntersection2 = intersectionPoint(object.box.back, aRay);
    pointOfIntersection3 = intersectionPoint(object.box.bottom, aRay);
    pointOfIntersection4 = intersectionPoint(object.box.top, aRay);
    pointOfIntersection5 = intersectionPoint(object.box.right, aRay);
    pointOfIntersection6 = intersectionPoint(object.box.left, aRay);

    
    

        //checks front face against x and y bounds
    if((pointOfIntersection1.x<=object.box.right.origin.x) && (pointOfIntersection1.x>=object.box.left.origin.x) && (pointOfIntersection1.y>=object.box.bottom.origin.y) && (pointOfIntersection1.y<=object.box.top.origin.y))
    
    {
        
      
        intersection = true;
    }
    
    //checks back face

    
    

    else if((pointOfIntersection2.x<object.box.right.origin.x) && (pointOfIntersection2.x>object.box.left.origin.x) && (pointOfIntersection2.y>object.box.bottom.origin.y) && (pointOfIntersection2.y<object.box.top.origin.y))
    {
                intersection = true;
        
    }
    
    //checks bottom face
   

    else if((pointOfIntersection3.x<object.box.right.origin.x) && (pointOfIntersection3.x>object.box.left.origin.x) && (pointOfIntersection3.z<object.box.front.origin.z) && (pointOfIntersection3.z>object.box.back.origin.z))
    {
        intersection = true;
         
    }
    
   // checks top face

    
   else if((pointOfIntersection4.x<object.box.right.origin.x) && (pointOfIntersection4.x>object.box.left.origin.x) && (pointOfIntersection4.z<object.box.front.origin.z) && (pointOfIntersection4.z>object.box.back.origin.z))
    {
        intersection = true;
       
        
    }
    
    
    
    // checks right face
    
    
      else if((pointOfIntersection5.y<object.box.top.origin.y) && (pointOfIntersection5.y>object.box.bottom.origin.y) && (pointOfIntersection5.z<object.box.front.origin.z) && (pointOfIntersection5.z>object.box.back.origin.z))
    {
        intersection = true;
        
    }
    
    
    
    else if((pointOfIntersection6.y<object.box.top.origin.y) && (pointOfIntersection6.y>object.box.bottom.origin.y) && (pointOfIntersection6.z<object.box.front.origin.z) && (pointOfIntersection6.z>object.box.back.origin.z))
    {
        intersection = true;
        
    }
    

    
    else{
        intersection = false;
        
    }
  
    return intersection;

}




void mouse(int btn, int state, int x, int y){
    
        vec3D rayVec = GetOGLPos(x, y);
        point3D mouseScreen(camPos[0],camPos[1],camPos[2]);
        ray ray1(mouseScreen, rayVec);
    
    
    if(btn==GLUT_LEFT_BUTTON)
    {
       
        
        //Checks to see if there are any intersections with the resulting planes of the cars
        deselectAll();
        
        for(int i = 0;i<cars.size();i++){
            
            
            if(intersectObject(cars[i], ray1)){
                
                cars[i].selected =true;
                
                }
            
            else{
                    cars[i].selected =false;
                
           }
       }
    }
    
    
    else if(btn == GLUT_RIGHT_BUTTON && cars.size()>=1){
        
        
        
        for(int i = 0;i<cars.size();i++){
            
            if(intersectObject(cars[i], ray1)){
                cars.erase(cars.begin()+i);
                
            }
            
            else{
                
                
            }
        }

    }
    
    else if(state==!GLUT_DOWN){
        
    }
    else{
        
    }
    
}


void isSelected(){
    
    for(int i =0;i<cars.size();i++){
 
    
    
    }
}

void createCars(){
    
    object newObj(origin, currentItem, 1,1,1, 'b',standardBox(), false);
    cars.push_back(newObj);
    cars[0] = translateFunction(cars[0], -10, 0,0);
    
    object newObj2(origin, currentItem, 1,1,1, 'b',standardBox(), false);
    cars.push_back(newObj2);
   cars[1] = translateFunction(cars[1], 10, 0,0);
    
    object newObj3(origin, currentItem, 1,1,1, 'b',standardBox(), false);
    cars.push_back(newObj3);
  
}


        
        


//The timerFunc is set to redisplay every 5ms




    




void display(void){
    
if(carSelect){
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
        //---------------------------------------------
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texName);
        glDisable(GL_TEXTURE_2D);
        //-----------------------------------------------------------
        carRotation++;
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT1,GL_POSITION,light_pos1);
        
        
        
        glutKeyboardFunc(keyboard);
        
        if (cars.size()==0){
            createCars();
        }
        
        for(int i = 0; i<cars.size();i++){
            glPushMatrix();
            glTranslatef(cars[i].location.x,cars[i].location.y , cars[i].location.z);
            glRotatef(carRotation, 0, 1, 0);
            glutSolidCube(1);
            
            
            
            
            if(cars[i].selected){
                glutWireCube(cars[i].scaleX*2);
                
            }
            else{
                setMaterial(cars[i].material);
                
            }
            glPopMatrix();
            
        }
        
        
        //Used to switch the current targets shape a switch command was used to be able to easily change
        //Shapes
        
        
        glutSwapBuffers();
        glutTimerFunc(20,timer,0);
        
        
        
        
        
        
    }
    
    else{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//---------------------------------------------
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(500, -2.5, -100); //8+72=80,
	glTexCoord2f(0.0, 1.0); glVertex3f(500, -2.5, 100);
	glTexCoord2f(1.0, 1.0); glVertex3f(340,-2.5, 100 );//-8+72
	glTexCoord2f(1.0, 0.0); glVertex3f(340, -2.5, -100 );

	/*glTexCoord2f(0.0, 0.0); glVertex4f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex4f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex4f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex4f(2.41421, -1.0, -1.41421);*/
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
   //-----------------------------------------------------------


    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1,GL_POSITION,light_pos1);
    
    drawRoom();
    
    glutKeyboardFunc(keyboard);
    
    for(int i = 0; i<cars.size();i++){
        glPushMatrix();
        glTranslatef(cars[i].location.x,cars[i].location.y , cars[i].location.z);
        glPushMatrix();
        glScalef(cars[i].scaleX,cars[i].scaleY,cars[i].scaleZ);
        
            
            
            if(cars[i].selected){
                setMaterial('w');
                glutWireCube(cars[i].scaleX*2);
                cars[i].shape = currentItem;
                setMaterial(currentMaterial);
                cars[i].material = currentMaterial;
                
            }
            else{
                setMaterial(cars[i].material);
                
            }
        
        
        
        
       //Used to switch the current targets shape a switch command was used to be able to easily change
    //Shapes
        
        
        
        if(cars[i].selected){
            

           
            switch(currentItem)
            {
                case 'c':
                    glutSolidCube(cars[i].scaleX);
                    
                    break;
                    
                case 's':
                    glutSolidSphere(cars[i].scaleX/2,20,20);
                    break;
                    
                case 't':
                    glutSolidTeapot(cars[i].scaleX);
                    break;
                    
                case 'o':
                    glutSolidCone(cars[i].scaleX, cars[i].scaleX, 12, 12);
                    break;
                    
                case 'y':
                    glutSolidTorus(cars[i].scaleX/2, cars[i].scaleX, 10, 12);
                    
                    break;
                    
                    
            }
            
            
        }
        
        else{
        switch(cars[i].shape)
        {
            case 'c':
                glutSolidCube(cars[i].scaleX);
                break;
                
            case 's':
                glutSolidSphere(cars[i].scaleX/2,20,20);
                break;
                
            case 't':
                glutSolidTeapot(cars[i].scaleX);
                break;
                
            case 'o':
                glutSolidCone(cars[i].scaleX, cars[i].scaleX, 12, 12);
                break;
                
            case 'y':
               glutSolidTorus(cars[i].scaleX/2, cars[i].scaleX, 10, 12);
                break;
                

        }
            
        }
       
        
        
        glPopMatrix();
        glPopMatrix();
    }
    
    
    
    
    
    
    
    //The timerFunc is set to redisplay every 5ms
    
	glutSwapBuffers();
    glutTimerFunc(20,timer,0);
    }
    
}

//-----------------------------------
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}
//--------------------------------------------

int main(int argc, char** argv)
{
    
    srand(time(0));
    
    glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
    
   
    
    
	glutCreateWindow("Assignment 3");	//creates the window
    glutMouseFunc(mouse);
    glutSpecialFunc(special);

 

    
    
    glutDisplayFunc(display);	//registers "display" as the display callback function
    
    
	
	

    glutIdleFunc(idle);
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BACK);
  

	init();
    notesToTheTA();
    
	glutMainLoop();				//starts the event loop
    
    
	return(0);					//return may not be necessary on all compilers
}