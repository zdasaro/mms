#include <GL/freeglut.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "sim/Constants.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/MazeFileUtilities.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/Parameters.h"
#include "sim/Tile.h"
#include "algo/Solver.h"

// Function declarations
void draw();
void solve();
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
Solver* g_solver;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;

// Global primitive variable declarations
static /*non-const*/ bool PAUSED = false; // Initially set to false
static /*non-const*/ int SLEEP_TIME = 150; // ms between simulation steps

int main(int argc, char* argv[]){
    
    // TODO: read input file, and check size, use this as value instead of parameter

    // Ensure that the size parameters are valid
    if (sim::MAZE_WIDTH < 1 || sim::MAZE_HEIGHT < 1){
        std::cout << "Impossible maze size - check \"src/sim/Parameters.h\"" << std::endl;
        return 0;
    }

    // Initialize local simulation objects
    sim::Maze maze(sim::MAZE_WIDTH, sim::MAZE_HEIGHT, sim::getMazeFileDirPath(argv[0]), sim::MAZE_FILE);
    sim::Mouse mouse(&maze);
    sim::MouseInterface mouseInterface(&mouse, &SLEEP_TIME, &PAUSED);
    Solver solver(&mouseInterface);

    // Initialize the local graphics objects
    sim::MazeGraphic mazeGraphic(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);

    // Assign global variables
    g_solver = &solver;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitWindowSize(sim::WINDOW_WIDTH, sim::WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(sim::MAZE_FILE.c_str());
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyInput);

    // Start the solving loop (the name "first" is not important)
    std::thread first(solve);
    
    // Start the graphics loop (which never terminates)
    glutMainLoop();
}

// Draw method for the GLUT environment
void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    g_mazeGraphic->draw();
    g_mouseGraphic->draw();
    glFlush();
    usleep(1000*sim::SLEEP_TIME_MIN); // Reduces CPU usage
    glutPostRedisplay();
}

void solve(){
    usleep(1000*500); // Wait for 0.5 seconds for GLUT to intialize
    g_solver->solve();
}

void keyInput(unsigned char key, int x, int y){
    if (key == 32){ // Space bar
        PAUSED = !PAUSED;
    }
    else if (key == 'f' || key == 'F'){ // Faster
        SLEEP_TIME /= 1.15;
        if (SLEEP_TIME < sim::SLEEP_TIME_MIN){
            SLEEP_TIME = sim::SLEEP_TIME_MIN;
        }
    }
    else if (key == 's' || key == 'S'){ // Slower
        SLEEP_TIME *= 1.2;
        if (SLEEP_TIME > sim::SLEEP_TIME_MAX){
            SLEEP_TIME = sim::SLEEP_TIME_MAX;
        }
    }
    else if (key == 'q' || key == 'Q'){ // Quit
        exit(0);
    }
}
