#ifndef DIP_H
#define DIP_H

GLUquadric* IDquadric=gluNewQuadric() ;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double rotationanglea1=0;
double rotationanglea2=0;
double rotationanglea3=0;
GLuint texid;
GLuint fishup;
GLuint innerCircle;
GLuint outerCircle;
GLuint frontPlate;
GLuint door;
GLuint backPlate;
GLuint base;

GLUquadric* IQuadric;


struct point
{
	double x,y,z;
};

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);{
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);
            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);
            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);   //grey
        glBegin(GL_LINES);{
            for(i=-8;i<=8;i++){

                if(i==0)
                    continue;   //SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }glEnd();
    }
}


void drawSquare(float a)
{
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();
}


void drawss()
{
    glPushMatrix();{
        glRotatef(angle,0,0,1);
        glTranslatef(75,0,0);
        glRotatef(2*angle,0,0,1);


        glPushMatrix();{
            glRotatef(angle,0,0,1);
            glTranslatef(25,0,0);
            glRotatef(3*angle,0,0,1);
            glColor3f(0.0, 0.0, 1.0);
            drawSquare(5);

        }glPopMatrix();



        glColor3f(1.0, 0.0, 0.0);
        drawSquare(10.0);
    }glPopMatrix();

}

//draws half sphere
//draws half sphere
void drawsphere(float radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=sqrt(radius*radius-h*h);
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }

    }
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
            glBegin(GL_QUADS);{
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

            }glEnd();
        }

    }
}

void drawCylindar(float radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    double factor=1;

    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        //r=sqrt(radius*radius-h*h);
        r= radius*factor;
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            //points[i][j].y=radius;
            points[i][j].z=h;
        }
       // factor-=.02;
    }
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
            glBegin(GL_QUADS);{
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

            }glEnd();
        }

    }
}

void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if(vz == 0)
        vz = .0001;

    float v = sqrt( vx*vx + vy*vy + vz*vz );
    float ax = 57.2957795*acos( vz/v );
    if ( vz < 0.0 )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef( x1,y1,z1 );
    glRotatef(ax, rx, ry, 0.0);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, subdivisions, 1);

    //draw the first cap
    gluQuadricOrientation(quadric,GLU_INSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glTranslatef( 0,0,v );

    //draw the second cap
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();
}
void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
    gluDeleteQuadric(quadric);
}

void drawInnerCircle(float radius,float degree)
{

    glPushMatrix();{
       //glBegin(GL_LINE_LOOP);
       glPushMatrix();{
           glBegin(GL_POLYGON);{
               glVertex3f(0,0,0);
               for (int i=0; i < degree; i++)
               {
                  float degInRad = i*DEG2RAD;
                  glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);
                 // glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,-3);
               }
           }
           glEnd();
       }glPopMatrix();

       glPushMatrix();{
            //float degInRad = i*DEG2RAD;
            //glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);
            float degInRad1 ;
            float degInRad2 ;


//            glEnable(GL_TEXTURE_2D);
//            glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
//            glNormal3f(1.0,0.0,0.0);
//            glBegin(GL_POLYGON);
//                glTexCoord2f(0,0); glVertex3f(0,0,0);
//                glTexCoord2f(10,0); glVertex3f(0,100,0);
//                glTexCoord2f(10,10); glVertex3f(100,100,0);
//                glTexCoord2f(0,10); glVertex3f(100,0,0);
//            glEnd();
//            glDisable(GL_TEXTURE_2D);


            for(int i=2;i<degree;i++)
            {
                int j=i-1;
                 degInRad1 = i*DEG2RAD;
                 degInRad2 = j*DEG2RAD;
                 glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,innerCircle); // here texid corresponds a bitmap image.
                glNormal3f(1.0,0.0,0.0);
                glBegin(GL_POLYGON);{
                glTexCoord2f(0,0);  glVertex3f(cos(degInRad1)*radius,sin(degInRad1)*radius,0);
                glTexCoord2f(0,1); glVertex3f(cos(degInRad2)*radius,sin(degInRad2)*radius,0);
                glTexCoord2f(1,0); glVertex3f(cos(degInRad2)*radius,sin(degInRad2)*radius,-3);
                glTexCoord2f(1,1); glVertex3f(cos(degInRad1)*radius,sin(degInRad1)*radius,-3);
                }glEnd();
                glDisable(GL_TEXTURE_2D);
            }
            //printf("%f, %f\n",cos(degInRad1)*radius,sin(degInRad1)*radius);
       }
       glPopMatrix();
   }glPopMatrix();
}


void drawOuterCircle(float radius,float degree)
{

    glPushMatrix();{
       //glBegin(GL_LINE_LOOP);
       glPushMatrix();{
           glBegin(GL_POLYGON);{
               glVertex3f(0,0,0);
               for (int i=0; i < degree; i++)
               {
                  float degInRad = i*DEG2RAD;
                  glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);
                 // glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,-3);
               }
           }
           glEnd();
       }glPopMatrix();

       glPushMatrix();{
            //float degInRad = i*DEG2RAD;
            //glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);
            float degInRad1 ;
            float degInRad2 ;
            for(int i=2;i<degree;i++)
            {
                int j=i-1;
                 degInRad1 = i*DEG2RAD;
                 degInRad2 = j*DEG2RAD;
                 glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,outerCircle); // here texid corresponds a bitmap image.
                glNormal3f(1.0,0.0,0.0);
                glBegin(GL_POLYGON);{
                glTexCoord2f(0,0);  glVertex3f(cos(degInRad1)*radius,sin(degInRad1)*radius,0);
                glTexCoord2f(0,1); glVertex3f(cos(degInRad2)*radius,sin(degInRad2)*radius,0);
                glTexCoord2f(1,0); glVertex3f(cos(degInRad2)*radius,sin(degInRad2)*radius,-3);
                glTexCoord2f(1,1); glVertex3f(cos(degInRad1)*radius,sin(degInRad1)*radius,-3);
                }glEnd();
                glDisable(GL_TEXTURE_2D);
            }
           // printf("%f, %f\n",cos(degInRad1)*radius,sin(degInRad1)*radius);
       }
       glPopMatrix();
   }glPopMatrix();
}

void RenderSineWave()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);  // clear background with black
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    double w = glutGet( GLUT_WINDOW_WIDTH );
    double h = glutGet( GLUT_WINDOW_HEIGHT );
    double ar = w / h;
    glOrtho( -360 * ar, 360 * ar, -120, 120, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glPointSize(10);
    glColor3f(1.0,0.0,0.0);

    glBegin(GL_POINTS);
    for(int i=0;i<361;i=i+5)
    {
        float x = (float)i;
        float y = 100.0 * sin(i *(6.284/360.0));
        glVertex2f(x,y);
    }
    glEnd();

    glutSwapBuffers();
}

void drawFish() {
    int mult= 1;

    glPushMatrix();{

        //        glEnable(GL_TEXTURE_2D);
//            glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
//            glNormal3f(1.0,0.0,0.0);
//            glBegin(GL_POLYGON);
//                glColor3f(1,1,1);
//                glTexCoord2f(0,0); glVertex3f(0,0,0);
//                glTexCoord2f(10,0); glVertex3f(0,100,0);
//                glTexCoord2f(10,10); glVertex3f(100,100,0);
//                glTexCoord2f(0,10); glVertex3f(100,0,0);
//            glEnd();
//        glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glColor3f(1,1,1);
        glTexCoord2f(0,0); glVertex3f(mult*0, mult*0,mult*0);
        glTexCoord2f(50,0); glVertex3f( mult*20,mult*0,mult*0);
        glTexCoord2f(100,30); glVertex3f(mult*40,mult*40,mult*0);
        glTexCoord2f(60,60); glVertex3f(mult*35,mult* 45,mult*0);
        glTexCoord2f(30,100); glVertex3f(mult*28, mult*60,mult*0);
        glTexCoord2f(0,90); glVertex3f(mult*0, mult*60,mult*0);
    }glEnd();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
     glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*28, mult*60,mult*0);
        glTexCoord2f(100,0); glVertex3f( mult*20,mult*65,mult*0);
        glTexCoord2f(0,100); glVertex3f(mult*17,mult*60,mult*0);
    }glEnd();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*17, mult*60,mult*0);
        glTexCoord2f(0,100); glVertex3f( mult*15,mult*62.5,mult*0);
        glTexCoord2f(100,0); glVertex3f(mult*0,mult*62.5,mult*0);
        glTexCoord2f(100,100); glVertex3f(mult*0,mult*60,mult*0);
    }glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*15, mult*62.5,mult*0);
        glTexCoord2f(0,100); glVertex3f( mult*25,mult*80,mult*0);
        glTexCoord2f(100,100);glVertex3f( mult*0,mult*80,mult*0);
        glTexCoord2f(100,0);glVertex3f( mult*0,mult*62.5,mult*0);
    }glEnd();
    glDisable(GL_TEXTURE_2D);

    //lenja begins
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_TRIANGLES);{
        glTexCoord2f(0,0);glVertex3f(mult*25, mult*80,mult*0);
        glTexCoord2f(100,0);glVertex3f( mult*20,mult*82.5,mult*0);
        glTexCoord2f(0,100);glVertex3f(mult*20,mult*80,mult*0);

    }glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*20, mult*80,mult*0);
        glTexCoord2f(0,100);glVertex3f( mult*20,mult*82.5,mult*0);
        glTexCoord2f(100,0);glVertex3f(mult*15,mult*84.5,mult*0);
        glTexCoord2f(100,100);glVertex3f(mult*15,mult*80,mult*0);

    }glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*15, mult*80,mult*0);
        glTexCoord2f(100,0);glVertex3f( mult*15,mult*84.5,mult*0);
        glTexCoord2f(100,100);glVertex3f(mult*10,mult*86,mult*0);
        glTexCoord2f(0,100);glVertex3f(mult*10,mult*80,mult*0);

    }glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*10, mult*80,mult*0);
        glTexCoord2f(100,0);glVertex3f( mult*10,mult*86,mult*0);
        glTexCoord2f(100,100);glVertex3f(mult*5,mult*87,mult*0);
        glTexCoord2f(0,100);glVertex3f(mult*5,mult*80,mult*0);

    }glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,fishup); // here texid corresponds a bitmap image.
    glNormal3f(1.0,0.0,0.0);
    glBegin(GL_POLYGON);{
        glTexCoord2f(0,0); glVertex3f(mult*5, mult*80,mult*0);
        glTexCoord2f(0,100);glVertex3f( mult*5,mult*87,mult*0);
        glTexCoord2f(100,100);glVertex3f(mult*0,mult*87,mult*0);
        glTexCoord2f(100,0);glVertex3f(mult*0,mult*80,mult*0);

    }glEnd();
    glDisable(GL_TEXTURE_2D);


    }
    glPopMatrix();
}


void drawfishbones()
{
    int mult = 1;
      glPushMatrix();{
          glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,frontPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
            glColor3f(1,1,1);
            glTexCoord2f(0,0);glVertex3f(mult*0, mult*0,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*20,mult*0,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*20,mult*00,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*0,mult*0,mult*-10);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }glPopMatrix();

    glPushMatrix();{
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,frontPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
            glColor3f(1,1,1);
            glTexCoord2f(0,0);glVertex3f(mult*0, mult*0,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*-20,mult*0,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*-20,mult*00,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*0,mult*0,mult*-10);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }glPopMatrix();

    glPushMatrix();{
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,door); // here texid corresponds a bitmap image.
        glNormal3f(1.0,0.0,0.0);
        glBegin(GL_POLYGON);{
            glTexCoord2f(0,0);glVertex3f(mult*20, mult*0,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*30,mult*20,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*30,mult*20,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*20,mult*0,mult*-10);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }glPopMatrix();

    glPushMatrix();{
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,door); // here texid corresponds a bitmap image.
        glNormal3f(1.0,0.0,0.0);
        glBegin(GL_POLYGON);{
            glTexCoord2f(0,0);glVertex3f(mult*-20, mult*0,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*-30,mult*20,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*-30,mult*20,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*-20,mult*0,mult*-10);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }glPopMatrix();

    glPushMatrix();{
        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,frontPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
            glTexCoord2f(0,0);glVertex3f(mult*30, mult*20,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*40,mult*40,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*40,mult*40,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*30,mult*20,mult*-10);
           // glColor3f(1,0,0);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }glPopMatrix();

    glPushMatrix();{
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,frontPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
            glTexCoord2f(0,0);glVertex3f(mult*-30, mult*20,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*-40,mult*40,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*-40,mult*40,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*-30,mult*20,mult*-10);
           // glColor3f(1,0,0);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }glPopMatrix();

    glPushMatrix();{
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,frontPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
            glTexCoord2f(0,0);glVertex3f(mult*40, mult*40,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*35,mult*45,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*35,mult*45,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*40,mult*40,mult*-10);
           // glColor3f(1,0,0);
        }glEnd();
        glDisable(GL_TEXTURE_2D);


        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,frontPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
            glTexCoord2f(0,0);glVertex3f(mult*-40, mult*40,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*-35,mult*45,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*-35,mult*45,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*-40,mult*40,mult*-10);
           // glColor3f(1,0,0);
        }glEnd();
        glDisable(GL_TEXTURE_2D);

    }glPopMatrix();

    glPushMatrix();{
         glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,backPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
         glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
            glTexCoord2f(0,0);glVertex3f(mult*35, mult*45,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*28,mult*60,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*28,mult*60,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*35,mult*45,mult*-10);

        }glEnd();
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,backPlate); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
            glTexCoord2f(0,0);glVertex3f(mult*-35, mult*45,mult*0);
            glTexCoord2f(1,0);glVertex3f( mult*-28,mult*60,mult*0);
            glTexCoord2f(1,1);glVertex3f(mult*-28,mult*60,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*-35,mult*45,mult*-10);

        }glEnd();
        glDisable(GL_TEXTURE_2D);

    }glPopMatrix();

    glPushMatrix();{
        glBegin(GL_POLYGON);{
            glColor3f(.5,.5,.5);
            glVertex3f(mult*28, mult*60,mult*0);
            glVertex3f( mult*20,mult*65,mult*0);
            glVertex3f(mult*20,mult*65,mult*-10);
            glVertex3f(mult*28,mult*60,mult*-10);

        }glEnd();


        glBegin(GL_POLYGON);{
           // glColor3f(1,0,0);
           glColor3f(.5,.5,.5);
            glVertex3f(mult*-28, mult*60,mult*0);
            glVertex3f( mult*-20,mult*65,mult*0);
            glVertex3f(mult*-20,mult*65,mult*-10);
            glVertex3f(mult*-28,mult*60,mult*-10);

        }glEnd();

    }glPopMatrix();

    glPushMatrix();{
        glBegin(GL_POLYGON);{
            glColor3f(1,1,1);
            glVertex3f(mult*25, mult*80,mult*-0.01);
            glVertex3f(mult*20,mult*82.5,mult*-0.01);
            glVertex3f(mult*15,mult*84.5,mult*-0.01);
            glVertex3f(mult*10,mult*86,mult*-0.01);
            glVertex3f(mult*5,mult*87,mult*-0.01);
            glVertex3f(mult*0,mult*87,mult*-.01);
            glVertex3f(mult*0,mult*87,mult*-10);
            glVertex3f(mult*5,mult*87,mult*-10);
            glVertex3f(mult*10,mult*86,mult*-10);
            glVertex3f(mult*15,mult*84.5,mult*-10);
            glVertex3f(mult*20,mult*82.5,mult*-10);
            glVertex3f(mult*25, mult*80,mult*-10);
        }glEnd();

        //glEnable(GL_TEXTURE_2D);
           // glBindTexture(GL_TEXTURE_2D,door); // here texid corresponds a bitmap image.
           // glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
            glColor3f(0,1,.2);
            glTexCoord2f(0,0);glVertex3f(mult*-5, mult*87,mult*-3);
            glTexCoord2f(1,0);glVertex3f( mult*-1,mult*87,mult*-3);
            glTexCoord2f(1,1);glVertex3f(mult*-1,mult*87,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*-5,mult*87,mult*-10);

        }glEnd();

        //glEnable(GL_TEXTURE_2D);
           // glBindTexture(GL_TEXTURE_2D,door); // here texid corresponds a bitmap image.
           // glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);{
            glColor3f(0,1,.2);
            glTexCoord2f(0,0);glVertex3f(mult*5, mult*87,mult*-3);
            glTexCoord2f(1,0);glVertex3f( mult*1,mult*87,mult*-3);
            glTexCoord2f(1,1);glVertex3f(mult*1,mult*87,mult*-10);
            glTexCoord2f(0,1);glVertex3f(mult*5,mult*87,mult*-10);

        }glEnd();
        //glDisable(GL_TEXTURE_2D);

        glBegin(GL_POLYGON);{
            glColor3f(1,1,1);
            glVertex3f(mult*-25, mult*80,mult*-0.01);
            glVertex3f( mult*-20,mult*82.5,mult*-0.01);
            glVertex3f(mult*-15,mult*84.5,mult*-0.01);
            glVertex3f(mult*-10,mult*86,mult*-0.01);
            glVertex3f(mult*-5,mult*87,mult*-0.01);
            glVertex3f(mult*-0,mult*87,mult*-0.01);
            glVertex3f(mult*-0,mult*87,mult*-10);
            glVertex3f(mult*-5,mult*87,mult*-10);
            glVertex3f(mult*-10,mult*86,mult*-10);
            glVertex3f(mult*-15,mult*84.5,mult*-10);
            glVertex3f( mult*-20,mult*82.5,mult*-10);
            glVertex3f(mult*-25, mult*80,mult*-10);
        }glEnd();

    }glPopMatrix();
}

void drawPiller()
{

        glPushMatrix();{
           // glTranslatef(4,-10,0);

           //base polygons
            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);


                glVertex3f(8,43,0.5);
                glVertex3f(14,35,0.5);
                glVertex3f(24,35,0.5);
                glVertex3f(30,43,0.5);
                glVertex3f(24,51,0.5);
                glVertex3f(13,51,0.5);

            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);


                glVertex3f(-8,43,0.5);
                glVertex3f(-14,35,0.5);
                glVertex3f(-24,35,0.5);
                glVertex3f(-30,43,0.5);
                glVertex3f(-24,51,0.5);
                glVertex3f(-13,51,0.5);

            }glEnd();
            //base polygons

            //first pillar
            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(0,43,0.5+160);
                glVertex3f(6,35,0.5+160);
                glVertex3f(16,35,0.5+160);
                glVertex3f(22,43,0.5+160);
                glVertex3f(16,51,0.5+160);
                glVertex3f(6,51,0.5+160);

            }glEnd();


            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-0,43,0.5+160);
                glVertex3f(-6,35,0.5+160);
                glVertex3f(-16,35,0.5+160);
                glVertex3f(-22,43,0.5+160);
                glVertex3f(-16,51,0.5+160);
                glVertex3f(-6,51,0.5+160);

            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(8,43,0.5);
                glVertex3f(14,35,0.5);
                glVertex3f(6,35,0.5+160);
                glVertex3f(0,43,0.5+160);
            }glEnd();


             glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-8,43,0.5);
                glVertex3f(-14,35,0.5);
                glVertex3f(-6,35,0.5+160);
                glVertex3f(-0,43,0.5+160);

            }glEnd();

             glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(14,35,0.5);
                glVertex3f(24,35,0.5);
                glVertex3f(16,35,0.5+160);
                glVertex3f(6,35,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(-14,35,0.5);
                glVertex3f(-24,35,0.5);
                glVertex3f(-16,35,0.5+160);
                glVertex3f(-6,35,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(24,35,0.5);
                glVertex3f(30,43,0.5);
                glVertex3f(22,43,0.5+160);
                glVertex3f(16,35,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(-24,35,0.5);
                glVertex3f(-30,43,0.5);
                glVertex3f(-22,43,0.5+160);
                glVertex3f(-16,35,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(30,43,0.5);
                glVertex3f(24,51,0.5);
                glVertex3f(16,51,0.5+160);
                glVertex3f(22,43,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(-30,43,0.5);
                glVertex3f(-24,51,0.5);
                glVertex3f(-16,51,0.5+160);
                glVertex3f(-22,43,0.5+160);
            }glEnd();


            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(24,51,0.5);
                glVertex3f(13,51,0.5);
                glVertex3f(6,51,0.5+160);
                glVertex3f(16,51,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(-24,51,0.5);
                glVertex3f(-13,51,0.5);
                glVertex3f(-6,51,0.5+160);
                glVertex3f(-16,51,0.5+160);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(8,43,0.5);
                glVertex3f(13,51,0.5);
                glVertex3f(6,51,0.5+160);
                glVertex3f(0,43,0.5+160);

            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);

                glVertex3f(-8,43,0.5);
                glVertex3f(-13,51,0.5);
                glVertex3f(-6,51,0.5+160);
                glVertex3f(-0,43,0.5+160);

            }glEnd();

            // end of first pillar

            //starting of second pillar

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-10,35,160.5+67);
                glVertex3f(10,35,160.5+67);
                glVertex3f(16,43,160.5+67);
                glVertex3f(10,51,160.5+67);
                glVertex3f(-10,51,160.5+67);
                glVertex3f(-16,43,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(6,35,160.5);
                glVertex3f(16,35,160.5);
                glVertex3f(10,35,160.5+67);
                glVertex3f(0,35,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-6,35,160.5);
                glVertex3f(-16,35,160.5);
                glVertex3f(-10,35,160.5+67);
                glVertex3f(0,35,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(16,35,160.5);
                glVertex3f(22,43,160.5);
                glVertex3f(16,43,160.5+67);
                glVertex3f(10,35,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-16,35,160.5);
                glVertex3f(-22,43,160.5);
                glVertex3f(-16,43,160.5+67);
                glVertex3f(-10,35,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(22,43,160.5);
                glVertex3f(16,51,160.5);

                glVertex3f(10,51,160.5+67);
                glVertex3f(16,43,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-22,43,160.5);
                glVertex3f(-16,51,160.5);

                glVertex3f(-10,51,160.5+67);
                glVertex3f(-16,43,160.5+67);
            }glEnd();


            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(16,51,160.5);
                glVertex3f(6,51,160.5);

                glVertex3f(0,51,160.5+67);
                glVertex3f(10,51,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-16,51,160.5);
                glVertex3f(-6,51,160.5);

                glVertex3f(0,51,160.5+67);
                glVertex3f(-10,51,160.5+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(6,35,160.5);
                glVertex3f(-6,35,160.5);
                glVertex3f(0,35,160.5+67);

            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(6,51,160.5);
                glVertex3f(-6,51,160.5);
                glVertex3f(0,51,160.5+67);

            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(5,35,0.5+160+67+45);
                glVertex3f(11,43,0.5+160+45+67);
                glVertex3f(5,51,0.5+160+45+67);
                glVertex3f(-5,51,0.5+160+45+67);
                glVertex3f(-11,43,0.5+160+45+67);
                glVertex3f(-5,35,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-10,35,0.5+160+67);
                glVertex3f(10,35,0.5+160+67);
                glVertex3f(5,35,0.5+160+45+67);
                glVertex3f(-5,35,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(10,35,0.5+160+67);
                glVertex3f(16,43,0.5+160+67);
                glVertex3f(11,43,0.5+160+45+67);
                glVertex3f(5,35,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(16,43,0.5+160+67);
                glVertex3f(10,51,0.5+160+67);
                glVertex3f(5,51,0.5+160+45+67);
                glVertex3f(11,43,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(10,51,0.5+160+67);
                glVertex3f(-10,51,0.5+160+67);
                glVertex3f(-5,51,0.5+160+45+67);
                glVertex3f(5,51,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-10,51,0.5+160+67);
                glVertex3f(-16,43,0.5+160+67);
                glVertex3f(-11,43,0.5+160+45+67);
                glVertex3f(-5,51,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(16,43,0.5+160+67);
                glVertex3f(10,51,0.5+160+67);
                glVertex3f(5,51,0.5+160+45+67);
                glVertex3f(11,43,0.5+160+45+67);
            }glEnd();


            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-16,43,0.5+160+67);
                glVertex3f(-10,35,0.5+160+67);
                glVertex3f(-5,35,0.5+160+45+67);
                glVertex3f(-11,43,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(16,43,0.5+160+67);
                glVertex3f(10,51,0.5+160+67);
                glVertex3f(5,51,0.5+160+45+67);
                glVertex3f(11,43,0.5+160+45+67);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-.6,43,0.5+160+67+45+90);
                glVertex3f(.6,43,0.5+160+67+45+90);
                glVertex3f(1.2,43.8,0.5+160+67+45+90);
                glVertex3f(.6,44.6,0.5+160+67+45+90);
                glVertex3f(-.6,44.6,0.5+160+67+45+90);
                glVertex3f(-1.2,43.8,0.5+160+67+45+90);
            }glEnd();


            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-5,35,0.5+160+67+45);
                glVertex3f(5,35,0.5+160+67+45);
                glVertex3f(.6,43,0.5+160+45+67+90);
                glVertex3f(-.6,43,0.5+160+45+67+90);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(5,35,0.5+160+67+45);
                glVertex3f(11,43,0.5+160+67+45);
                glVertex3f(1.2,43.8,0.5+160+45+67+90);
                glVertex3f(.6,43,0.5+160+45+67+90);
            }glEnd();




            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(11,43,0.5+160+67+45);
                glVertex3f(5,51,0.5+160+67+45);
                glVertex3f(.6,44.6,0.5+160+45+67+90);
                glVertex3f(1.2,43.8,0.5+160+45+67+90);
            }glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(5,51,0.5+160+67+45);
                glVertex3f(-5,51,0.5+160+67+45);
                glVertex3f(-.6,44.6,0.5+160+45+67+90);
                glVertex3f(.6,44.6,.5+160+45+67+90);
            }glEnd();

                        glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-5,51,0.5+160+67+45);
                glVertex3f(-11,43,0.5+160+67+45);
                glVertex3f(-1.2,43.8,0.5+160+45+67+90);
                glVertex3f(-.6,44.6,0.5+160+45+67+90);
            }glEnd();

                        glBegin(GL_POLYGON);{
                glColor3f(1,1,1);
                glVertex3f(-5,35,0.5+160+67+45);
                glVertex3f(-11,43,0.5+160+67+45);
                glVertex3f(-1.2,43.8,0.5+160+45+67+90);
                glVertex3f(-.6,43,0.5+160+45+67+90);
            }glEnd();
        }
        glPopMatrix();

}


void drawBase()
{
    glPushMatrix();
    {


        //outer half circle
        glPushMatrix();{

            glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                glVertex3f(-43,50,-13);
                glVertex3f(-60,85,-13);
                glVertex3f(-50,90,-13);
                glVertex3f(0,50,-13);
                glVertex3f(0,0,-13);
            }
            glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                glVertex3f(50,24,-13);
                glVertex3f(60,27,-13);
                //glVertex3f(70,24,-13);
                glVertex3f(60,35,-13);
                glVertex3f(0,35,-13);
                glVertex3f(0,0,-13);
            }
            glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                //glVertex3f(50,24,-13);
                glVertex3f(60,27,-13);
                //glVertex3f(70,24,-13);
                glVertex3f(73,24,-13);
                glVertex3f(73,35,-13);
                glVertex3f(60,35,-13);
            }
            glEnd();

            glColor3f(.5,.5,.5);
            glTranslatef(0,0,-13);
            glTranslatef(0,25,0);
            glRotatef(150,0,0,1);
            drawOuterCircle(50,210);

        }glPopMatrix();

        //inner half circle
        glPushMatrix();{
            glColor3f(.5,.5,.5);
            glTranslatef(0,0,-10);
            glTranslatef(3,30,0);
            glRotatef(180,0,0,1);

            drawInnerCircle(43,185);
        }glPopMatrix();

        glPushMatrix();{
            glColor3f(.5,.5,.5);
            glBegin(GL_POLYGON);{
                glVertex3f(39.64,27,-10);
                glVertex3f(39.64,27,-13);
                glVertex3f(45,88,-13);
                glVertex3f(45,88,-10);

            }glEnd();
            glBegin(GL_POLYGON);{
                glVertex3f(39.64,27,-10);
                //glVertex3f(39.64,27,-13);
                //glVertex3f(50,88,-13);
                glVertex3f(45,88,-10);
                glVertex3f(0,50,-10);

            }glEnd();
//          glTranslatef(0,0,-10);
//          glTranslatef(3,30,0);
//            glRotatef(180,0,0,1);

            //drawInnerCircle(43,185);
        }glPopMatrix();



        //main part drawing here

        glPushMatrix();{

            glColor3f(1,0,0);
            drawfishbones();
            glColor3f(0,1,0);
            drawFish();
            glRotatef(180,0,1,0);
            drawFish();
        }glPopMatrix();
        //main part drawing here
    }
    glPopMatrix();
}


void drawJoints(double scaling, double height)
{
    glPushMatrix();{
//        glBegin(GL_POLYGON);{
//            glColor3f(1,1,1);
//            glVertex3f(5,53,height);
//            glVertex3f(15,53,height);
//            glVertex3f(18,53,height);
//            glVertex3f(8,53,height);
//        }glEnd();

        glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                //glVertex3f(50,24,-13);
                glVertex3f(scaling*5,scaling*53,height);
                //glVertex3f(70,24,-13);
                glVertex3f(scaling*20,scaling*53,height);
                glVertex3f(scaling*20,scaling*58,height);
                glVertex3f(scaling*5,scaling*58,height);
            }
            glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                //glVertex3f(50,24,-13);
                glVertex3f(scaling*5,scaling*53,height-5);
                //glVertex3f(70,24,-13);
                glVertex3f(scaling*20,scaling*53,height-5);
                glVertex3f(scaling*20,scaling*58,height-5);
                glVertex3f(scaling*5,scaling*58,height-5);
            }
            glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                //glVertex3f(50,24,-13);
                glVertex3f(scaling*5,scaling*53,height);
                //glVertex3f(70,24,-13);
                glVertex3f(scaling*20,scaling*53,height);
                glVertex3f(scaling*20,scaling*53,height-5);
                glVertex3f(scaling*5,scaling*53,height-5);
            }
            glEnd();

            glBegin(GL_POLYGON);{
                glColor3f(.5,.5,.5);
                //glVertex3f(50,24,-13);
                glVertex3f(scaling*5,scaling*58,height);
                //glVertex3f(70,24,-13);
                glVertex3f(scaling*20,scaling*58,height);
                glVertex3f(scaling*20,scaling*58,height-5);
                glVertex3f(scaling*5,scaling*58,height-5);
            }
            glEnd();

    }glPopMatrix();
}

void drawFinalJoints(double height)
{
    glPushMatrix();{
    glTranslatef(-12.5,-15,0);
    drawJoints(1,height);
    }glPopMatrix();
}

void drawrailings(){
        glPushMatrix();{
            glTranslatef(0,0,-10);
            glTranslatef(-38,37,0);
            glRotatef(-96,0,0,1);
            for(int i=2;i<185;i+=10)
            {
                 int j=i-1;
                 double degInRad1 = i*DEG2RAD;
                 double degInRad2 = j*DEG2RAD;

                 glTranslatef(cos(degInRad1)*7.3,sin(degInRad1)*7.3,0);
                 glPushMatrix();{

                 glColor3f(1,1,1);
                       // renderCylinder_convenient(cos(degInRad1)*7.3,sin(degInRad1)*7.3,10,cos(degInRad2)*7.3,sin(degInRad2)*7.3,10,.2,20);
                        gluCylinder(IDquadric,.2 ,.2 ,3,20,20);

                }glPopMatrix();


                }glEnd();
        }glPopMatrix();

        //circular railing
        glPushMatrix();{
            glTranslatef(0,0,-17);
            glTranslatef(3,30,0);
            glRotatef(180,0,0,1);
            for(int i=10;i<185;i+=10)
            {
                 int j=i-10;
                 double degInRad1 = i*DEG2RAD;
                 double degInRad2 = j*DEG2RAD;

                 //glTranslatef(cos(degInRad1)*7.3,sin(degInRad1)*7.3,0);
                 glPushMatrix();{

                 glColor3f(1,1,1);
                        renderCylinder_convenient(cos(degInRad1)*42,sin(degInRad1)*42,10,cos(degInRad2)*42,sin(degInRad2)*42,10,.2,20);
             //           gluCylinder(IDquadric,.2 ,.2 ,3,20,20);

                }glPopMatrix();


                }glEnd();
        }glPopMatrix();

        //circular ends here

        //circular top starts here
        glPushMatrix();{
            glTranslatef(0,0,-2);
            glTranslatef(0,25,0);
            glRotatef(170,0,0,1);
            for(int i=10;i<210;i+=10)
            {
                 int j=i-10;
                 double degInRad1 = i*DEG2RAD;
                 double degInRad2 = j*DEG2RAD;

                 //glTranslatef(cos(degInRad1)*7.3,sin(degInRad1)*7.3,0);
                 glPushMatrix();{

                 glColor3f(1,1,1);
                        renderCylinder_convenient(cos(degInRad1)*35,sin(degInRad1)*35,0,cos(degInRad2)*35,sin(degInRad2)*35,0,1,20);
             //           gluCylinder(IDquadric,.2 ,.2 ,3,20,20);

                }glPopMatrix();


                }glEnd();
        }glPopMatrix();


        glPushMatrix();{
            glColor3f(1,1,1);
            glTranslatef(0,0,-2);
            renderCylinder_convenient(10,0,0,10,-9,0,.5,20);
            renderCylinder_convenient(-10,0,0,-10,-9,0,.5,20);
            renderCylinder_convenient(20.5,1,0,26,1,0,.5,20);
            renderCylinder_convenient(-20.5,1,0,-26,1,0,.5,20);
            renderCylinder_convenient(20,4,0,32,11,0,.5,20);
            renderCylinder_convenient(-20,4,0,-32,11,0,.5,20);
            renderCylinder_convenient(26,12,0,33,16,0,.5,20);
            renderCylinder_convenient(-26,12,0,-33,16,0,.5,20);

            renderCylinder_convenient(35,24,0,15,5,0,.5,20);
            renderCylinder_convenient(-35,24,0,15,5,0,.5,20);

            renderCylinder_convenient(35,24,0,0,24,0,.5,20);
            renderCylinder_convenient(-35,24,0,0,24,0,.5,20);

        }glPopMatrix();

        //horizontal starts here


            for (int i=-20;i<=20;i+=5)
            {
                glPushMatrix();{
                    glColor3f(1,1,1);
                    glTranslatef(i,0,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }

            for(double i=20;i<=40;i+=2)
            {

                glPushMatrix();{
                    glColor3f(1,1,1);
                    double j=2*i-40;
                    glTranslatef(i,j,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }

            for(double i=20;i<=40;i+=2)
            {

                glPushMatrix();{
                    glColor3f(1,1,1);
                    double j=2*i-40;
                    glTranslatef(-i,j,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }
            for(double i=28;i<=35;i+=2)
            {
                glPushMatrix();{
                    glColor3f(1,1,1);
                    double j=-2.142*i+75.005+45;
                    glTranslatef(i,j,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }

            for(double i=28;i<=35;i+=2)
            {
                glPushMatrix();{
                    glColor3f(1,1,1);
                    double j=-2.142*i+75.005+45;
                    glTranslatef(-i,j,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }

            for(double i=35;i<=40;i+=3)
            {
                glPushMatrix();{
                    glColor3f(1,1,1);
                    double j=-i+80;
                    glTranslatef(i,j,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }

            for(double i=35;i<=40;i+=3)
            {
                glPushMatrix();{
                    glColor3f(1,1,1);
                    double j=-i+80;
                    glTranslatef(-i,j,0);
                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
                }glPopMatrix();
            }

            glPushMatrix();{
                glColor3f(1,1,1);
                renderCylinder_convenient(20,0,3,-20,0,3,.2,10);
                renderCylinder_convenient(20,0,3,40,40,3,.2,10);
                renderCylinder_convenient(35,45,3,40,40,3,.2,10);
                renderCylinder_convenient(35,45,3,28,60,3,.2,10);
                renderCylinder_convenient(28,60,3,20,65,3,.2,10);
                renderCylinder_convenient(-20,0,3,-40,40,3,.2,10);
                renderCylinder_convenient(-35,45,3,-40,40,3,.2,10);
                renderCylinder_convenient(-35,45,3,-28,60,3,.2,10);
                renderCylinder_convenient(-28,60,3,-20,65,3,.2,10);
            }glPopMatrix();

//
//            for(double i=20;i<=28;i+=3)
//            {
//                glPushMatrix();{
//                    glColor3f(1,0,0);
//                    double j=(i-28)/(8)*(-5)+60;
//                    glTranslatef(i,j,0);
//                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
//                }glPopMatrix();
//            }
//
//            for(double i=20;i<=28;i+=3)
//            {
//                glPushMatrix();{
//                    glColor3f(1,0,0);
//                    double j=(i-28)/(8)*(-5)+60;
//                    glTranslatef(-i,j,0);
//                    gluCylinder(IDquadric,.2 ,.2 ,3,20,20);
//                }glPopMatrix();
//            }
//        }


       // glPopMatrix();
}


void loadFinalRailing()
{
    glColor3f(1,0,0);

    drawrailings();



}




void loadBaseImage()
{
    glPushMatrix();{
        glTranslatef(0,0,-16.01);
        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,base); // here texid corresponds a bitmap image.
            glNormal3f(1.0,0.0,0.0);
            glBegin(GL_POLYGON);
                glColor3f(1,1,1);
                glTexCoord2f(0,0); glVertex3f(-150,-150,0);
                glTexCoord2f(1,0); glVertex3f(-150,150,0);
                glTexCoord2f(1,1); glVertex3f(150,150,0);
                glTexCoord2f(0,1); glVertex3f(150,-150,0);
            glEnd();
        glDisable(GL_TEXTURE_2D);

    }
    glPopMatrix();
}
#endif