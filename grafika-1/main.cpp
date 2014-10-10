//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Széll András
// Neptun : DP1FGW
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

const int CIRCLE_RESOLUTION = 100;
const int CURVE_RESOLUTION = 1000;
const int ROTATE_INTERVAL = 5 * 1000;
const float CONTROL_POINT_R = 2.0f;
const float CONTROL_POINT_ROTATE_R = 5.0f;

#include "imps.cpp"

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;

const float screenWidthf = 600.0f;
const float screenHeightf = 600.0f;

const float cameraWidthf = 58.0f;
const float cameraHeightf = 68.0f;

//ControllPoints
ControllPoints cntrPts;
ControllPoints lastCntrPts;
Vector2D lastCntrVector;
bool rotateCntrPts = false;
long rotateStartTime;
float rotateT;
bool centerPointSelected = false;
int centerPointNum;

//Translation vectors
Vector2D tranVect = Vector2D(screenWidthf/cameraWidthf, screenHeightf/cameraHeightf);
Vector2D tranVectBack = Vector2D(cameraWidthf/screenWidthf, cameraHeightf/screenHeightf);


// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    glViewport(0, 0, screenWidth, screenHeight);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.95f, 0.95f, 0.95f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    
    //Control points
    ControllPoints cp = cntrPts;
    
    // Calculate control point rotation
    if (rotateCntrPts) {
        for (int i = 0; i < cp.size; i++) {
            float angle = (i%2 == 0) ? (rotateT + 0.25f) * 2.0f * M_PI : (1.0f-(rotateT - 0.25f)) * 2.0f * M_PI;
            cp.points[i] = Point2D(cp.points[i].x + CONTROL_POINT_ROTATE_R * cosf(angle), cp.points[i].y + CONTROL_POINT_ROTATE_R * (sinf(angle) - 1.0f));
        }
    }
    
    // Save last control points
    lastCntrPts = cp;
    
    // Center point
    Point2D centerPoint = (centerPointSelected) ? cp.points[centerPointNum] : Point2D(cameraWidthf/2.0f, cameraHeightf/2.0f);
    Vector2D centerVector = centerPoint.move(Vector2D(-cameraWidthf/2.0f, -cameraHeightf/2.0f)).vectorFromOrigo();
    lastCntrVector = centerVector;
    
    // Convex hull
    if (cp.size >= 2) {
        //Turkiz
        glColor3f(0.2549f, 0.8980f, 0.9686f);
        
        ConvexHull ch(&cp);
        ch.calcHull();
        
        glBegin(GL_POLYGON); {
            for (int i = 0; i < ch.size; i++) {
                Point2D p = ch.hull[i].move(centerVector * -1).toGlCoordinates(screenWidthf, screenHeightf, tranVect);
                glVertex2f(p.x, p.y);
            }
        } glEnd();
    }
    
    // Brezier curve
    {
        //Piros
        glColor3f(1.0f, 0.0f, 0.0f);
        
        BrezierCurve bc(&cp);
        
        glBegin(GL_LINE_STRIP); {
            for (int i = 0; i < CURVE_RESOLUTION; i++) {
                float t = (float)i/(float)CURVE_RESOLUTION;
                Point2D p = Point2D(bc.r(t)).move(centerVector * -1).toGlCoordinates(screenWidthf, screenHeightf, tranVect);
                glVertex2f(p.x,p.y);
            }
        } glEnd();
    }
    
    // Catmull-Clark curve
    {
        //Kek
        glColor3f(0.0f, 0.0f, 1.0f);
        
        CatmullClarkCurve cc(&cp);
        cc.calcPoints(MAX_SUBDIVISION_ITERATIONS);
        
        glBegin(GL_LINE_STRIP); {
            for (int i = 0; i < cc.size; i++) {
                Point2D p = cc.points[i].move(centerVector * -1).toGlCoordinates(screenWidthf, screenHeightf, tranVect);
                glVertex2f(p.x,p.y);
            }
        } glEnd();
    }
    
    // Catmull-Rom spline
    {
        //Zold
        glColor3f(0.0f, 0.7f, 0.0f);
        
        CatmullRomSpline cr(cp);
        
        glBegin(GL_LINE_STRIP); {
            for (int i = 0; i < cp.size; i++) {
                for (int k = 0; k < CURVE_RESOLUTION; k++) {
                    float t = (float)i/(float)CURVE_RESOLUTION;
                    Point2D p = Point2D(cr.r(t, i)).move(centerVector * -1).toGlCoordinates(screenWidthf, screenHeightf, tranVect);
                    glVertex2f(p.x,p.y);
                }
            }
        } glEnd();
    }
    
    //  Draw control points (most priority)
    {
        //Fekete
        glColor3f(0.0f, 0.0f, 0.0f);
        
        for (int i = 0; i<cp.size; i++) {
            Point2D center = cp.points[i];
            
            glBegin(GL_POLYGON); {
                for (int j = 0; j <= CIRCLE_RESOLUTION; j++) {
                    float angle = (float)j / CIRCLE_RESOLUTION * 2.0f * M_PI;
                    Point2D p = Point2D(center.x + CONTROL_POINT_R * cosf(angle), center.y + CONTROL_POINT_R * sinf(angle));
                    
                    //Move, Translate and convert ot GL coordinates
                    p = p.move(centerVector * -1).toGlCoordinates(screenWidthf, screenHeightf, tranVect);
                    glVertex2f(p.x,p.y);
                }
                
            } glEnd();
        }
    }
    
    glutSwapBuffers();     				// Buffercsere: rajzolas vege
}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    else if(key == ' ' && !rotateCntrPts) {
        rotateCntrPts = true;
        rotateStartTime = glutGet(GLUT_ELAPSED_TIME);
    }
}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {
    
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
        if(rotateCntrPts)
        {
            Point2D p = (Point2D(x, y) + tranVectBack).move(lastCntrVector);
            for (int i = 0; i < lastCntrPts.size; i++) {
                if (lastCntrPts.points[i].dist(p) <= CONTROL_POINT_R) {
                    centerPointNum = i;
                    centerPointSelected = true;
                }
            }
        }
        else {
            cntrPts.add(Point2D(x, y) + tranVectBack);
            glutPostRedisplay( ); // Ilyenkor rajzold ujra a kepet
        }
    }
}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{
    
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido*/
    
    if(rotateCntrPts) {
        //Calc rotateT
        rotateT = (float)((time - rotateStartTime) % ROTATE_INTERVAL) / (float)ROTATE_INTERVAL;
        glutPostRedisplay();
    }
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer
    
    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon
    
    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    
    onInitialization();					// Az altalad irt inicializalast lefuttatjuk
    
    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);
    
    glutMainLoop();					// Esemenykezelo hurok
    
    return 0;
}

