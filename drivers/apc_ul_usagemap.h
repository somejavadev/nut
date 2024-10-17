/*
 * apc_ul_usagemap.h - APC Microlink driver "usage" (exposed variable) path 
 *                     definitions and mappings from usages to NUT directives 
 *                     and instcmds
 *
 *  Copyright (C) 
 *    2024 Thomas Eldon <tom@tomeldon.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef NUT_APC_UL_USAGEMAP_H_SEEN
#define NUT_APC_UL_USAGEMAP_H_SEEN 1

#include "apc_ul_util.h"

/* FQUNs used directly in the driver */
extern const fqid_t FQUN_MasterPassword[MAX_FQIDS];
extern const fqid_t FQUN_SerialNumber[MAX_FQIDS];
extern const fqid_t FQUN_SlavePassword[MAX_FQIDS];
extern const fqid_t FQUN_UPSStatus_BF[MAX_FQIDS];
extern const fqid_t FQUN_BatterySystemError_BF[MAX_FQIDS];
extern const fqid_t FQUN_RuntimeCalibrationStatus[MAX_FQIDS];
extern const fqid_t FQUN_SimpleSignalingStatus_BF[MAX_FQIDS];
extern const fqid_t FQUN_InputStatus_BF[MAX_FQIDS];
extern const fqid_t FQUN_OutletCommand[MAX_FQIDS];


/* Mapped usage arrays iterated in the driver */
extern mapped_usage_t* switched_outlet_group_usages[];
extern int switched_outlet_group_usages_length;

extern mapped_usage_t* updated_usages[];
extern int updated_usages_length;

extern mapped_command_t* instcmds[];
extern int instcmds_length;

#endif