#pragma once
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

#include "xbmc/libXBMC_addon.h"
#include "xbmc/libXBMC_adsp.h"
#include "xbmc/libXBMC_gui.h"

class CDSPProcess_FreeSurround;

extern std::string                   g_strUserPath;
extern std::string                   g_strAddonPath;
extern CDSPProcess_FreeSurround     *g_usedDSPs[AE_DSP_STREAM_MAX_STREAMS];
extern ADDON::CHelper_libXBMC_addon *XBMC;
extern CHelper_libXBMC_gui          *GUI;
extern CHelper_libXBMC_adsp         *ADSP;
