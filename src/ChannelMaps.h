/*
 *      Copyright (C) 2014 Team XBMC
 *      Copyright (C) 2007-2010 Christian Kothe
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

/*
 * passed upon fsurround available under
 * http://www.hydrogenaud.io/forums/index.php?showtopic=52235
 * written by pro_optimizer
 */

#ifndef CHANNELMAPS_H
#define CHANNELMAPS_H

#include <vector>
#include <map>

#include "xbmc/xbmc_adsp_types.h"

/*
 * Identifier translation from FreeSurround to XBMC ADSP
 *
 * ci_front_left          = AE_DSP_PRSNT_CH_FL
 * ci_front_center_left   = AE_DSP_PRSNT_CH_FLOC
 * ci_front_center        = AE_DSP_PRSNT_CH_FC
 * ci_front_center_right  = AE_DSP_PRSNT_CH_FROC
 * ci_front_right         = AE_DSP_PRSNT_CH_FR
 * ci_side_front_left     = AE_DSP_PRSNT_CH_TFL
 * ci_side_front_right    = AE_DSP_PRSNT_CH_TFR
 * ci_side_center_left    = AE_DSP_PRSNT_CH_SL
 * ci_side_center_right   = AE_DSP_PRSNT_CH_SR
 * ci_side_back_left      = AE_DSP_PRSNT_CH_TBL
 * ci_side_back_right     = AE_DSP_PRSNT_CH_TBR
 * ci_back_left           = AE_DSP_PRSNT_CH_BL
 * ci_back_center_left    = AE_DSP_PRSNT_CH_BLOC
 * ci_back_center         = AE_DSP_PRSNT_CH_BC
 * ci_back_center_right   = AE_DSP_PRSNT_CH_BROC
 * ci_back_right          = AE_DSP_PRSNT_CH_BR
 * ci_lfe                 = AE_DSP_PRSNT_CH_LFE
 */

const int grid_res = 21; // resolution of the lookup grid

// channel allocation maps (per setup)
typedef std::vector<std::vector<float*> > alloc_lut;
extern std::map<unsigned,alloc_lut> chn_alloc;
// channel metadata maps (per setup)
extern std::map<unsigned,std::vector<float> > chn_angle;
extern std::map<unsigned,std::vector<float> > chn_xsf;
extern std::map<unsigned,std::vector<float> > chn_ysf;
extern std::map<unsigned,std::vector<AE_DSP_CHANNEL_PRESENT> > chn_id;
extern bool init_maps();

#endif

