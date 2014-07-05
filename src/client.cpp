/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <vector>
#include <string>
#include "client.h"
#include "xbmc/xbmc_adsp_dll.h"
#include "xbmc/util/util.h"
#include "xbmc/util/StdString.h"
#include "GUIDialogFreeSurround.h"
#include "DSPProcessFreeSurround.h"

using namespace std;
using namespace ADDON;

#ifdef TARGET_WINDOWS
#define snprintf _snprintf
#endif


/* User adjustable settings are saved here.
 * Default values are defined inside client.h
 * and exported to the other source files.
 */
std::string                                     g_strUserPath       = "";
std::string                                     g_strAddonPath      = "";
CDSPProcess_FreeSurround                       *g_usedDSPs[AE_DSP_STREAM_MAX_STREAMS];
CHelper_libXBMC_addon                          *XBMC                = NULL;
CHelper_libXBMC_adsp                           *ADSP                = NULL;
CHelper_libXBMC_gui                            *GUI                 = NULL;
ADDON_STATUS                                    m_CurStatus         = ADDON_STATUS_UNKNOWN;
AE_DSP_MENUHOOK                                 m_MenuHook;
struct AE_DSP_MODES::AE_DSP_MODE  m_ModeInfoStruct;

extern "C" {

void ADDON_ReadSettings(void)
{
}

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!hdl || !props)
    return ADDON_STATUS_UNKNOWN;

  AE_DSP_PROPERTIES* adspprops = (AE_DSP_PROPERTIES*)props;

  XBMC = new CHelper_libXBMC_addon;
  if (!XBMC->RegisterMe(hdl))
  {
    SAFE_DELETE(XBMC);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  GUI = new CHelper_libXBMC_gui;
  if (!GUI->RegisterMe(hdl))
  {
    SAFE_DELETE(GUI);
    SAFE_DELETE(XBMC);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  ADSP = new CHelper_libXBMC_adsp;
  if (!ADSP->RegisterMe(hdl))
  {
    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(XBMC);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  XBMC->Log(LOG_DEBUG, "%s - Creating the Audio DSP free surround add-on", __FUNCTION__);

  m_CurStatus     = ADDON_STATUS_UNKNOWN;
  g_strUserPath   = adspprops->strUserPath;
  g_strAddonPath  = adspprops->strAddonPath;

  for (int i = 0; i < AE_DSP_STREAM_MAX_STREAMS; i++)
    g_usedDSPs[i] = NULL;

  CDSPSettings settings;
  settings.LoadSettingsData(true);

  std::string imagePath;

  m_ModeInfoStruct.iUniqueDBModeId        = -1;         // set by RegisterMode
  m_ModeInfoStruct.iModeType              = AE_DSP_MODE_TYPE_MASTER_PROCESS;
  m_ModeInfoStruct.iModeNumber            = ID_MASTER_PROCESS_FREE_SURROUND;
  m_ModeInfoStruct.bHasSettingsDialog     = true;
  m_ModeInfoStruct.iModeDescription       = 30002;
  m_ModeInfoStruct.iModeHelp              = 30003;
  m_ModeInfoStruct.iModeName              = 30000;
  m_ModeInfoStruct.iModeSetupName         = 30001;
  m_ModeInfoStruct.iModeSupportTypeFlags  = AE_DSP_PRSNT_ASTREAM_BASIC | AE_DSP_PRSNT_ASTREAM_MUSIC | AE_DSP_PRSNT_ASTREAM_MOVIE;
  m_ModeInfoStruct.bIsDisabled            = false;

  strncpy(m_ModeInfoStruct.strModeName, "Free Surround", sizeof(m_ModeInfoStruct.strModeName) - 1);
  imagePath = g_strAddonPath;
  imagePath += "/resources/skins/Confluence/media/adsp-freesurround.png";
  strncpy(m_ModeInfoStruct.strOwnModeImage, imagePath.c_str(), sizeof(m_ModeInfoStruct.strOwnModeImage) - 1);
  memset(m_ModeInfoStruct.strOverrideModeImage, 0, sizeof(m_ModeInfoStruct.strOwnModeImage)); // unused
  ADSP->RegisterMode(&m_ModeInfoStruct);

  m_MenuHook.iHookId                      = ID_MASTER_PROCESS_FREE_SURROUND;
  m_MenuHook.category                     = AE_DSP_MENUHOOK_MASTER_PROCESS;
  m_MenuHook.iLocalizedStringId           = 30001;
  m_MenuHook.iRelevantModeId              = ID_MASTER_PROCESS_FREE_SURROUND;
  m_MenuHook.bNeedPlayback                = true;
  ADSP->AddMenuHook(&m_MenuHook);

  m_CurStatus = ADDON_STATUS_OK;

  return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  for (int i = 0; i < AE_DSP_STREAM_MAX_STREAMS; i++)
    SAFE_DELETE(g_usedDSPs[i]);

  SAFE_DELETE(ADSP);
  SAFE_DELETE(GUI);
  SAFE_DELETE(XBMC);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

bool ADDON_HasSettings()
{
  return true;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  (void) sSet; // Remove compiler warning
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue)
{
  (void) settingName; (void) settingValue;
  return ADDON_STATUS_OK;
}

void ADDON_Stop()
{
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
  (void) flag; (void) sender; (void) message; (void) data; // Remove compiler warning
}

const char* GetAudioDSPAPIVersion(void)
{
  static const char *strApiVersion = XBMC_AE_DSP_API_VERSION;
  return strApiVersion;
}

const char* GetMininumAudioDSPAPIVersion(void)
{
  static const char *strMinApiVersion = XBMC_AE_DSP_MIN_API_VERSION;
  return strMinApiVersion;
}

const char* GetGUIAPIVersion(void)
{
  static const char *strGuiApiVersion = XBMC_GUI_API_VERSION;
  return strGuiApiVersion;
}

const char* GetMininumGUIAPIVersion(void)
{
  static const char *strMinGuiApiVersion = XBMC_GUI_MIN_API_VERSION;
  return strMinGuiApiVersion;
}

AE_DSP_ERROR GetAddonCapabilities(AE_DSP_ADDON_CAPABILITIES* pCapabilities)
{
  pCapabilities->bSupportsInputProcess    = false;
  pCapabilities->bSupportsPreProcess      = false;
  pCapabilities->bSupportsInputResample   = false;
  pCapabilities->bSupportsMasterProcess   = true;
  pCapabilities->bSupportsPostProcess     = false;
  pCapabilities->bSupportsOutputResample  = false;

  return AE_DSP_ERROR_NO_ERROR;
}

const char *GetDSPName(void)
{
  static const char *strBackendName = "Free Surround Processor";
  return strBackendName;
}

const char *GetDSPVersion(void)
{
  static CStdString strDSPVersion = "0.1";
  return strDSPVersion.c_str();
}

const char *GetConnectionString(void)
{
  static const char *emptyStr = "";
  return emptyStr;
}

AE_DSP_ERROR CallMenuHook(const AE_DSP_MENUHOOK &menuhook, const AE_DSP_MENUHOOK_DATA &item)
{
  if (menuhook.iHookId == ID_MASTER_PROCESS_FREE_SURROUND)
  {
    CGUIDialogFreeSurround settings(item.data.iStreamId);
    settings.DoModal();
    return AE_DSP_ERROR_NO_ERROR;
  }
  return AE_DSP_ERROR_UNKNOWN;
}

AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *addonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties)
{
  if (g_usedDSPs[addonSettings->iStreamID])
  {
    delete g_usedDSPs[addonSettings->iStreamID];
    g_usedDSPs[addonSettings->iStreamID] = NULL;
  }

  CDSPProcess_FreeSurround *proc = new CDSPProcess_FreeSurround(addonSettings->iStreamID);
  AE_DSP_ERROR err = proc->StreamCreate(addonSettings, pProperties);
  if (err == AE_DSP_ERROR_NO_ERROR)
    g_usedDSPs[addonSettings->iStreamID] = proc;
  else
    delete proc;

  return err;
}

AE_DSP_ERROR StreamDestroy(AE_DSP_STREAM_ID id)
{
  AE_DSP_ERROR err = AE_DSP_ERROR_UNKNOWN;
  if (g_usedDSPs[id])
  {
    err = g_usedDSPs[id]->StreamDestroy();
    delete g_usedDSPs[id];
    g_usedDSPs[id] = NULL;
  }
  return err;
}

AE_DSP_ERROR StreamInitialize(const AE_DSP_SETTINGS *settings)
{
  AE_DSP_ERROR err = AE_DSP_ERROR_UNKNOWN;

  if (g_usedDSPs[settings->iStreamID])
    err = g_usedDSPs[settings->iStreamID]->StreamInitialize(settings);

  return err;
}

AE_DSP_ERROR StreamIsModeSupported(AE_DSP_STREAM_ID id, AE_DSP_MODE_TYPE type, unsigned int mode_id, int unique_db_mode_id)
{
  return g_usedDSPs[id]->StreamIsModeSupported(type, mode_id, unique_db_mode_id);
}

AE_DSP_ERROR MasterProcessSetMode(AE_DSP_STREAM_ID id, AE_DSP_STREAMTYPE type, unsigned int client_mode_id, int unique_db_mode_id)
{
  (void) id; (void) type; (void) client_mode_id; (void) unique_db_mode_id;
  return AE_DSP_ERROR_NO_ERROR;
}

unsigned int MasterProcessNeededSamplesize(AE_DSP_STREAM_ID id)
{
  (void) id;
  return 0;
}

float MasterProcessGetDelay(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->StreamGetDelay();
}

unsigned int MasterProcess(AE_DSP_STREAM_ID id, float **array_in, float **array_out, unsigned int samples)
{
  return g_usedDSPs[id]->StreamProcess(array_in, array_out, samples);
}

int MasterProcessGetOutChannels(AE_DSP_STREAM_ID id, unsigned long &out_channel_present_flags)
{
  return g_usedDSPs[id]->StreamGetOutChannels(out_channel_present_flags);
}

const char *MasterProcessGetStreamInfoString(AE_DSP_STREAM_ID id)
{
  (void) id;
  return "";
}

/*!
 * Unused DSP addon functions
 */
bool InputProcess(AE_DSP_STREAM_ID id, const float **array_in, unsigned int samples) { (void) id; (void) array_in; (void) samples; return true; }
unsigned int InputResampleProcessNeededSamplesize(AE_DSP_STREAM_ID id) { (void) id; return 0; }
int InputResampleSampleRate(AE_DSP_STREAM_ID id) { (void) id; return 0; }
float InputResampleGetDelay(AE_DSP_STREAM_ID id) { (void) id; return 0.0; }
unsigned int InputResampleProcess(AE_DSP_STREAM_ID id, float **array_in, float **array_out, unsigned int samples) { (void) id; (void) array_in; (void) array_out; (void) samples;  return 0; }
unsigned int PreProcessNeededSamplesize(AE_DSP_STREAM_ID id, unsigned int mode_id) { (void) id; (void) mode_id; return 0; }
float PreProcessGetDelay(AE_DSP_STREAM_ID id, unsigned int mode_id) { (void) id; (void) mode_id; return 0.0; }
unsigned int PreProcess(AE_DSP_STREAM_ID id, unsigned int mode_id, float **array_in, float **array_out, unsigned int samples) { (void) id; (void) mode_id; (void) array_in; (void) array_out; (void) samples;  return 0; }
unsigned int PostProcessNeededSamplesize(AE_DSP_STREAM_ID id, unsigned int mode_id) { (void) id; (void) mode_id; return 0; }
float PostProcessGetDelay(AE_DSP_STREAM_ID id, unsigned int mode_id) { (void) id; (void) mode_id; return 0.0; }
unsigned int PostProcess(AE_DSP_STREAM_ID id, unsigned int mode_id, float **array_in, float **array_out, unsigned int samples) { (void) id; (void) mode_id; (void) array_in; (void) array_out; (void) samples;  return 0; }
unsigned int OutputResampleProcessNeededSamplesize(AE_DSP_STREAM_ID id) { (void) id; return 0; }
int OutputResampleSampleRate(AE_DSP_STREAM_ID id) { (void) id; return 0; }
float OutputResampleGetDelay(AE_DSP_STREAM_ID id) { (void) id; return 0; }
unsigned int OutputResampleProcess(AE_DSP_STREAM_ID id, float **array_in, float **array_out, unsigned int samples) { (void) id; (void) array_in; (void) array_out; (void) samples;  return 0; }

}
