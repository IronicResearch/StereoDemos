# StereoDemos
Stereo Demos

Archive of OpenGL demos ported for stereo viewing by DaveM.
Originally created as porting examples for StereoGraphics & SciTech MGL SDKs.
All demos re-ported to GLUT with OEM-specific references removed or relocated.

Each OpenGL example sets up a viewpoint perspective amenable to stereo viewing.
Stereo viewing applies two sets of shifts along the x-axis, one for shifting the
viewpoint for the left or right eyes, and another shift for adjusting the 
parallax point for comfortable stereo viewing. These two shifts correspond to the
classic stereographer's parallel-projection viewing model, where the viewpoint
shift mimics the separation of left/right cameras, and the adjustment shift
mimics the overlap of left/right images viewed by the stereoscope device.

Many OpenGL apps use `glFrustum()` for setting up the scene's viewpoint perspective.
So the viewpoint shift is commonly added as a `glTranslate()` call after `glFrustum()`,
and the image adjustment can be included as part of the `glFrustum()` plane boundaries.
Then whenever the OpenGL scene gets rendered, the pair of stereo views can be rendered
corresponding to the left or right eye viewpoint. OpenGL libraries internally cache the
viewing matrix as a single concatenated 4x4 matrix, so the two x-axis shifts ultimately
affect the 3rd and 4th elements of the 4x4 viewing matrix. Then an optimized variation
of the 2-pass stereo rendering loop could take advantage of cached matrix elements
via `glLoadMatrix()` calls.

Simplified 2-pass rendering loop unrolled for explanation:

```
	glDrawBuffer(GL_BACK_LEFT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left-dxAdjust, right-dxAdjust, top, bottom, near, far);
	glTranslatef(dxViewpt, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	// render scene for left eye view
	// ...
	glFlush();

	glDrawBuffer(GL_BACK_RIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left+dxAdjust, right+dxAdjust, top, bottom, near, far);
	glTranslatef(-dxViewpt, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	// render scene for right eye view
	// ...
	glFlush();

	glutSwapBuffers();
```

The *Gears* demo by Brian Paul is commonly distributed as part of the Mesa library,
an OpenGL-compatible library usually installed on Linux X11 desktops.

The *Skyfly* demo by Mark Kilgard is commonly found in GLUT distributions, as
described in "The Green Book", *OpenGL Programming for the X Window System*.

Other demos are usually found with GLUT distributions as part of "The Red Book"
examples (referring to the *OpenGL Programmer's Guide*).

Some of these GLUT demos were once part of the Win32 GLUT library distribution for
Windows, so some makefiles are compatible with Visual C `nmake`.
```
nmake -f makefile.win
```
These GLUT demos were also included as part of the SciTech MGL SDK at one point, so some
makefiles are compatible with multi-platform `dmake` with appropriate compiler settings.
```
dmake -f makefile.dmk
```
