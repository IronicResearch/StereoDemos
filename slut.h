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

void SLUT_StartStereoSignal(void);
void SLUT_UpdateStereoSignal(void);
void SLUT_StopStereoSignal(void);

