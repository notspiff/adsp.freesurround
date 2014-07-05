#pragma once
/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://www.xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "xbmc/util/StdString.h"
#include "client.h"
#include "FreeSurroundSettings.h"

class CGUIDialogFreeSurround : private CDSPSettings
{
public:
  CGUIDialogFreeSurround(unsigned int streamId);
  virtual ~CGUIDialogFreeSurround();

  bool Show();
  void Close();
  void DoModal();

private:
  bool OnClick(int controlId);
  bool OnFocus(int controlId);
  bool OnInit();
  bool OnAction(int actionId);

  static bool OnClickCB(GUIHANDLE cbhdl, int controlId);
  static bool OnFocusCB(GUIHANDLE cbhdl, int controlId);
  static bool OnInitCB(GUIHANDLE cbhdl);
  static bool OnActionCB(GUIHANDLE cbhdl, int actionId);

  const unsigned int               m_StreamId;

  CAddonGUISettingsSliderControl   *m_CircularWrap;
  CAddonGUISettingsSliderControl   *m_Shift;
  CAddonGUISettingsSliderControl   *m_Depth;
  CAddonGUISettingsSliderControl   *m_Focus;
  CAddonGUISettingsSliderControl   *m_CenterImage;
  CAddonGUISettingsSliderControl   *m_FrontSeparation;
  CAddonGUISettingsSliderControl   *m_RearSeparation;

  CAddonGUIWindow                  *m_window;
};
