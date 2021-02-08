/*
 * Test program for GLUT stereo support.
 *
 * Simple though effective diagnostic test for stereo display output.
 * Test displays overlapping red and blue rectangles in stereo buffers.
 * If stereo display mode is working properly, overlap region should
 * appear magenta. It has been all too common experience that graphics
 * boards which are supposed to be stereo-ready may only work in
 * certain display modes or resolutions. This test will provide immediate
 * feedback as to whether stereo display support is actually working,
 * and does so without the need for wearing stereo glasses.
 *
 * Dave Milici, Ironic Research Labs <davem@ironicresearch.com>
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GLUT/glut.h>

#if defined(_WIN32) && (defined (WIN32_GUI) || !defined(__CONSOLE__))
#define MSGINFO(S) MessageBox(NULL, S, szAppName, MB_OK | MB_ICONINFORMATION)
#define MSGWARN(S) MessageBox(NULL, S, szAppName, MB_OK | MB_ICONWARNING)
#else
#define MSGINFO(S) printf(S)
#define MSGWARN(S) printf(S)
#endif

/* Define STEREO_UTILITY for stereo signalling utility. */
#define STEREO_UTILITY

#ifdef  STEREO_UTILITY
#include "slut.h"
#endif

/* Define FULLSCREEN for fullscreen display support using GLUT Game mode. */
#define FULLSCREEN

char szAppName[] = "GLUT Stereo Test";
char szStereoPass[] = "GLUT Stereo is supported.\n\n"
    "You should be able to see overlapping red and blue\n"
    "rectangles appear as a purple region in the following\n"
    "test window.\n";
char szStereoFail[] = "GLUT Stereo is not supported.\n\n"
    "You should check the OpenGL or compatible 3D driver\n"
    "configuration for availability of stereo support.\n";

void draw(void)
{
    /* Clear both stereo buffers */
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT);
    /* Note this does not work on ATI GL drivers, 
     * so clear each stereo buffer explicity below.
     */

    /* Draw red rectangle in left eye buffer */
    glDrawBuffer(GL_BACK_LEFT);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glRectf(-1.0, -1.0, 0.5, 0.5);
    glFlush();

    /* Draw blue rectangle in right eye buffer */
    glDrawBuffer(GL_BACK_RIGHT);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    glRectf(-0.5, -0.5, 1.0, 1.0);
    glFlush();

#ifdef STEREO_UTILITY
    SLUT_UpdateStereoSignal();
#endif

    /* Update display to show overlapping areas as magenta */
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLint) width, (GLint) height);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    /* Need to be able to exit in fullscreen game mode (Doh!) */
    switch (key) {
    case 'q':
    case 0x1B:
#ifdef STEREO_UTILITY
         SLUT_StopStereoSignal();
#endif
#ifdef FULLSCREEN
         glutLeaveGameMode();
#endif
         exit(0);
    }
}

int main(int argc, char *argv[])
{
    char displaymode[80] = "640x480:32@120";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_STEREO);

    /* Query GLUT for stereo capability -- non-stereo boards fail here. */
    if (glutGet(GLUT_DISPLAY_MODE_POSSIBLE))
        MSGINFO(szStereoPass);
    else {
        MSGWARN(szStereoFail);
        return EXIT_FAILURE;
        }

    /* Display the stereo test if supported -- stereo boards prove it here. */
#ifdef FULLSCREEN
    if (argc > 1)
        strcpy(displaymode, argv[1]);
    glutGameModeString(displaymode);
    glutEnterGameMode();
#else
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(120, 90);
    glutCreateWindow(szAppName);
#endif

#ifdef STEREO_UTILITY
    SLUT_StartStereoSignal();
#endif

    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}
