/**
 * Filename: Voronoi_polygon_set.h
 * Project name: RPVoronoi_deg2
 * Created on: 9/14/07
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _OX_OGL_H_
#define _OX_OGL_H_

// apple specific openGL includes
#ifdef __APPLE__
    #include "GLUT/glut.h"
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
#endif

#include <stdio.h>
#include <cmath>

int main_window;
void draw_objects();
void project_specifics();
void user_keyboard_press(unsigned char key, int x, int y);

/*************************************************************
                    UI
 *************************************************************/
struct OX_Glut_Global_Vars {
    // mouse
    float last_x, last_y;
    int mouse_button;

    // scale and rotate inc
    float scale_inc, rotate_inc, translate_inc;

    // world vars
    float world_scale, world_rotate_x, world_rotate_y, world_rotate_z;
    float world_translate_x, world_translate_y, world_translate_z;

    // debug info
    bool debug;

    OX_Glut_Global_Vars() { init(); }
    void init() {
    	// mouse click
        last_x = -1;
        last_y = -1;
        mouse_button = -1;

        // rotate
        rotate_inc = 2.5;
        world_rotate_x = 0;
        world_rotate_y = 0;
        world_rotate_z = 0;

        // translate
        world_translate_x = 0;
        world_translate_y = 0;
        world_translate_z = 0;
        translate_inc = 0.15;

        // scale
        scale_inc = .05;
        world_scale = 1;

        debug = 0;
    }

    std::string to_string() {
        std::ostringstream os;
        os << "last_x: " << last_x << std::endl;
        os << "last_y: " << last_y << std::endl;
        os << "mouse_button: " << mouse_button << std::endl;
        os << "rotate_inc: " << rotate_inc << std::endl;
        os << "world_rotate_x: " << world_rotate_x << std::endl;
        os << "world_rotate_y: " << world_rotate_y << std::endl;
        os << "world_rotate_z: " << world_rotate_z << std::endl;
        os << "world_translate_x: " << world_translate_x << std::endl;
        os << "world_translate_y: " << world_translate_y << std::endl;
        os << "world_translate_z: " << world_translate_z << std::endl;
        os << "translate_inc: " << translate_inc << std::endl;
        os << "scale_inc: " << scale_inc << std::endl;
        os << "world_scale: " << world_scale << std::endl;
        os << "debug: " << debug << std::endl;
        return os.str();
    }
};

OX_Glut_Global_Vars ox_gg; // ox glut global

/** function for mouse drag */
void mouse_drag(int x, int y) {
    switch(ox_gg.mouse_button) {
    case GLUT_RIGHT_BUTTON:
        // xDrag
        if (x > ox_gg.last_x) { ox_gg.world_rotate_y += ox_gg.rotate_inc; }
        else if (x < ox_gg.last_x) { ox_gg.world_rotate_y -= ox_gg.rotate_inc; }

        // yDrag
        if (y > ox_gg.last_y) { ox_gg.world_rotate_x += ox_gg.rotate_inc; }
        else if (y < ox_gg.last_y) { ox_gg.world_rotate_x -= ox_gg.rotate_inc; }
        break;
    case GLUT_LEFT_BUTTON:
        // xDrag
        if (x > ox_gg.last_x) {
            ox_gg.world_translate_x += ox_gg.translate_inc;
        } else if (x < ox_gg.last_x) {
            ox_gg.world_translate_x -= ox_gg.translate_inc;
        }

        // yDrag
        if (y > ox_gg.last_y) {
            ox_gg.world_translate_y -= ox_gg.translate_inc;
        } else if (y < ox_gg.last_y) {
            ox_gg.world_translate_y += ox_gg.translate_inc;
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        std::cout << "MIDDLE!!!" << std::endl;
        break;
    default: printf("Unknown mouse button pressed?"); break;
    };
    // set new vals
    ox_gg.last_x = x;
    ox_gg.last_y = y;
}

/** function for mouse click */
void mouse_click(int button, int state, int x, int y) {
	int specialKey = glutGetModifiers();
	// mouse state
	switch(state) {
	case GLUT_UP:
		ox_gg.last_x = -1;
		ox_gg.last_y = -1;
		ox_gg.mouse_button = -1;
		break;
	case GLUT_DOWN:
		ox_gg.last_x = x;
		ox_gg.last_y = y;
		ox_gg.mouse_button = (specialKey == GLUT_ACTIVE_CTRL) ?
				GLUT_RIGHT_BUTTON : button;
		break;
	default:
		printf("Unknown mouse button state?\n");
		break;
	};
}

void keyboard_press(unsigned char key, int x, int y) {
    switch (key) {
        case '-' : ox_gg.world_scale -= ox_gg.scale_inc; break;
        case '=' : ox_gg.world_scale += ox_gg.scale_inc; break;
        case 'm' : ox_gg.world_translate_x = -7.5;
                    ox_gg.world_translate_y = -7.5;
                    break;
        case 'o' : ox_gg.world_translate_x = 0;
                    ox_gg.world_translate_y = 0;
                    break;
        case 'd' : ox_gg.debug = !ox_gg.debug; break;
        case 'q' : exit(0);
        default  : std::cout << "key Pressed: " << key << std::endl;
                   user_keyboard_press(key, x, y);
                   break;
    }
    if (ox_gg.debug) { std::cerr << ox_gg.to_string(); }
}

void keyboard_special_press(int key, int x, int y) {
    switch (key) {
        case (GLUT_KEY_UP) : ox_gg.world_rotate_x += ox_gg.rotate_inc; return;
        case (GLUT_KEY_DOWN) : ox_gg.world_rotate_x -= ox_gg.rotate_inc; return;
        case (GLUT_KEY_RIGHT) : ox_gg.world_rotate_y += ox_gg.rotate_inc; return;
        case (GLUT_KEY_LEFT) : ox_gg.world_rotate_y -= ox_gg.rotate_inc; return;
        default  : std::cout << "special Pressed: " << key << std::endl;
    }
}


/*************************************************************
                    OpenGL Functions
 *************************************************************/
void reshape(int width, int height) {
    if ( glutGetWindow() != main_window ) { glutSetWindow(main_window);  }

    double aspectratio;

    if (height == 0) { height = 1; }
    aspectratio = width / (double) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/100, width/100, -height/100, height/100, 10, -10);
    //gluPerspective(45.0f, aspectratio, 0.0f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle(void) {
    if ( glutGetWindow() != main_window ) { glutSetWindow(main_window);  }
    glutPostRediSplay();
}

void renderDebug() {
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}


void diSplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    glRotatef(ox_gg.world_rotate_x, 1, 0, 0);
    glRotatef(ox_gg.world_rotate_y, 0, 1, 0);
    glRotatef(ox_gg.world_rotate_z, 0, 0, 1);
    glTranslatef(ox_gg.world_translate_x, ox_gg.world_translate_y,
            ox_gg.world_translate_z);
    glScalef(ox_gg.world_scale, ox_gg.world_scale, ox_gg.world_scale);

    if (ox_gg.debug) { renderDebug(); }
    draw_objects();

    glPopMatrix();
    glutSwapBuffers();
}

//void update(int value) {
//    glutPostRediSplay();
//    glutTimerFunc(25, update, 0);
//    return;
//}

void setupOGL(int argc, char** argv) {

    // set up open gl stuff
    glutInit(&argc, argv);


    glutInitDiSplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    //glutInitWindowSize(640, 480);
    //glutInitWindowSize(1024, 768);
    glutInitWindowSize(900, 900);
    //glutInitWindowSize(500, 500);

    main_window = glutCreateWindow("Viz");
    glutDiSplayFunc(diSplay);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    //glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_drag);
    glutKeyboardFunc(keyboard_press);
    glutSpecialFunc(keyboard_special_press);
    //glutTimerFunc(25, update, 0);

    project_specifics();

    glutMainLoop();
}

#endif
