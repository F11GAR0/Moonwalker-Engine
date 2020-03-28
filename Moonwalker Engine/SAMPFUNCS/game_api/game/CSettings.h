/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        sdk/game/CSettings.h
*  PURPOSE:     Game settings interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_SETTINGS
#define __CGAME_SETTINGS

#if (!defined(RWFORCEENUMSIZEINT))
#define RWFORCEENUMSIZEINT ((int)((~((unsigned int)0))>>1))
#endif /* (!defined(RWFORCEENUMSIZEINT)) */

enum VideoModeFlag // RwVideoModeFlag
{
    rwVIDEOMODEEXCLUSIVE    = 0x0001, /**<Exclusive (i.e. full-screen) */
    rwVIDEOMODEINTERLACE    = 0x0002, /**<Interlaced                   */
    rwVIDEOMODEFFINTERLACE  = 0x0004, /**<Flicker Free Interlaced      */

    /* Platform specific video mode flags. */

    rwVIDEOMODE_PS2_FSAASHRINKBLIT    = 0x0100,
    /**< \if sky2
     *   Full-screen antialiasing mode 0
     *   \endif
     */
    rwVIDEOMODE_PS2_FSAAREADCIRCUIT   = 0x0200,
    /**< \if sky2
     *   Full-screen antialiasing mode 1
     *   \endif
     */

    rwVIDEOMODE_XBOX_WIDESCREEN       = 0x0100,
    /**< \if xbox
     *   Wide screen.
     *   \endif
     */
    rwVIDEOMODE_XBOX_PROGRESSIVE      = 0x0200,
    /**< \if xbox
     *   Progressive.
     *   \endif
     */
    rwVIDEOMODE_XBOX_FIELD            = 0x0400,
    /**< \if xbox
     *   Field rendering.
     *   \endif
     */
    rwVIDEOMODE_XBOX_10X11PIXELASPECT = 0x0800,
    /**< \if xbox
     *   The frame buffer is centered on the display.
     *   On a TV that is 704 pixels across, this would leave 32 pixels of black
     *   border on the left and 32 pixels of black border on the right.
     *   \endif
     */

    rwVIDEOMODEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

struct VideoMode //RwVideoMode
{
    int             width;   /**< Width  */
    int             height;  /**< Height */
    int             depth;   /**< Depth  */
    VideoModeFlag   flags;   /**< Flags  */
    int             refRate; /**< Approximate refresh rate */
    int             format;  /**< Raster format
                                * \see RwRasterFormat 
                                */
};

enum eAspectRatio
{
    ASPECT_RATIO_AUTO,
    ASPECT_RATIO_4_3,
    ASPECT_RATIO_16_10,
    ASPECT_RATIO_16_9,
};

enum eRadarMode
{
    RADAR_MODE_ALL = 0,
    RADAR_MODE_BLIPS_ONLY,
    RADAR_MODE_NO_HUD
};

class CGameSettings
{
public:
    virtual bool            IsWideScreenEnabled     ( void ) = 0;
    virtual void            SetWideScreenEnabled    ( bool bEnabled ) = 0;
    virtual unsigned int    GetNumVideoModes        ( void ) = 0;
    virtual VideoMode *     GetVideoModeInfo        ( VideoMode * modeInfo, unsigned int modeIndex ) = 0;
    virtual unsigned int    GetCurrentVideoMode     ( void ) = 0;
    virtual void            SetCurrentVideoMode     ( unsigned int modeIndex, bool bOnRestart ) = 0;
    virtual unsigned int    GetNumAdapters          ( void ) = 0;
    virtual unsigned int    GetCurrentAdapter       ( void ) = 0;
    virtual bool            HasUnsafeResolutions    ( void ) = 0;
    virtual bool            IsUnsafeResolution      ( int iWidth, int iHeight ) = 0;
    virtual void            SetAdapter              ( unsigned int uiAdapterIndex ) = 0;
    virtual unsigned char   GetRadioVolume          ( void ) = 0;
    virtual void            SetRadioVolume          ( unsigned char ucVolume ) = 0;
    virtual unsigned char   GetSFXVolume            ( void ) = 0;
    virtual void            SetSFXVolume            ( unsigned char ucVolume ) = 0;
    virtual unsigned int    GetUsertrackMode        ( void ) = 0;
    virtual void            SetUsertrackMode        ( unsigned int uiMode ) = 0;
    virtual bool            IsUsertrackAutoScan     ( void ) = 0;
    virtual void            SetUsertrackAutoScan    ( bool bEnable ) = 0;
    virtual bool            IsRadioEqualizerEnabled ( void ) = 0;
    virtual void            SetRadioEqualizerEnabled( bool bEnable ) = 0;
    virtual bool            IsRadioAutotuneEnabled  ( void ) = 0;
    virtual void            SetRadioAutotuneEnabled ( bool bEnable ) = 0;

    virtual float           GetDrawDistance         ( void ) = 0;
    virtual void            SetDrawDistance         ( float fDrawDistance ) = 0;

    virtual unsigned int    GetBrightness           ( void ) = 0;
    virtual void            SetBrightness           ( unsigned int uiBrightness ) = 0;

    virtual unsigned int    GetFXQuality            ( void ) = 0;
    virtual void            SetFXQuality            ( unsigned int fxQualityId ) = 0;

    virtual float           GetMouseSensitivity     ( void ) = 0;
    virtual void            SetMouseSensitivity     ( float fSensitivity ) = 0;

    virtual unsigned int    GetAntiAliasing         ( void ) = 0;
    virtual void            SetAntiAliasing         ( unsigned int uiAntiAliasing, bool bOnRestart ) = 0;

    virtual bool            IsMipMappingEnabled     ( void ) = 0;
    virtual void            SetMipMappingEnabled    ( bool bEnable ) = 0;

    virtual bool            IsVolumetricShadowsEnabled     ( void ) = 0;
    virtual void            SetVolumetricShadowsEnabled    ( bool bEnable ) = 0;
    virtual void            SetVolumetricShadowsSuspended  ( bool bSuspended ) = 0;

    virtual float           GetAspectRatioValue     ( void ) = 0;
    virtual eAspectRatio    GetAspectRatio          ( void ) = 0;
    virtual void            SetAspectRatio          ( eAspectRatio aspectRatio, bool bAdjustmentEnabled = true ) = 0;

    virtual bool            IsGrassEnabled          ( void ) = 0;
    virtual void            SetGrassEnabled         ( bool bEnable ) = 0;

    virtual eRadarMode      GetRadarMode            ( void ) = 0;
    virtual void            SetRadarMode            ( eRadarMode hudMode ) = 0;

    virtual void            Save                    ( void ) = 0;

	virtual struct CSettingsSAInterface*	GetInterface(void) = 0;
};

#endif
