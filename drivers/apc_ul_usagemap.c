/*
 * apc_ul_usagemap.c - APC Microlink driver "usage" (exposed variable) path 
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

#include "apc_ul_usagemap.h"

/*----------------------------------------------------------------
 |
 |            Exposed variable (usage) path definitions
 |
 -----------------------------------------------------------------*/

/* 
 * Zero (0) is used as the escape byte 
 * in these for calculating length, if
 * no zero found, it uses MAX_FQIDS 
 */

/*
		Root usages
*/
const fqid_t FQUN_UPSStatus_BF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x0A, fqid_usage));

const fqid_t FQUN_UPSStatusChangeReason[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x0B, fqid_usage));

const fqid_t FQUN_UPSCommandBF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x14, fqid_usage));

const fqid_t FQUN_BatterySystemError_BF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x34, fqid_usage));

const fqid_t FQUN_OutletCommand[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0xB5, fqid_usage));

const fqid_t FQUN_NameString[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x82, fqid_usage));

/*
		Communication related
*/

const fqid_t FQUN_SlavePassword[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x08, fqid_collection),
	FQID(0x05, fqid_usage));

const fqid_t FQUN_MasterPassword[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x08, fqid_collection),
	FQID(0x06, fqid_usage));

const fqid_t FQUN_SimpleSignalingStatus_BF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x08, fqid_collection),
	FQID(0x07, fqid_usage));

/*
		Battery related
*/

const fqid_t FQUN_ReplaceBatteryTestStatus[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x11, fqid_usage));

const fqid_t FQUN_RuntimeCalibrationStatus[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x13, fqid_usage));

const fqid_t FQUN_BatteryTemp[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x22, fqid_usage));

const fqid_t FQUN_BatteryChargePct[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x20, fqid_usage));

const fqid_t FQUN_BatteryVoltageDC[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x21, fqid_usage));

const fqid_t FQUN_BatteryInstallDate[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x48, fqid_usage));

const fqid_t FQUN_BatteryReplaceDate[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x19, fqid_usage));

const fqid_t FQUN_BatteryTestInterval[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x18, fqid_usage));

const fqid_t FQUN_BatteryRuntimeRemaining[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x24, fqid_usage));

const fqid_t FQUN_BatteryRuntimeRemaining4B[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x9F, fqid_usage));

const fqid_t FQUN_ReplaceBatteryTestCommand[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x10, fqid_usage));

const fqid_t FQUN_RuntimeCalibrationCommand[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x05, fqid_collection),
	FQID(0x12, fqid_usage));

/*
		Input related
*/

const fqid_t FQUN_InputStatus_BF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x06, fqid_collection),
	FQID(0x16, fqid_usage));

const fqid_t FQUN_InputFrequency[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x06, fqid_collection),
	FQID(0x27, fqid_usage));

const fqid_t FQUN_InputVoltageAC[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x06, fqid_collection),
	FQID(0x25, fqid_usage));

const fqid_t FQUN_InputEfficiency[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x06, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID(0x17, fqid_usage));

/*
		Manufacturer info
*/

const fqid_t FQUN_SerialNumber[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x40, fqid_usage));

const fqid_t FQUN_ModelName[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x44, fqid_usage));

const fqid_t FQUN_ManufacturingDate[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x19, fqid_usage));

const fqid_t FQUN_FwString1[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID_INDEX(0x10, fqid_collection, 0, 4),
	FQID(0x02, fqid_namespace),
	FQID(0x4A, fqid_usage), );

const fqid_t FQUN_FwString2[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID_INDEX(0x10, fqid_collection, 1, 4),
	FQID(0x02, fqid_namespace),
	FQID(0x4A, fqid_usage), );

const fqid_t FQUN_FwString3[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID_INDEX(0x10, fqid_collection, 2, 4),
	FQID(0x02, fqid_namespace),
	FQID(0x4A, fqid_usage), );

const fqid_t FQUN_FwString4[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x09, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID_INDEX(0x10, fqid_collection, 3, 4),
	FQID(0x02, fqid_namespace),
	FQID(0x4A, fqid_usage), );

/*
		Output related
*/

const fqid_t FQUN_OutputRealPowerPct[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x28, fqid_usage));

const fqid_t FQUN_OutputApparentPowerPct[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x49, fqid_usage));

const fqid_t FQUN_OutputNominalRealPower[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x2A, fqid_usage));

const fqid_t FQUN_OutputNominalApparentPower[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x2B, fqid_usage));

const fqid_t FQUN_OutputVoltageAC[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x25, fqid_usage));

const fqid_t FQUN_OutputCurrentAC[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x26, fqid_usage));

const fqid_t FQUN_OutputFrequency[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x27, fqid_usage));

const fqid_t FQUN_OutputVoltageSetting[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x2C, fqid_usage));

const fqid_t FQUN_OutputEnergy[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID(0x26, fqid_usage));

const fqid_t FQUN_SensitivitySetting[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID(0x25, fqid_usage));

const fqid_t FQUN_LowerAcceptableVoltage[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x0E, fqid_usage));

const fqid_t FQUN_UpperAcceptableVoltage[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x07, fqid_collection),
	FQID(0x0D, fqid_usage));

/*
		Unswitched/Master Outlet Group 0
*/

const fqid_t FQUN_MOG1_Name[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0x82, fqid_usage));

const fqid_t FQUN_MOG1_Status_BF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0xB6, fqid_usage));

const fqid_t FQUN_MOG1_StayOffCountdownConfig[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0xB7, fqid_usage));

const fqid_t FQUN_MOG1_TurnOffCountdownConfig[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0xB8, fqid_usage));

const fqid_t FQUN_MOG1_TurnOnCountdownConfig[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0xB9, fqid_usage));

const fqid_t FQUN_MOG1_StayOffCountdownValue[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0x2E, fqid_usage));

const fqid_t FQUN_MOG1_TurnOffCountdownValue[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0x2D, fqid_usage));

const fqid_t FQUN_MOG1_TurnOnCountdownValue[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x3E, fqid_collection),
	FQID(0xAD, fqid_usage));

/*
		Switched Outlet Group 1
*/

const fqid_t FQUN_SOG1_Name[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0x82, fqid_usage));

const fqid_t FQUN_SOG1_Status_BF[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0xB6, fqid_usage));

const fqid_t FQUN_SOG1_StayOffCountdownConfig[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0xB7, fqid_usage));

const fqid_t FQUN_SOG1_TurnOffCountdownConfig[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0xB8, fqid_usage));

const fqid_t FQUN_SOG1_TurnOnCountdownConfig[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0xB9, fqid_usage));

const fqid_t FQUN_SOG1_StayOffCountdownValue[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0x2E, fqid_usage));

const fqid_t FQUN_SOG1_TurnOffCountdownValue[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0x2D, fqid_usage));

const fqid_t FQUN_SOG1_TurnOnCountdownValue[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID_INDEX(0x3D, fqid_collection, 0, COLLECTION_IGNORE_LENGTH),
	FQID(0xAD, fqid_usage));

/*
		User Interface
*/
const fqid_t FQUN_UserInterfaceCommand[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x0B, fqid_collection),
	FQID(0x3B, fqid_usage));

/*
		Other main settings
*/
const fqid_t FQUN_PowerQualitySetting[MAX_FQIDS] = FQUN(
	FQID(0x02, fqid_namespace),
	FQID(0x04, fqid_collection),
	FQID(0x03, fqid_namespace),
	FQID(0x27, fqid_usage));


/*----------------------------------------------------------------
 |
 |            Mappings from usages to NUT directives
 |
 -----------------------------------------------------------------*/


/* Usage mappings */
mapped_usage_t M_BatteryTemp = {
	.fqun = FQUN_BatteryTemp,
	.info_directive = "ups.temperature",
	.type = ul_type_Int,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 1,
			.decimal_point = 7,
		}},
	.size = 2,
};

mapped_usage_t M_BatteryChargePct = {
	.fqun = FQUN_BatteryChargePct,
	.info_directive = "battery.charge",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 9,
		}},
};

mapped_usage_t M_BatteryVoltageDC = {
	.fqun = FQUN_BatteryVoltageDC,
	.info_directive = "battery.voltage",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 1,
			.decimal_point = 5,
		}},
};

mapped_usage_t M_BatteryInstallDate = {
	.fqun = FQUN_BatteryInstallDate,
	.info_directive = "battery.date",
	.type = ul_type_DateDays2000,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
};

mapped_usage_t M_LoadPct = {
	.fqun = FQUN_OutputRealPowerPct,
	.info_directive = "ups.load",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 8,
		}},
};

mapped_usage_t M_OutputApparentPowerPct = {
	.fqun = FQUN_OutputApparentPowerPct,
	.info_directive = "ups.power",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 8,
		}},
};

mapped_usage_t M_OutputRealPowerPct = {
	.fqun = FQUN_OutputRealPowerPct,
	.info_directive = "ups.realpower",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 8,
		}},
};

mapped_usage_t M_OutputNominalApparentPower = {
	.fqun = FQUN_OutputNominalApparentPower,
	.info_directive = "ups.power.nominal",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

mapped_usage_t M_OutputNominalRealPower = {
	.fqun = FQUN_OutputNominalRealPower,
	.info_directive = "ups.realpower.nominal",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

mapped_usage_t M_OutputVoltageAC = {
	.fqun = FQUN_OutputVoltageAC,
	.info_directive = "output.voltage",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 6,
		}},
};

mapped_usage_t M_OutputCurrentAC = {
	.fqun = FQUN_OutputCurrentAC,
	.info_directive = "output.current",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 5,
		}},
};

mapped_usage_t M_OutputFrequency = {
	.fqun = FQUN_OutputFrequency,
	.info_directive = "output.frequency",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 7,
		}},
};

mapped_usage_t M_OutputEnergy = {
	.fqun = FQUN_OutputEnergy,
	.info_directive = "experimental.output.energy",
	.type = ul_type_Int,
	.size = 4,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

mapped_usage_t M_InputFrequency = {
	.fqun = FQUN_InputFrequency,
	.info_directive = "input.frequency",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 7,
		}},
};

mapped_usage_t M_InputVoltageAC = {
	.fqun = FQUN_InputVoltageAC,
	.info_directive = "input.voltage",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 6,
		}},
};

mapped_usage_t M_InputEfficiency = {
	.fqun = FQUN_InputEfficiency,
	.info_directive = "ups.efficiency",
	.type = ul_type_Enum,
	.size = 2,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 7,
			.ranges = (enum_range_t[]){
				{.name = "unavailable", .show_value = 0, .start = -32768, .end = -1},
				{.name = "", .show_value = 1, .start = 0, .end = 12800},
			},
			.ranges_length = 2,
		}},
};

mapped_usage_t M_ModelName = {
	.fqun = FQUN_ModelName,
	.info_directive = "device.model",
	.type = ul_type_String,
	.size = 0,
	.flags = 0,
};

mapped_usage_t M_SerialNumber = {
	.fqun = FQUN_SerialNumber,
	.info_directive = "device.serial",
	.type = ul_type_String,
	.size = 0,
	.flags = 0,
};

mapped_usage_t M_NameString = {
	.fqun = FQUN_NameString,
	.info_directive = "device.id",
	.type = ul_type_String,
	.size = 0,
	.flags = 0 | APC_UL_FLAG_SETVAR,
};

mapped_usage_t M_BatteryTestResult = {
	.fqun = FQUN_ReplaceBatteryTestStatus,
	.info_directive = "ups.test.result",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = "Pending"},
				{.index = 1, .name = "InProgress"},
				{.index = 2, .name = "Passed"},
				{.index = 3, .name = "Failed"},
				{.index = 4, .name = "Refused"},
				{.index = 5, .name = "Aborted"},
				{.index = 6, .name = "Protocol"}, 
				{.index = 7, .name = "LocalUser"},
				{.index = 8, .name = "Internal"},
				{.index = 9, .name = "InvalidState"},
				{.index = 10, .name = "InternalFault"},
				{.index = 11, .name = "StateOfChargeNotAcceptable"},
				{.index = 12, .name = ""}, /* USB Port */
				{.index = 13, .name = ""}, /* RJ45 Port */
				{.index = 14, .name = ""}, /* SmartSlot */
				{.index = 15, .name = ""}, /* Network (smartconnect port) */
			},
			.values_length = 16,
		},
	},
};

mapped_usage_t M_StatusChangeReason = {
	.fqun = FQUN_UPSStatusChangeReason,
	.info_directive = "input.transfer.reason",
	.type = ul_type_Enum,
	.size = 2,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			.ranges = (enum_range_t[]){
				{.start = 0, .end = 0, .name = "SystemInitialization"},
				{.start = 1, .end = 1, .name = "HighInputVoltage"},
				{.start = 2, .end = 2, .name = "LowInputVoltage"},
				{.start = 3, .end = 3, .name = "DistortedInput"},
				{.start = 4, .end = 4, .name = "RapidChangeOfInputVoltage"},
				{.start = 5, .end = 5, .name = "HighInputFrequency"},
				{.start = 6, .end = 6, .name = "LowInputFrequency"},
				{.start = 7, .end = 7, .name = "FreqAndOrPhaseDifference"},
				{.start = 8, .end = 8, .name = "AcceptableInput"},
				{.start = 9, .end = 9, .name = "AutomaticTest"},
				{.start = 10, .end = 10, .name = "TestEnded"},
				{.start = 11, .end = 11, .name = "LocalUICommand"},
				{.start = 12, .end = 12, .name = "ProtocolCommand"},
				{.start = 13, .end = 13, .name = "LowBatteryVoltage"},
				{.start = 14, .end = 14, .name = "GeneralError"},
				{.start = 15, .end = 15, .name = "PowerSystemError"},
				{.start = 16, .end = 16, .name = "BatterySystemError"},
				{.start = 17, .end = 17, .name = "ErrorCleared"},
				{.start = 18, .end = 18, .name = "AutomaticRestart"},
				{.start = 19, .end = 19, .name = "DistortedInverterOutput"},
				{.start = 20, .end = 20, .name = "InverterOutputAcceptable"},
				{.start = 21, .end = 21, .name = "EPOInterface"},
				{.start = 22, .end = 22, .name = "InputPhaseDeltaOutOfRange"},
				{.start = 23, .end = 23, .name = "InputNeutralNotConnected"},
				{.start = 24, .end = 24, .name = "ATSTransfer"},
				{.start = 25, .end = 25, .name = "ConfigurationChange"},
				{.start = 26, .end = 26, .name = "AlertAsserted"},
				{.start = 27, .end = 27, .name = "AlertCleared"},
				{.start = 28, .end = 28, .name = "PlugRatingExceeded"},
				{.start = 29, .end = 29, .name = "OutletGroupStateChange"},
				{.start = 30, .end = 30, .name = "FailureBypassExpired"},
			},
			.ranges_length = 31,
		}},
};

mapped_usage_t M_ManufacturingDate = {
	.fqun = FQUN_ManufacturingDate,
	.info_directive = "device.mfr.date",
	.type = ul_type_DateDays2000,
	.size = 2,
	.flags = 0,
};

mapped_usage_t M_FwString1 = {
	.fqun = FQUN_FwString1,
	.info_directive = "ups.firmware",
	.type = ul_type_String,
	.size = 0,
	.flags = 0,
};

/* Bitmask */
mapped_usage_t M_OutputVoltageSetting = {
	.fqun = FQUN_OutputVoltageSetting,
	.info_directive = "output.voltage.nominal",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_BITMASK_EXCLUSIVE | APC_UL_FLAG_SETVAR,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = "VAC100"},
				{.index = 1, .name = "VAC120"},
				{.index = 2, .name = "VAC200"},
				{.index = 3, .name = "VAC208"},
				{.index = 4, .name = "VAC220"},
				{.index = 5, .name = "VAC230"},
				{.index = 6, .name = "VAC240"},
				/* Not in modbus specification, safer to leave out */
 			/*  {.index = 7, .name = ""},
				{.index = 8, .name = ""},
				{.index = 9, .name = ""},
				{.index = 10, .name = ""},
				{.index = 11, .name = ""},
				{.index = 12, .name = ""},
				{.index = 13, .name = ""},
				{.index = 14, .name = ""},
				{.index = 15, .name = ""},
				{.index = 16, .name = ""},
				{.index = 17, .name = ""},
				{.index = 18, .name = ""},
				{.index = 19, .name = ""}, */
			},
			.values_length = 20,
		}},
};

mapped_usage_t M_SensitivitySetting = {
	.fqun = FQUN_SensitivitySetting,
	.info_directive = "input.sensitivity",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_BITMASK_EXCLUSIVE | APC_UL_FLAG_SETVAR,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = "High"},
				{.index = 1, .name = "Reduced"},
				{.index = 2, .name = "Low"},
			},
			.values_length = 3,
		}},
};

mapped_usage_t M_PowerQualitySetting = {
	.fqun = FQUN_PowerQualitySetting,
	.info_directive = "input.quality",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_BITMASK_EXCLUSIVE | APC_UL_FLAG_SETVAR,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = "Custom"},
				{.index = 1, .name = "Good"},
				{.index = 2, .name = "Fair"},
				{.index = 3, .name = "Poor"},
			},
			.values_length = 4,
		}},
};

mapped_usage_t M_BatteryTestInterval = {
	.fqun = FQUN_BatteryTestInterval,
	.info_directive = "ups.test.interval",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_BITMASK_EXCLUSIVE | APC_UL_FLAG_SETVAR,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = "None"},
				{.index = 1, .name = "Startup"},
				{.index = 2, .name = "Startup_Every_7"},
				{.index = 3, .name = "Startup_Every_14"},
				{.index = 4, .name = "Startup_7_After_last"},
				{.index = 5, .name = "Startup_14_After_last"},
			},
			.values_length = 6,
		}},
};

mapped_usage_t M_LowerAcceptableVoltage = {
	.fqun = FQUN_LowerAcceptableVoltage,
	.info_directive = "input.transfer.low",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

mapped_usage_t M_UpperAcceptableVoltage = {
	.fqun = FQUN_UpperAcceptableVoltage,
	.info_directive = "input.transfer.high",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

mapped_usage_t M_BatteryReplaceDate = {
	.fqun = FQUN_BatteryReplaceDate,
	.info_directive = "battery.date.maintenance",
	.type = ul_type_DateDays2000,
	.size = 2,
	.flags = 0,
};

/* Note: Max 18 hours for 2 byte value */
mapped_usage_t M_BatteryRuntimeRemaining = {
	.fqun = FQUN_BatteryRuntimeRemaining,
	.info_directive = "battery.runtime",
	.type = ul_type_Int,
	.size = 2,
	.flags = 0,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

/* Note: This doesn't exist on all models, please confirm */
mapped_usage_t M_BatteryRuntimeRemaining4B = {
	.fqun = FQUN_BatteryRuntimeRemaining4B,
	.info_directive = "battery.runtime",
	.type = ul_type_Int,
	.size = 4,
	.flags = 0 | APC_UL_FLAG_IGNORE_MISSING,
	.type_info = {
		.int_info_t = {
			._signed = 0,
			.decimal_point = 0,
		}},
};

/* AKA RebootCountdown */
mapped_usage_t M_DelayReboot = {
	.fqun = FQUN_MOG1_StayOffCountdownConfig,
	.info_directive = "ups.delay.reboot",
	.type = ul_type_Int,
	.size = 4,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.int_info_t = {
			.decimal_point = 0,
			._signed = 1,
		},
	},
};

mapped_usage_t M_DelayShutdown = {
	.fqun = FQUN_MOG1_TurnOffCountdownConfig,
	.info_directive = "ups.delay.shutdown",
	.type = ul_type_Enum,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "No_shutdown", .show_value = 0, .start = -1, .end = -1},
				{.name = "", .show_value = 1, .start = 0, .end = 32767},
			},
			.ranges_length = 2,
		},
	},
};

mapped_usage_t M_DelayStart = {
	.fqun = FQUN_MOG1_TurnOnCountdownConfig,
	.info_directive = "ups.delay.start",
	.type = ul_type_Enum,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "No_start", .show_value = 0, .start = -1, .end = -1},
				{.name = "", .show_value = 1, .start = 0, .end = 32767},
			},
			.ranges_length = 2,
		},
	},
};

mapped_usage_t M_TimerReboot = {
	.fqun = FQUN_MOG1_StayOffCountdownValue,
	.info_directive = "ups.timer.reboot",
	.type = ul_type_Enum,
	.size = 4,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "Inactive", .show_value = 0, .start = -1, .end = -1},
				{.name = "Ended", .show_value = 0, .start = 0, .end = 0},
				{.name = "", .show_value = 1, .start = 1, .end = 2147483647},
			},
			.ranges_length = 3,
		},
	},
};

mapped_usage_t M_TimerShutdown = {
	.fqun = FQUN_MOG1_TurnOffCountdownValue,
	.info_directive = "ups.timer.shutdown",
	.type = ul_type_Enum,
	.size = 4,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "Inactive", .show_value = 0, .start = -1, .end = -1},
				{.name = "Ended", .show_value = 0, .start = 0, .end = 0},
				{.name = "", .show_value = 1, .start = 1, .end = 2147483647},
			},
			.ranges_length = 3,
		},
	},
};

mapped_usage_t M_TimerStart = {
	.fqun = FQUN_MOG1_TurnOnCountdownValue,
	.info_directive = "ups.timer.start",
	.type = ul_type_Enum,
	.size = 4,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "Inactive", .show_value = 0, .start = -1, .end = -1},
				{.name = "Ended", .show_value = 0, .start = 0, .end = 0},
				{.name = "", .show_value = 1, .start = 1, .end = 2147483647},
			},
			.ranges_length = 3,
		},
	},
};

mapped_usage_t M_MOG1_Status = {
	.fqun = FQUN_MOG1_Status_BF,
	.info_directive = "outlet.group.0.status",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = "StateOn"},
				{.index = 1, .name = "StateOff"},
				{.index = 2, .name = "ProcessReboot"},
				{.index = 3, .name = "ProcessShutdown"},
				{.index = 4, .name = "ProcessSleep"},
				{.index = 7, .name = "PendingLoadShedding"},
				{.index = 8, .name = "PendingOnDelay"}, 
				{.index = 9, .name = "PendingOffDelay"},
				{.index = 10, .name = "PendingOnACPresence"},
				{.index = 11, .name = "PendingOnMinRuntime"},
				{.index = 12, .name = "MemberGroupProcess1"},
				{.index = 13, .name = "MemberGroupProcess2"},
				{.index = 14, .name = "LowRuntime"},
			},
			.values_length = 13,
		},
	},
};

mapped_usage_t M_MOG1_NameSTR = {
	.fqun = FQUN_MOG1_Name,
	.info_directive = "outlet.group.0.name",
	.type = ul_type_String,
	.size = 0,
	.flags = 0 | APC_UL_FLAG_SETVAR,
};

/* AKA RebootCountdown */
mapped_usage_t M_MOG1_StayOffCountdownSetting = {
	.fqun = FQUN_MOG1_StayOffCountdownConfig,
	.info_directive = "outlet.group.0.delay.reboot",
	.type = ul_type_Int,
	.size = 4,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.int_info_t = {
			.decimal_point = 0,
			._signed = 1,
		},
	},
};

mapped_usage_t M_MOG1_TurnOffCountdownSetting = {
	.fqun = FQUN_MOG1_TurnOffCountdownConfig,
	.info_directive = "outlet.group.0.delay.shutdown",
	.type = ul_type_Enum,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "No_shutdown", .show_value = 0, .start = -1, .end = -1},
				{.name = "", .show_value = 1, .start = 0, .end = 32767},
			},
			.ranges_length = 2,
		},
	},
};

mapped_usage_t M_MOG1_TurnOnCountdownSetting = {
	.fqun = FQUN_MOG1_TurnOnCountdownConfig,
	.info_directive = "outlet.group.0.delay.start",
	.type = ul_type_Enum,
	.size = 2,
	.flags = 0 | APC_UL_FLAG_SETVAR,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "No_start", .show_value = 0, .start = -1, .end = -1},
				{.name = "", .show_value = 1, .start = 0, .end = 32767},
			},
			.ranges_length = 2,
		},
	},
};

mapped_usage_t M_MOG1_StayOffCountdown_EN = {
	.fqun = FQUN_MOG1_StayOffCountdownValue,
	.info_directive = "outlet.group.0.timer.reboot",
	.type = ul_type_Enum,
	.size = 4,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "Inactive", .show_value = 0, .start = -1, .end = -1},
				{.name = "Ended", .show_value = 0, .start = 0, .end = 0},
				{.name = "", .show_value = 1, .start = 1, .end = 2147483647},
			},
			.ranges_length = 3,
		},
	},
};

mapped_usage_t M_MOG1_TurnOffCountdown_EN = {
	.fqun = FQUN_MOG1_TurnOffCountdownValue,
	.info_directive = "outlet.group.0.timer.shutdown",
	.type = ul_type_Enum,
	.size = 4,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "Inactive", .show_value = 0, .start = -1, .end = -1},
				{.name = "Ended", .show_value = 0, .start = 0, .end = 0},
				{.name = "", .show_value = 1, .start = 1, .end = 2147483647},
			},
			.ranges_length = 3,
		},
	},
};

mapped_usage_t M_MOG1_TurnOnCountdown_EN = {
	.fqun = FQUN_MOG1_TurnOnCountdownValue,
	.info_directive = "outlet.group.0.timer.start",
	.type = ul_type_Enum,
	.size = 4,
	.flags = 0,
	.type_info = {
		.enum_info_t = {
			._signed = 1,
			.decimal_point = 0,
			.ranges = (enum_range_t[]){
				{.name = "Inactive", .show_value = 0, .start = -1, .end = -1},
				{.name = "Ended", .show_value = 0, .start = 0, .end = 0},
				{.name = "", .show_value = 1, .start = 1, .end = 2147483647},
			},
			.ranges_length = 3,
		},
	},
};

/* Debug usages */
mapped_usage_t M_Debug_UPSStatus = {
	.fqun = FQUN_UPSStatus_BF,
	.info_directive = "debug.ups.status",
	.type = ul_type_Bitfield,
	.size = 2,
	.flags = 0,
	.type_info = {
		.bitfield_info_t = {
			.values = (bitfield_value_t[]){
				{.index = 0, .name = ""}, /* Not used change */
				{.index = 1, .name = "StateOnline"},
				{.index = 2, .name = "StateOnBattery"},
				{.index = 3, .name = "StateOnBypass"},
				{.index = 4, .name = "StateOutputOff"},
				{.index = 5, .name = "Fault"},
				{.index = 6, .name = "InputBad"},
				{.index = 7, .name = "Test"},
				{.index = 8, .name = "PendingOutputOn"},
				{.index = 9, .name = "PendingOutputOff"},
				{.index = 10, .name = ""},
				{.index = 11, .name = ""},
				{.index = 12, .name = ""},
				{.index = 13, .name = "HighEfficiency"},
				{.index = 14, .name = "InformationalAlert"},
				{.index = 15, .name = ""},
				/* Not in modbus spec, leaving out in case of undefined behaviour */
/* 				{.index = 16, .name = ""},
				{.index = 17, .name = ""},
				{.index = 18, .name = ""},
				{.index = 19, .name = ""},
				{.index = 20, .name = ""},
				{.index = 21, .name = ""},
				{.index = 22, .name = ""},
				{.index = 23, .name = ""},
				{.index = 24, .name = ""}, */
			},
			.values_length = 25,
		},
	},
};

mapped_usage_t* updated_usages[] = {
	&M_BatteryTemp,
	&M_BatteryChargePct,
	&M_BatteryVoltageDC,
	&M_BatteryInstallDate,
	&M_LoadPct,
	&M_OutputApparentPowerPct,
	&M_OutputRealPowerPct,
	&M_OutputVoltageAC,
	&M_OutputCurrentAC,
	&M_OutputFrequency,
	&M_InputFrequency,
	&M_InputVoltageAC,
	&M_InputEfficiency,
	&M_ModelName,
	&M_SerialNumber,
	&M_NameString,
	&M_ManufacturingDate,
	&M_FwString1,
	&M_OutputVoltageSetting,
	&M_SensitivitySetting,
	&M_PowerQualitySetting,
	&M_LowerAcceptableVoltage,
	&M_UpperAcceptableVoltage,
	&M_BatteryTestInterval,
	&M_BatteryReplaceDate,
	&M_BatteryRuntimeRemaining,
	&M_BatteryRuntimeRemaining4B,  /* Must be after the last one, so it overwrites it if the usage_t is present */
	&M_OutputNominalApparentPower,
	&M_OutputNominalRealPower,
	&M_OutputEnergy,
	&M_StatusChangeReason,
	&M_BatteryTestResult,
	/* Timers for master (unswitched) outlet group */
	&M_TimerReboot,
	&M_TimerShutdown,
	&M_TimerStart,
	&M_DelayReboot,
	&M_DelayShutdown,
	&M_DelayStart,
	/* Debug usages */
	/* &M_Debug_UPSStatus, */
};

int updated_usages_length = sizeof(updated_usages) / sizeof(mapped_usage_t*);

/* Shutdown then return after AC back on. Unswitched (main) outlet group
Note: on devices with 3 output groups and no MOG(unswitched/master), these may 
not work (APCs official driver doesnt work either, it sends a blank command to device.
NMC does work, but commands all outlets simultaneously, probably most practical, 
alternatively you could use the simple signalling command which works on everything) */
mapped_command_t C_shutdownReturn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "shutdown.return",

	.command = (uint8_t[]){0x41, 0x08},
	.command_length = 2,
};

/* Shutdown then stay off. Unswitched (main) outlet group */
mapped_command_t C_shutdownStayoff = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "shutdown.stayoff",

	.command = (uint8_t[]){0x41, 0x04},
	.command_length = 2,
};

mapped_command_t C_loadOff = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "load.off",

	.command = (uint8_t[]){0x41, 0x04},
	.command_length = 2,
};

mapped_command_t C_loadOffDelay = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "load.off.delay",

	.command = (uint8_t[]){0x41, 0x84},
	.command_length = 2,
};

mapped_command_t C_loadOn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "load.on",

	.command = (uint8_t[]){0x41, 0x02},
	.command_length = 2,
};

mapped_command_t C_loadOnDelay = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "load.on.delay",

	.command = (uint8_t[]){0x41, 0x42},
	.command_length = 2,
};

mapped_command_t C_shutdownReboot = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "shutdown.reboot",

	.command = (uint8_t[]){0x41, 0x10},
	.command_length = 2,
};

/* Same as above except it waits before turning off (time specified by ups.delay.shutdown) */
mapped_command_t C_shutdownRebootGraceful = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "shutdown.reboot.graceful",

	.command = (uint8_t[]){0x41, 0x90},
	.command_length = 2,
};

mapped_command_t C_startCalibrate = {
	.fqun = FQUN_RuntimeCalibrationCommand,
	.command_directive = "calibrate.start",

	.command = (uint8_t[]){0x00, 0x01},
	.command_length = 2,
};

mapped_command_t C_stopCalibrate = {
	.fqun = FQUN_RuntimeCalibrationCommand,
	.command_directive = "calibrate.stop",

	.command = (uint8_t[]){0x00, 0x02},
	.command_length = 2,
};

mapped_command_t C_testPanel = {
	.fqun = FQUN_UserInterfaceCommand,
	.command_directive = "test.panel.start",

	.command = (uint8_t[]){0x00, 0x01},
	.command_length = 2,
};

mapped_command_t C_testBatteryStart = {
	.fqun = FQUN_ReplaceBatteryTestCommand,
	.command_directive = "test.battery.start",

	.command = (uint8_t[]){0x00, 0x01},
	.command_length = 2,
};

mapped_command_t C_beeperMute = {
	.fqun = FQUN_UserInterfaceCommand,
	.command_directive = "beeper.mute",

	.command = (uint8_t[]){0x00, 0x04},
	.command_length = 2,
};

mapped_command_t C_bypassStart = {
	.fqun = FQUN_UPSCommandBF,
	.command_directive = "bypass.start",

	.command = (uint8_t[]){0x00, 0x10},
	.command_length = 2,
};

mapped_command_t C_bypassStop = {
	.fqun = FQUN_UPSCommandBF,
	.command_directive = "bypass.stop",

	.command = (uint8_t[]){0x00, 0x20},
	.command_length = 2,
};

/* SOG */
mapped_command_t C_UOG_shutdownReturn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.0.shutdown.return",

	.command = (uint8_t[]){0x41, 0x08},
	.command_length = 2,
};

mapped_command_t C_UOG_loadOff = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.0.load.off",

	.command = (uint8_t[]){0x41, 0x04},
	.command_length = 2,
};

mapped_command_t C_UOG_loadOn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.0.load.on",

	.command = (uint8_t[]){0x41, 0x02},
	.command_length = 2,
};

mapped_command_t C_UOG_shutdownReboot = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.0.load.cycle",

	.command = (uint8_t[]){0x41, 0x10},
	.command_length = 2,
};

/* SOG1 */
mapped_command_t C_SOG1_shutdownReturn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.1.shutdown.return",

	.command = (uint8_t[]){0x42, 0x08},
	.command_length = 2,
};

mapped_command_t C_SOG1_loadOff = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.1.load.off",

	.command = (uint8_t[]){0x42, 0x04},
	.command_length = 2,
};

mapped_command_t C_SOG1_loadOn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.1.load.on",

	.command = (uint8_t[]){0x42, 0x02},
	.command_length = 2,
};

mapped_command_t C_SOG1_shutdownReboot = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.1.load.cycle",

	.command = (uint8_t[]){0x42, 0x10},
	.command_length = 2,
};

/* SOG2 */
mapped_command_t C_SOG2_shutdownReturn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.2.shutdown.return",

	.command = (uint8_t[]){0x44, 0x08},
	.command_length = 2,
};

mapped_command_t C_SOG2_loadOff = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.2.load.off",

	.command = (uint8_t[]){0x44, 0x04},
	.command_length = 2,
};

mapped_command_t C_SOG2_loadOn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.2.load.on",

	.command = (uint8_t[]){0x44, 0x02},
	.command_length = 2,
};

mapped_command_t C_SOG2_shutdownReboot = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.2.load.cycle",

	.command = (uint8_t[]){0x44, 0x10},
	.command_length = 2,
};

/* SOG3 */
mapped_command_t C_SOG3_shutdownReturn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.3.shutdown.return",

	.command = (uint8_t[]){0x48, 0x08},
	.command_length = 2,
};

mapped_command_t C_SOG3_loadOff = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.3.load.off",

	.command = (uint8_t[]){0x48, 0x04},
	.command_length = 2,
};

mapped_command_t C_SOG3_loadOn = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.3.load.on",

	.command = (uint8_t[]){0x48, 0x02},
	.command_length = 2,
};

mapped_command_t C_SOG3_shutdownReboot = {
	.fqun = FQUN_OutletCommand,
	.command_directive = "outlet.3.load.cycle",

	.command = (uint8_t[]){0x48, 0x10},
	.command_length = 2,
};

mapped_command_t* instcmds[] = {
	&C_shutdownReturn,
	&C_shutdownStayoff,
	&C_loadOff,
	&C_loadOffDelay,
	&C_loadOn,
	&C_loadOnDelay,
	&C_shutdownReboot,
	&C_shutdownRebootGraceful,
	&C_startCalibrate,
	&C_stopCalibrate,
	&C_testPanel,
	&C_testBatteryStart,
	&C_beeperMute,
	&C_bypassStart,
	&C_bypassStop,

	/* Outlet groups */
	&C_UOG_shutdownReturn,
	&C_UOG_loadOff,
	&C_UOG_loadOn,
	&C_UOG_shutdownReboot,
	&C_SOG1_shutdownReturn,
	&C_SOG1_loadOff,
	&C_SOG1_loadOn,
	&C_SOG1_shutdownReboot,
	&C_SOG2_shutdownReturn,
	&C_SOG2_loadOff,
	&C_SOG2_loadOn,
	&C_SOG2_shutdownReboot,
	&C_SOG3_shutdownReturn,
	&C_SOG3_loadOff,
	&C_SOG3_loadOn,
	&C_SOG3_shutdownReboot,
};

int instcmds_length = sizeof(instcmds) / sizeof(mapped_command_t*);

mapped_usage_t* switched_outlet_group_usages[] = {
	&M_MOG1_NameSTR,
	&M_MOG1_Status,
	&M_MOG1_TurnOffCountdownSetting,
	&M_MOG1_TurnOnCountdownSetting,
	&M_MOG1_StayOffCountdownSetting,
	&M_MOG1_TurnOffCountdown_EN,
	&M_MOG1_TurnOnCountdown_EN,
	&M_MOG1_StayOffCountdown_EN,
};

int switched_outlet_group_usages_length = sizeof(switched_outlet_group_usages) / sizeof(mapped_usage_t*);

