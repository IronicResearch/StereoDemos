/* Stereoscopic Library Utility Toolkit.
 *
 * A small supplementary library of utility routines for
 * supporting stereo devices which use visual signalling
 * systems for establishing left/right stereo sync.
 *
 * Utimately these signalling mechanisms could be added
 * into an underlying driver library, which would be able
 * to support more direct stereo signalling techniques
 * via IO ports or the VESA standard stereo connector.
 * Until then, this library should suffice to support
 * devices like StereoGraphics StereoEyes blue-line code
 * and IO-Display Systems H3D key frame color codes.
 *
 * Dave Milici, Ironic Research Labs.
 * <davem@ironicresearch.com>
 */

#include <GLUT/glut.h>
#include <stdlib.h>
#include <string.h>

#ifndef false
#define false			0
#endif

#ifndef true
#define true			1
#endif

#define SLUT_SIGNAL_NONE        0
#define SLUT_SIGNAL_H3D         1
#define SLUT_SIGNAL_STEREOEYES  2

/* Set default stereo signal type here, or overide as environment setting */
static int SLUT_StereoSignal = SLUT_SIGNAL_STEREOEYES;

static int bH3DSignalOn = false;
static int bH3DSignalOff = false;

static char arH3DColorOn[10]    = "RRGGYYYYGG";
static char arH3DColorOff[10]   = "RRGGYYYY  ";

/* Utility to start stereo signalling. Typically this is used
 * for displaying the start key frame color codes for H3D.
 * It may also be used for creating a separate tiny window
 * for displaying StereoGraphics blue-line code if running in
 * windowed mode.
 */
void SLUT_StartStereoSignal(void)
{
    char* devstr;

    /* Check for signal type selection overide in environment setting */
    if ((devstr = (char*)getenv("StereoDevice")) != NULL) {
        if (strcmp(devstr, "None") == 0)
            SLUT_StereoSignal = SLUT_SIGNAL_NONE;
        else if (strcmp(devstr, "H3D") == 0)
            SLUT_StereoSignal = SLUT_SIGNAL_H3D;
        else if (strcmp(devstr, "StereoEyes") == 0)
            SLUT_StereoSignal = SLUT_SIGNAL_STEREOEYES;
        else if (strncmp(devstr, "blue", 4) == 0)
            SLUT_StereoSignal = SLUT_SIGNAL_STEREOEYES;
    }

    switch (SLUT_StereoSignal) {
    case SLUT_SIGNAL_H3D:
        /* Defer H3D signal to display handler */
        bH3DSignalOn = true;
        glutPostRedisplay();
        break;
    case SLUT_SIGNAL_STEREOEYES:
        break;
    }
}

/* Utility to update stereo signalling. Typically this is used
 * for displaying StereoGraphics blue-line code while running.
 */
void SLUT_UpdateStereoSignal(void)
{
    int     w,h,i,j;
    char*   pcolor;
    GLint   mode;
    GLfloat x,y,dx,dy;
    GLfloat rgba[4];
    
    (void)x;
    (void)dx;
    
    /* Nothing to do if no stereo signal selected */
    if (SLUT_StereoSignal == SLUT_SIGNAL_NONE)
        return;

    /* GLUT Game mode display change may only be Win32-specific */
    if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE)
        && glutGameModeGet(GLUT_GAME_MODE_DISPLAY_CHANGED)) {
        w = glutGameModeGet(GLUT_GAME_MODE_WIDTH);
        h = glutGameModeGet(GLUT_GAME_MODE_HEIGHT);
    }
    /* GLUT screen display dimensions should always be valid */
    if (w <= 0 || h <= 0) {
        w = glutGet(GLUT_SCREEN_WIDTH);
        h = glutGet(GLUT_SCREEN_HEIGHT);
    }

    /* Apple stereo example saves color state info */
    glGetFloatv(GL_CURRENT_COLOR, &rgba[0]);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_COLOR_LOGIC_OP);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);

    switch (SLUT_StereoSignal) {
    case SLUT_SIGNAL_H3D:
        /* Nothing to do if signal already displayed */
        if (!bH3DSignalOn && !bH3DSignalOff)
            return;
        /* Save current matrix state */
        glGetIntegerv(GL_MATRIX_MODE, &mode);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        /* Select color table corresponding to on or off signal */
        pcolor = (bH3DSignalOn) ? &arH3DColorOn[0] : &arH3DColorOff[0];
        /* H3D color frames must be displayed for 30 fields */
        for (j=0; j<30; j++) {
            /* H3D color bars may be drawn 2 lines at a time */
            glDrawBuffer(GL_BACK);
            dy = 2.0/(GLfloat)h;
            for (i=0; i<10; i+=1) {
                switch (pcolor[i]) {
                case 'R': glColor3f(1.0, 0.0, 0.0); break;
                case 'G': glColor3f(0.0, 1.0, 0.0); break;
                case 'B': glColor3f(1.0, 0.0, 1.0); break;
                case 'Y': glColor3f(1.0, 1.0, 0.0); break;
                case ' ':
                default : glColor3f(0.0, 0.0, 0.0); break;
                }
                y = 1.0 - i*dy;
                glRectf(-1.0, y-dy, 1.0, y);
                glFlush();
            }
            /* Clear the rest of the frame to black */
            glColor3f(0.0, 0.0, 0.0);
            glRectf(-1.0, -1.0, 1.0, y-dy);
            glFlush();
            glutSwapBuffers();
        }
        /* H3D color frames must be followed by black */
        for (j=0; j<2; j++) {
            glColor3f(0.0, 0.0, 0.0);
            glRectf(-1.0, -1.0, 1.0, 1.0);
            glFlush();
            glutSwapBuffers();
        }
        /* Restore previous matrix state */
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(mode);
        /* Reset H3D signal flag */
        bH3DSignalOn =
        bH3DSignalOff = false;
        break;
    case SLUT_SIGNAL_STEREOEYES:
        /* Save current matrix state */
        glGetIntegerv(GL_MATRIX_MODE, &mode);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        /* Draw 25% blue line in left eye buffer */
        glDrawBuffer(GL_BACK_LEFT);
        glColor3f(0.0, 0.0, 1.0);
        glRectf(-1.0, -1.0, -0.5, -0.99);
        glFlush();
        glColor3f(0.0, 0.0, 0.0);
        glRectf(-1.0, -1.0, -0.5, -0.99);
        glFlush();
        /* Draw 75% blue line in right eye buffer */
        glDrawBuffer(GL_BACK_RIGHT);
        glColor3f(0.0, 0.0, 1.0);
        glRectf(-1.0, -1.0, 0.5, -0.99);
        glFlush();
        glColor3f(0.0, 0.0, 0.0);
        glRectf(-1.0, -1.0, 0.5, -0.99);
        glFlush();
        /* Restore previous matrix state */
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(mode);
        break;
    }
    
    /* Restore color state info */
    glPopAttrib();
    glColor4fv(rgba);
}

/* Utility to stop stereo signalling. Typically this is used
 * for displaying the stop key frame color codes for H3D.
 * It may also be used for destroying the separate window
 * for displaying StereoGraphics blue-line code.
 */
void SLUT_StopStereoSignal(void)
{
    switch (SLUT_StereoSignal) {
    case SLUT_SIGNAL_H3D:
        /* Call H3D signal display handler immediately (prior to exit) */
        bH3DSignalOff = true;
        SLUT_UpdateStereoSignal();
        break;
    case SLUT_SIGNAL_STEREOEYES:
        break;
    }
}



