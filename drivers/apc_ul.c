/*
 * apc_ul.c - APC Microlink driver compatible with APC Microlink units
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
 *
 */

/*  Trademark Notice:
 *
 *  "APC" is a registered trademark of Schneider Electric IT Corporation.
 *  This program (microlink UPS driver) is not affiliated with or endorsed
 *  by American Power Conversion or Schneider Electric IT Corporation.
 */

#include "main.h"
/* #include "attribute.h" */
/* #include "config.h" */
#include "apc_ul.h"
/* #include "apc_ul_debugdatastore.h" */
#include "apc_ul_util.h"
#include "apc_ul_usagemap.h"
#include "serial.h"

/* #define ENDCHAR	'\r'	*/
/* #define IGNCHARS	""	*/

#define DRIVER_NAME "APC Microlink UPS driver"
#define DRIVER_VERSION "0.01"

uint8_t ul_row_size = 0;
uint8_t number_of_rows = 0;

uint8_t *data_block = NULL;

/* At most 512 usages for now */
usage_t usage_array[MAX_USAGES];
int usage_count = 0;

/* Store the tx'd slave password */
uint8_t slave_password[4];

enum ul_state state = ul_state_disconnected;

/* driver description structure */
upsdrv_info_t upsdrv_info = {DRIVER_NAME,
							 DRIVER_VERSION,
							 "Thomas Eldon <tom@tomeldon.com>",
							 DRV_EXPERIMENTAL,
							 {NULL}};

microlink_row_t read_ul_row(uint8_t *ret_array, int length);
static int instcmd(const char *cmdname, const char *extra);

void store_row(microlink_row_t row)
{
	if (data_block == NULL || row.row_num >= number_of_rows)
		return;

	for (uint8_t i = 0; i < ul_row_size; i++)
	{
		data_block[(row.row_num * ul_row_size) + i] = row.array[i + 1];
	}
}

/* Get a usage_t from the zero-escaped fqun array */
usage_t *fetchUsage(const fqid_t *fqun)
{
	size_t fqun_length = 0;
	for (size_t i = 0; i < MAX_FQIDS && fqun[i].id != 0; i++)
	{
		fqun_length = i + 1;
	}

	for (int i = 0; i < usage_count; i++)
	{
		usage_t current = usage_array[i];

		if (fqun_length == current.fqun_stack_size)
		{
			if (compareFQUN(fqun, current.fqun, fqun_length - 1) == 0)
			{
				return usage_array + i;
			}
		}
	}
	return NULL;
}

/* Returns 0 on successful creation */
int create_slave_password_wrapper(void)
{
	usage_t *master_pass_usage = fetchUsage(FQUN_MasterPassword);
	usage_t *serial_number_usage = fetchUsage(FQUN_SerialNumber);

	if (master_pass_usage == NULL || serial_number_usage == NULL)
	{
		return 1;
	}

	create_slave_password(data_block + (*master_pass_usage).data_location,
						  2, data_block, 8,
						  data_block + (*serial_number_usage).data_location,
						  (*serial_number_usage).data_size, slave_password);

	return 0;
}

/* Returns 0 on success */
int create_master_password_wrapper(uint8_t *master_password_verify)
{
	usage_t *serial_number_usage = fetchUsage(FQUN_SerialNumber);

	if (serial_number_usage == NULL)
	{
		return 1;
	}

	create_master_password_verify(
		slave_password, 2,
		data_block, 8,
		data_block + (*serial_number_usage).data_location, (*serial_number_usage).data_size,
		master_password_verify);

	return 0;
}

/* Returns 0 on success */
int verify_master_password(uint8_t *master_password_verify)
{
	usage_t *master_password_usage = fetchUsage(FQUN_MasterPassword);

	if (master_password_usage == NULL)
	{
		return 1;
	}

	/* Compare last 2 bytes of the master password usage_t with the calculated one */
	if (memcmp(data_block + (*master_password_usage).data_location + 2, master_password_verify, 2) == 0)
	{
		return 0;
	}

	return 1;
}

/* Write complete row (without checksum) to microlink master, waiting for valid response
Returns 0 on success */
int write_ul_row(uint8_t *row, int row_length, int validate_response)
{
	/* Sanity check. Largest USER WRITABLE usage_t (size of data to write, or row[2])
	observed is 16 (aka the commonly used "ul_row_size") */
	if ((row[1] + row[2]) > ul_row_size)
	{
		upslogx(LOG_ERR, "Cannot write row as data crosses boundary (Validation not implemented or shouldn't exist)");
		return 1;
	}

	uint8_t row_with_cs[row_length + 2];

	memcpy(row_with_cs, row, row_length);

	if (addChecksumToMessage(row_with_cs, row_length, row_length + 2) != 0)
	{
		return 1;
	}

	if (ser_send_buf(upsfd, row_with_cs, row_length + 2) != (row_length + 2))
	{
		return 1;
	}

	if (validate_response == 0)
	{
		return 0;
	}

	/* Validate the response */
	uint8_t row_data[32];
	microlink_row_t ul_row = read_ul_row(row_data, 32);

	if (validate_row(ul_row) != 0)
	{
		upslogx(LOG_ERR, "UPS ignored write / sent invalid row after write");
		return 1;
	}

	/* row[0] is the row number, row[1] is the offset into the row, row[2] is the length */
	if (ul_row.row_num == row[0] && memcmp(row + 3, row_data + 1 + row[1], row[2]) == 0)
	{
		return 0;
	}
	else
	{
		upslogx(LOG_ERR, "Received row's data doesn't match after write");
		/* print_hex(row, row_length); */
		/* print_hex(ul_row.array, ul_row.length); */
		return 1;
	}
}

/* Pass in data of length "usage_t.data_size" and write it to master
Returns 0 if successful. Validate_response should be set */
int write_usage(usage_t *usage_t, uint8_t *data, int validate_response)
{
#ifndef DEBUG_DATASTORE
	uint8_t to_write[3 + usage_t->data_size];
	to_write[0] = usage_t->data_location / ul_row_size;
	to_write[1] = usage_t->data_location % ul_row_size;
	to_write[2] = usage_t->data_size;

	memcpy(to_write + 3, data, usage_t->data_size);

	return write_ul_row(to_write, 3 + usage_t->data_size, validate_response);
#else
	memcpy(data_block + usage_t->data_location, data, usage_t->data_size);
	return 0;
#endif
}

/* Returns 0 on success */
int write_password(uint8_t *data)
{
	usage_t *master_pass_usage = fetchUsage(FQUN_SlavePassword);

	if (master_pass_usage == NULL)
		return 1;

	return write_usage(master_pass_usage, data, 0);
}

/* Pass in integer and usage_t and write it to master
Returns 0 on success */
int write_usage_integer(usage_t *usage_t, int64_t data, int is_signed)
{
	if (usage_t->data_size > 4)
	{
		upslogx(LOG_ERR, "Usage exposed with data size greater than 4 bytes, not supported!");
		return 1;
	}

	/* Overflow check */
	if (is_signed)
	{
		if (data < -(1LL << ((usage_t->data_size * 8) - 1)) || data >= (1LL << ((usage_t->data_size * 8) - 1)))
		{
			upslogx(LOG_ERR, "Integer too large to be written to device!");
			return 1;
		}
	}
	else
	{
		if (data >= (1LL << (usage_t->data_size * 8)))
		{
			upslogx(LOG_ERR, "Unsigned integer too large to be written to device!");
			return 1;
		}
	}

	uint8_t output[usage_t->data_size];

	/* Convert to big endian ordering */
	for (int oi = usage_t->data_size - 1; oi >= 0; oi--)
	{
		output[oi] = (uint8_t)(data & 0xFF);
		data >>= 8;
	}

	return write_usage(usage_t, output, 1);
}

/* Returns 0 on successful detection */
int detect_ul_device(void)
{
	if (state != ul_state_disconnected)
		return 1;

	/* Flush in and out queues */
	ser_flush_io(upsfd);

	/* Send NAK. Sends row 0 again (or repeats last row if monitoring). */
	/* Note: If sent 3 times WHILE MONITORING, it resets the protocol and sends row 0 again on the 4th NAK */
	ssize_t writeResult = ser_send_char(upsfd, 0xFD);
	if (writeResult != 1)
	{
		upslogx(LOG_ERR, "Cannot write to serial port. Not open?");
		/* Attempt to reopen? Quits immediately anyway so dont bother */
		return 1;
	}

	upsdebugx(1, "Polling Microlink UPS device");

	uint8_t row_data[32];
	microlink_row_t row = read_ul_row(row_data, 32);

	if (validate_row(row) == 0 && row.row_num == 0)
	{
		number_of_rows = row.array[3];
		upslogx(LOG_INFO, "Device found! Number of rows: %i", number_of_rows);

		/* Reset existing parameters */
		usage_count = 0;
		if (data_block != NULL)
		{
			upsdebugx(1, "Unloading old datastore");
			free(data_block);
			data_block = NULL;
		}

		/* data_block = malloc(number_of_rows * ul_row_size); */

		data_block = xmalloc(number_of_rows * ul_row_size);

		store_row(row);
		state = ul_state_connected;
		return 0;
	}
	else
	{
		upsdebugx(1, "Polling failed, invalid row or timeout");
	}
	return 1;
}

/* Send password to device and verify response
Returns 0 on success */
int auth_ul_device(void)
{
	if (create_slave_password_wrapper() != 0)
	{
		upslogx(LOG_ERR, "[Auth] Failed to calculate slave password");
		return 1;
	}

	/* Flush in queue */
	tcflush(upsfd, TCIFLUSH);
	if (write_password(slave_password) != 0)
	{
		upslogx(LOG_ERR, "[Auth] Failed to send slave password");
		return 1;
	}

	uint8_t row_data[32];

	/* After sending password, read 1 row, should be master password row (usually second to last) */
	microlink_row_t row = read_ul_row(row_data, 32);
	if (validate_row(row) != 0)
	{
		upslogx(LOG_ERR, "[Auth] Invalid row RX'd after sending password, auth failed");
		return 1;
	}

	store_row(row);

	usage_t *master_usage = fetchUsage(FQUN_MasterPassword);
	if (master_usage == NULL)
	{
		upslogx(LOG_ERR, "[Auth] Cannot find master usage_t for auth");
		return 1;
	}
	int target_row = master_usage->data_location / ul_row_size;

	if ((int)row.row_num != target_row)
	{
		upslogx(LOG_ERR, "[Auth] Wrong row number RX'd after sending password (%i), auth failed", row.row_num);
		return 1;
	}

	uint8_t masterverify[2];

	if (create_master_password_wrapper(masterverify) != 0)
	{
		upslogx(LOG_ERR, "[Auth] Failed to create master password for verification");
		return 1;
	}

	if (verify_master_password(masterverify) == 0)
	{
		return 0;
	}
	else
	{
		upslogx(LOG_ERR, "[Auth] Failed to auth, cannot verify master password");
		return 1;
	}
}

/* Returns 0 on successful parse */
int setup_ul_device(void)
{
	if (state != ul_state_connected)
		return 1;

	uint8_t row_data[32];
	microlink_row_t row;

	for (int i = 0; i < 1000; i++)
	{
		/* Flush in queue */
		tcflush(upsfd, TCIFLUSH);

		if (i == 0)
		{
			/* Send NAK to start */
			ser_send_char(upsfd, 0xFD);
		}
		else
		{
			/* Send ACK */
			ser_send_char(upsfd, 0xFE);
		}

		row = read_ul_row(row_data, 32);

		if (validate_row(row) != 0)
		{
			return 1;
		}

		store_row(row);

		upsdebugx(1, "[Setup] RX: %i", row.row_num);

		if (row.row_num == number_of_rows - 1)
		{
			/* Now parse the descriptor */
			usage_count = parseDatastore(data_block, number_of_rows,
										 ul_row_size, usage_array);

			if (usage_count < 0)
			{
				upslogx(LOG_ERR, "Error while loading descriptor!");
				return 1;
			}

			upslogx(LOG_INFO, "Loaded descriptor with %i usages", usage_count);

			if (auth_ul_device() == 0)
			{
				state = ul_state_authenticated;
				return 0;
			}
			else
			{
				state = ul_state_disconnected;
				return 1;
			}
		}
	}
	return 1;
}

/* Destination will have data copied into */
void datastoreReadInto(size_t data_location, size_t data_size, uint8_t *destination)
{
	if (data_location >= (number_of_rows * ul_row_size))
	{
		return;
	}
	memcpy(destination, data_block + data_location, data_size);
}

/* Destination will have a NUL appended to so must be usage_t->data_size + 1 */
void readStringUsageInto(usage_t *usage_t, char *destination)
{
	datastoreReadInto(usage_t->data_location, usage_t->data_size, (uint8_t *)destination);
	destination[usage_t->data_size] = '\0';
}

/* Destination will have data copied into */
void readUsageInto(usage_t *usage_t, uint8_t *destination)
{
	datastoreReadInto(usage_t->data_location, usage_t->data_size, destination);
}

/* Returns integer (big endian) from datastore (4 byte max) */
int64_t datastoreReadInteger(size_t data_location, size_t data_size, int is_signed)
{
	if (data_size > 4)
	{
		upslogx(LOG_ERR, "Integer too large to be read from datastore!");
		return 0;
	}

	int64_t result = 0;

	uint8_t string[data_size];
	datastoreReadInto(data_location, data_size, string);

	for (size_t i = 0; i < data_size; i++)
	{
		result |= string[i] << ((data_size - 1 - i) * 8);
	}

	/* Sign extend if negative */
	if (is_signed && (string[0] & 0x80))
	{
		result |= (int64_t)(0xFFFFFFFF << (data_size * 8));
	}

	return result;
}

int64_t readUsageValueInteger(usage_t *usage_t, int is_signed)
{
	return datastoreReadInteger(usage_t->data_location, usage_t->data_size, is_signed);
}

/* Destination array will have data copied into */
int64_t readUsageMinInteger(usage_t *usage_t, int is_signed)
{
	if (usage_t->min_location == 0)
	{
		upslogx(LOG_ERR, "Usage has no min_location!");
		return 0;
	}
	return datastoreReadInteger(usage_t->min_location, usage_t->data_size, is_signed);
}

/* Destination array will have data copied into */
int64_t readUsageMaxInteger(usage_t *usage_t, int is_signed)
{
	if (usage_t->max_location == 0)
	{
		upslogx(LOG_ERR, "Usage has no max_location!");
		return 0;
	}
	return datastoreReadInteger(usage_t->max_location, usage_t->data_size, is_signed);
}

/* Destination array will have data copied into */
void readUsageBitmaskInto(usage_t *usage_t, uint8_t *destination)
{
	if (usage_t->bitmask_location == 0)
	{
		upslogx(LOG_ERR, "Usage has no bitmask!");
		return;
	}
	datastoreReadInto(usage_t->bitmask_location, usage_t->data_size, destination);
}

void process_mapped_usage(mapped_usage_t *current)
{
	usage_t *usage_t = fetchUsage(current->fqun);

	if (usage_t == NULL)
	{
		if (!(current->flags & APC_UL_FLAG_IGNORE_MISSING))
		{
			char fqunString[FQUN_AS_STRING_MAX_LENGTH] = {0};
			fqun_to_string(current->fqun, fqunString);
			upslogx(LOG_WARNING, "Usage not resolved for FQUN (%s) (Device does not expose usage_t)", fqunString);
		}
		return;
	}

	switch (current->type)
	{
	case ul_type_String:
	{
		char string[usage_t->data_size + 1];
		readStringUsageInto(usage_t, string);

		dstate_setinfo(current->info_directive, "%s", string);
		break;
	}
	case ul_type_Int:
	{
		if (usage_t->data_size > 4)
		{
			upslogx(LOG_ERR, "Usage too large for integer reading (%s)", current->info_directive);
			return;
		}

		int_info_t info = current->type_info.int_info_t;

		int64_t value = readUsageValueInteger(usage_t, info._signed);

		float val = ((float)value) / (1 << info.decimal_point);

		dstate_setinfo(current->info_directive, "%f", val);

		break;
	}
	case ul_type_Enum:
	{
		if (usage_t->data_size > 4)
		{
			upslogx(LOG_ERR, "Usage too large for enum integer reading (%s)", current->info_directive);
			return;
		}

		enum_info_t info = current->type_info.enum_info_t;

		int64_t value = readUsageValueInteger(usage_t, info._signed);

		dstate_setinfo(current->info_directive, "No enum range! (%li)", value);

		/* Match value against a range */
		for (int i = 0; i < info.ranges_length; i++)
		{
			enum_range_t range = info.ranges[i];
			if ((int)value >= range.start && (int)value <= range.end)
			{
				if (range.show_value)
				{
					float val = ((float)value) / (1 << info.decimal_point);

					dstate_setinfo(current->info_directive, "%f", val);
					break;
				}
				else
				{
					dstate_setinfo(current->info_directive, "%s", range.name);
					break;
				}
			}
		}

		break;
	}
	case ul_type_Bitfield:
	{
		bitfield_info_t info = current->type_info.bitfield_info_t;

		uint8_t string[usage_t->data_size];
		readUsageInto(usage_t, string);

		char destination[512] = {0};

		for (int i = 0; i < (usage_t->data_size) * 8; i++)
		{
			if ((string[(usage_t->data_size - 1) - (i / 8)] & (1 << (i % 8))) != 0)
			{
				char *bitfield_name = "unknown";

				bitfield_value_t *bitfield_values = info.values;

				for (int j = 0; j < info.values_length; j++)
				{
					if (bitfield_values[j].index == i)
					{
						bitfield_name = bitfield_values[j].name;
						break;
					}
				}

				if (strlen(destination) + strlen(bitfield_name) + 1 > 512)
				{
					break;
				}

				snprintfcat(destination, 511, "%s ", bitfield_name);

				/* strcat(destination, bitfield_name);
				strcat(destination, " "); */
			}
		}

		dstate_setinfo(current->info_directive, "%s", destination);

		break;
	}
	case ul_type_DateDays2000:
	{
		if (usage_t->data_size > 4)
		{
			upslogx(LOG_ERR, "Usage too large for date integer reading (%s)", current->info_directive);
			return;
		}

		/* Unsigned int */
		int days = (int)readUsageValueInteger(usage_t, 0);

		/* Work out time since 2000 (Epoch is 1900, month 1, day 0) */
		struct tm t = {.tm_year = 100, .tm_mon = 0, .tm_mday = 1};
		t.tm_mday += (int)days;
		mktime(&t);

		char date_buffer[11];
		strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", &t);

		dstate_setinfo(current->info_directive, "%s", date_buffer);

		break;
	}
	}
}

void update_ups_status(void)
{
	usage_t *usage_upsStatusBF = fetchUsage(FQUN_UPSStatus_BF);
	usage_t *usage_batterySystemErrorBF = fetchUsage(FQUN_BatterySystemError_BF);
	usage_t *usage_runtimeCalibrationStatusBF = fetchUsage(FQUN_RuntimeCalibrationStatus);
	usage_t *usage_simpleSignalingStatusBF = fetchUsage(FQUN_SimpleSignalingStatus_BF);
	usage_t *usage_inputStatusBF = fetchUsage(FQUN_InputStatus_BF);

	if (usage_upsStatusBF == NULL || usage_batterySystemErrorBF == NULL ||
		usage_runtimeCalibrationStatusBF == NULL || usage_simpleSignalingStatusBF == NULL ||
		usage_inputStatusBF == NULL)
	{
		upslogx(LOG_ERR, "Usage not resolved for status update FQUNs (Device does not expose usage_t)");
		return;
	}
	else if (usage_upsStatusBF->data_size > 4 || usage_batterySystemErrorBF->data_size > 4 ||
			 usage_runtimeCalibrationStatusBF->data_size > 4 || usage_simpleSignalingStatusBF->data_size > 4 ||
			 usage_inputStatusBF->data_size > 4)
	{
		upslogx(LOG_ERR, "Usage(s) too large for updating status");
		return;
	}

	uint32_t ups_status_bf = readUsageValueInteger(usage_upsStatusBF, 0);
	uint32_t batt_sys_err_bf = readUsageValueInteger(usage_batterySystemErrorBF, 0);
	uint32_t runtime_cal_status_bf = readUsageValueInteger(usage_runtimeCalibrationStatusBF, 0);
	uint32_t simple_signaling_status_bf = readUsageValueInteger(usage_simpleSignalingStatusBF, 0);
	uint32_t input_status_bf = readUsageValueInteger(usage_inputStatusBF, 0);

	alarm_init();
	status_init();

	/* Online */
	if (ups_status_bf & (1 << 1))
	{
		status_set("OL");
	}
	/* On battery */
	if (ups_status_bf & (1 << 2))
	{
		status_set("OB");
	}
	/* Bypass */
	if (ups_status_bf & (1 << 3))
	{
		status_set("BYPASS");
	}
	/* Off */
	if (ups_status_bf & (1 << 4))
	{
		status_set("OFF");
	}
	/* Test */
	if (ups_status_bf & (1 << 7))
	{
		status_set("TEST");
	}
	/* High Efficiency */
	if (ups_status_bf & (1 << 13))
	{
		status_set("HE");
	}
	/* Overload State */
	if (ups_status_bf & (1 << 21))
	{
		status_set("OVER");
	}

	/* Fault */
	if (ups_status_bf & (1 << 5))
	{
		alarm_set("General fault");
	}
	/* Input bad */
	if (ups_status_bf & (1 << 6))
	{
		alarm_set("Input not acceptable");
	}

	/* Shutdown Imminent */
	if (simple_signaling_status_bf & (1 << 1))
	{
		status_set("LB");
	}

	/* Disconnected (Battery) */
	if (batt_sys_err_bf & (1 << 0))
	{
		alarm_set("Battery disconnected");
		status_set("RB");
	}

	/* NeedsReplacement (Battery) */
	if (batt_sys_err_bf & (1 << 2))
	{
		status_set("RB");
	}

	/* InProgress (Runtime calibration) */
	if (runtime_cal_status_bf & (1 << 1))
	{
		status_set("CAL");
	}

	/* Boost (Input status) */
	if (input_status_bf & (1 << 5))
	{
		status_set("BOOST");
	}

	/* Trim (Input status) */
	if (input_status_bf & (1 << 6))
	{
		status_set("TRIM");
	}

	alarm_commit();
	status_commit();
}

void update_outlet_group_state(void)
{
	usage_t *usage_outletCommandBF = fetchUsage(FQUN_OutletCommand);

	if (usage_outletCommandBF == NULL)
	{
		upslogx(LOG_ERR, "Usage not resolved for outlet goup command FQUN (Device does not expose usage_t)");
		return;
	}

	if (usage_outletCommandBF->bitmask_location == 0)
	{
		upslogx(LOG_ERR, "\"Outlet command\" usage_t has no bitmask.");
		return;
	}

	/* Calculate number of outlet groups */
	uint8_t *outletCommand_bitmask = data_block + usage_outletCommandBF->bitmask_location + usage_outletCommandBF->data_size - 2;

	uint8_t outlet_groups_enabled = outletCommand_bitmask[0] & 0x0F;
	/* upslogx(LOG_INFO, "Outlet group mask: %02X", outlet_groups_enabled); */
	int sog_outlet_count = 0;
	for (int i = 1; i < 4; i++)
	{
		if (outlet_groups_enabled & (1 << i))
		{
			sog_outlet_count++;
		}
	}

	/* Update state for unswitched outlet groups (UOG/MOG) */
	if (outlet_groups_enabled & (1 << 0))
	{
		for (int i = 0; i < switched_outlet_group_usages_length; i++)
		{
			mapped_usage_t *mog1 = switched_outlet_group_usages[i];

			process_mapped_usage(mog1);
		}
		dstate_setinfo("outlet.group.count", "%d", sog_outlet_count + 1);
	}
	else
	{
		dstate_setinfo("outlet.group.count", "%d", sog_outlet_count);
	}

	/* Update state for switched outlet groups (SOG) */
	for (int i = 1; i < 4; i++)
	{
		/* Group disabled */
		if ((outlet_groups_enabled & (1 << i)) == 0)
		{
			continue;
		}
		for (int j = 0; j < switched_outlet_group_usages_length; j++)
		{
			mapped_usage_t *sog_ptr = (switched_outlet_group_usages[j]);
			mapped_usage_t sog = *sog_ptr;

			char info_directive[strlen(sog.info_directive) + 1];
			strcpy(info_directive, sog.info_directive);

			if (i == 1)
			{
				info_directive[13] = '1';
			}
			else if (i == 2)
			{
				info_directive[13] = '2';
			}
			else if (i == 3)
			{
				info_directive[13] = '3';
			}

			sog.info_directive = info_directive;

			fqid_t fqun[MAX_FQIDS] = {0};
			for (int i = 0; i < MAX_FQIDS && sog.fqun[i].id != 0; i++)
			{
				fqun[i] = sog.fqun[i];
			}

			fqun[2] = (fqid_t)FQID_INDEX(0x3D, fqid_collection, i - 1, sog_outlet_count);

			sog.fqun = fqun;

			process_mapped_usage(&sog);
		}
	}
}

void update_all_state(void)
{
	/* Existing mapped usages */
	for (int i = 0; i < updated_usages_length; i++)
	{
		mapped_usage_t *current = updated_usages[i];
		process_mapped_usage(current);
	}

	/* Advanced status update here */
	update_ups_status();

	/* Enumerate outlet groups */
	update_outlet_group_state();

	dstate_dataok();
}

/* Monitor the UPS for 1 iteration, ending on (number_of_rows-1)
Returns 0 on success */
int monitor_ul_device_n(void)
{
	if (state != ul_state_authenticated)
		return 1;

	uint8_t row_data[32];
	microlink_row_t row;

	int first_hit = 0;
	int read_fails = 0;
	while (1)
	{
		/* Flush in queue */
		tcflush(upsfd, TCIFLUSH);

		/* Send ACK */
		ser_send_char(upsfd, 0xFE);

		row = read_ul_row(row_data, 32);

		if (validate_row(row) == 0)
		{
			store_row(row);
			upsdebugx(2, "[Monitor] RX: %i", row.row_num);

			/* If it's a descriptor row then we have left monitor state and it's going to want to reauth
			This shouldn't happen but is here in case */
			if (usage_count > 0 && row.row_num < usage_array[0].data_location / ul_row_size)
			{
				upslogx(LOG_ERR, "UPS Has left auth state during monitor");
				state = ul_state_disconnected;
				return 1;
			}

			if (row.row_num == (number_of_rows - 1))
			{
				if (first_hit)
				{
					/* Completed successful iteration of UL rows */
					ser_comm_good();
					return 0;
				}
				else
				{
					first_hit = 1;
				}
			}

			if (row.row_num == 0)
			{
				state = ul_state_disconnected;
				ser_comm_fail("Device has entered setup, read fail.");
				return 1;
			}
		}
		else
		{
			read_fails++;
			if (read_fails >= 3)
			{
				state = ul_state_disconnected;
				ser_comm_fail("Device failed to respond with valid row after 3 attempts.");
				return 1;
			}
		}
	}
}

ssize_t read_serial_into(uint8_t *buffer, size_t bytes, int timeoutms)
{
	if (timeoutms < 0)
	{
		return 0;
	}

	ssize_t bytes_read = ser_get_buf_len(upsfd, buffer, bytes, 0, 1000 * timeoutms);

	if (bytes_read < 0)
	{
		bytes_read = 0;
	}

	return bytes_read;
}

/* Read row from the microlink master (UPS) */
microlink_row_t read_ul_row(uint8_t *ret_array, int length)
{
	/* Should be less than ~730 to prevent device from resetting */
	int timeout_ms_readrow = 500;

	microlink_row_t row;
	row.length = -1;

	if (length < 32)
	{
		upslogx(LOG_ERR,
				"[Row] Buffer given too small for reading the row");

		return row;
	}

	ssize_t ret = 0;

	/* Wait for row 0 char 0 */
	ret = read_serial_into(ret_array, 1, timeout_ms_readrow);

	if (ret != 1)
	{
		upslogx(LOG_ERR,
				"[Row] Did not return first char after 0.5 seconds! Serial sent %li bytes", ret);

		return row;
	}

	if (ret_array[0] == 0x00)
	{
		/* Read 10 chars */
		ret = read_serial_into(ret_array + 1, 10, timeout_ms_readrow);
		if (ret != (ssize_t)10)
		{
			upslogx(LOG_ERR,
					"[Row0] Did not return next 10 chars. Serial sent %li bytes", ret);

			return row;
		}

		/* Only update this if it's disconnected. Corrupt data would otherwise potentially break it during monitoring */
		if (state == ul_state_disconnected)
		{
			ul_row_size = ret_array[2];
		}
		/* Set remaining characters to read */
		uint8_t remaining = ul_row_size - 8;

		if (!(remaining > 0 && ul_row_size < 30))
		{
			upslogx(LOG_ERR,
					"[Row0] Calculated remaining bytes invalid: %i bytes", remaining);

			ul_row_size = 0;

			return row;
		}

		/* Read rest of row */
		ret = read_serial_into(ret_array + 11, remaining, timeout_ms_readrow);
		if (ret != remaining)
		{
			upslogx(LOG_ERR,
					"[Row0] Remaining bytes not returned: Serial sent %li bytes", ret);

			return row;
		}
	}
	else
	{
		if (ul_row_size == 0)
		{
			return row;
		}

		ret = read_serial_into(ret_array + 1, ul_row_size + 2, timeout_ms_readrow);

		if (ret != ul_row_size + 2)
		{
			upslogx(LOG_ERR,
					"[Row] Remaining bytes not returned: Serial sent %li bytes", ret);

			return row;
		}
	}

	row.array = ret_array;
	row.length = ul_row_size + 3;
	row.row_num = ret_array[0];

	return row;
}

void print_hex(uint8_t *buffer, size_t bytes)
{
	for (size_t i = 0; i < bytes; i++)
	{
		printf("0x%02X", (unsigned char)buffer[i] % 256);
		printf(", ");
	}
	printf("\n");
}

int paused_last_time = 0;

void upsdrv_updateinfo(void)
{
	if (state != ul_state_authenticated)
	{
		upslogx(LOG_ERR, "Device is not connected, attempting to reconnect");
		dstate_datastale();

		/* Try to reconnect here, potential issue may take more than 6 seconds. Use ser_comm_good after successful monitor_ul_device_n. */
#ifndef DEBUG_DATASTORE
		for (int connect_tries = 0; state != ul_state_connected && connect_tries < 3; connect_tries++)
		{
			usleep(100 * 1000);
			detect_ul_device();
		}

		if (state != ul_state_connected)
		{
			return;
		}

		setup_ul_device();

		if (state != ul_state_authenticated)
		{
			upslogx(LOG_ERR, "Failed to setup or authenticate");
			state = ul_state_disconnected;
			return;
		}
#endif
	}

#ifndef DEBUG_DATASTORE
	if (monitor_ul_device_n() != 0)
	{
		return;
	}
#endif

	update_all_state();

	upslogx(LOG_INFO, "Successfully polled for data");

	/* Reduce poll interval if it keeps disconnecting (~730ms is maximum time UPS goes without losing auth) */
	/*
	 * poll_interval = 2;
	 */
}

void upsdrv_shutdown(void)
{
	/* tell the UPS to shut down, then return - DO NOT SLEEP HERE */

	/* maybe try to detect the UPS here, but try a shutdown even if
	   it doesn't respond at first if possible */

	/* replace with a proper shutdown function */
	upslogx(LOG_ERR, "shutdown not supported");
	set_exit_flag(-1);

	/* you may have to check the line status since the commands
	   for toggling power are frequently different for OL vs. OB */

	/* OL: this must power cycle the load if possible */

	/* OB: the load must remain off until the power returns */
}

static int instcmd(const char *cmdname, const char *extra)
{
	NUT_UNUSED_VARIABLE(extra);
	
	if (state != ul_state_authenticated)
	{
		return STAT_INSTCMD_FAILED;
	}

	for (int i = 0; i < instcmds_length; i++)
	{
		if (strcasecmp(cmdname, instcmds[i]->command_directive) == 0)
		{
			mapped_command_t *command = instcmds[i];

			usage_t *usage_t = fetchUsage(command->fqun);

			if (usage_t == NULL)
			{
				upslogx(LOG_ERR, "Usage not resolved for FQUN (Device does not expose usage_t)");
				return STAT_INSTCMD_FAILED;
			}

			uint8_t write_data[usage_t->data_size];
			memset(write_data, 0, usage_t->data_size * sizeof(uint8_t));
			memcpy(write_data, command->command, command->command_length);

			if (write_usage(usage_t, write_data, 1) != 0)
			{
				return STAT_INSTCMD_FAILED;
			}

			return STAT_INSTCMD_HANDLED;
		}
	}

	upslogx(LOG_NOTICE, "instcmd: unknown command [%s]", cmdname);
	return STAT_INSTCMD_UNKNOWN;
}

static int setvar(const char *varname, const char *val)
{
	/* ser_send_char(upsfd, 0xF7);
	usleep(100 * 1000);
	REMOVE THIS printf TESTING
	Send NAK to reset and break everything
	for (int i = 0; i < 10; i++) {
		ser_send_char(upsfd, 0xFD);
		usleep(100 * 1000);
	}
	return STAT_SET_FAILED; */

	if (state != ul_state_authenticated)
	{
		return STAT_SET_FAILED;
	}

	for (int i = 0; i < updated_usages_length; i++)
	{
		mapped_usage_t *current_mapped = updated_usages[i];

		if (strcasecmp(varname, current_mapped->info_directive))
		{
			continue;
		}

		if (!(current_mapped->flags && APC_UL_FLAG_SETVAR))
		{
			continue;
		}

		usage_t *usage_t = fetchUsage(current_mapped->fqun);

		if (usage_t == NULL)
		{
			upslogx(LOG_WARNING, "Usage not resolved for FQUN (Device does not expose usage_t)");
			return STAT_SET_FAILED;
		}

		/* Use the data size described in the USAGE not the mapping (as the mapping may be wrong) */
		switch (current_mapped->type)
		{
		case ul_type_String:
		{
			char string[usage_t->data_size];
			memset(string, 0x20, usage_t->data_size);

			for (int j = 0; j < usage_t->data_size && val[j] != '\0'; j++)
			{
				string[j] = val[j];
			}

			if (write_usage(usage_t, (uint8_t *)string, 1) != 0)
			{
				return STAT_SET_FAILED;
			}

			break;
		}
		case ul_type_Int:
		{
			int_info_t info = current_mapped->type_info.int_info_t;

			char *end;
			errno = 0;
			int64_t value_parse;
			value_parse = strtoll(val, &end, 0);
			if (end == val || *end != '\0' || errno > 0)
			{
				return STAT_SET_CONVERSION_FAILED;
			}

			if (write_usage_integer(usage_t, value_parse, info._signed) != 0)
			{
				return STAT_SET_FAILED;
			}

			break;
		}
		case ul_type_Enum:
		{
			/* Acts like an integer (for exposed variables at least) */
			enum_info_t info = current_mapped->type_info.enum_info_t;

			char *end;
			errno = 0;
			int64_t value_parse;
			value_parse = strtoll(val, &end, 0);
			if (end == val || *end != '\0' || errno > 0)
			{
				return STAT_SET_CONVERSION_FAILED;
			}

			if (write_usage_integer(usage_t, value_parse, info._signed) != 0)
			{
				return STAT_SET_FAILED;
			}

			break;
		}
		case ul_type_Bitfield:
		{
			/* Acts like an enum (as all exposed are exclusive) */
			if (!(current_mapped->flags && APC_UL_FLAG_BITMASK_EXCLUSIVE))
			{
				return STAT_SET_INVALID;
			}

			bitfield_info_t info = current_mapped->type_info.bitfield_info_t;

			for (int j = 0; j < info.values_length; j++)
			{
				if (!strcasecmp(val, info.values[j].name))
				{
					int index = info.values[j].index;

					if (index >= (usage_t->data_size * 8))
					{
						upslogx(LOG_ERR, "Bitfield index too large for usage_t size exposed by device");
						return STAT_SET_FAILED;
					}

					uint8_t val[usage_t->data_size];
					memset(val, 0, usage_t->data_size);

					val[usage_t->data_size - 1 - (index / 8)] = 1 << (index % 8);

					if (write_usage(usage_t, val, 1) != 0)
					{
						return STAT_SET_FAILED;
					}
				}
			}

			break;
		}
		case ul_type_DateDays2000:
		{
			struct tm user_time = {0};

			if (strptime(val, "%Y-%m-%d", &user_time) == NULL)
			{
				return STAT_SET_CONVERSION_FAILED;
			}

			struct tm start = {.tm_year = 100, .tm_mon = 0, .tm_mday = 1};

			time_t tt_user_time = timegm(&user_time);
			time_t tt_start = timegm(&start);

			if (tt_user_time < tt_start)
			{
				upslogx(LOG_ERR, "Date entered was before 1st Jan 2000");
				return STAT_SET_INVALID;
			}

			int64_t days = (tt_user_time - tt_start) / 86400LL;

			if (write_usage_integer(usage_t, days, 0) != 0)
			{
				return STAT_SET_FAILED;
			}

			break;
		}
		}
		upslogx(LOG_INFO, "Wrote variable successfully");
		return STAT_SET_HANDLED;
	}

	upslogx(LOG_NOTICE, "setvar: unknown variable [%s]", varname);
	return STAT_SET_UNKNOWN;
}

void upsdrv_initinfo(void)
{
	/* try to detect the UPS here - call fatal_with_errno(EXIT_FAILURE, ...)
	 * or fatalx(EXIT_FAILURE, ...) if it fails */

#ifndef DEBUG_DATASTORE
	int connect_tries = 0;
	while (state != ul_state_connected)
	{
		if (connect_tries > 3)
		{
			/* Sometimes when running on battery, response is delayed so extra 0x00 is sent while uart rx line drops */
			fatalx(EXIT_FAILURE, "Device not detected after 3 tries!");
		}

		detect_ul_device();

		if (state != ul_state_connected)
		{
			/* In case it half-read a corrupted row last time, there's still a transmission in progress, wait 100ms. */
			usleep(100 * 1000);
		}

		connect_tries++;
	}

	setup_ul_device();
#else
	data_block = debug_datastore;
	ul_row_size = 16;
	number_of_rows = 141;

	usage_count = parseDatastore(data_block, number_of_rows,
								 ul_row_size, usage_array);
	upsdebugx(1, "Loaded descriptor with %i usages", usage_count);

	state = ul_state_authenticated;
#endif

	if (state == ul_state_authenticated)
	{
		upslogx(LOG_INFO, "Auth success, starting to monitor");
#ifndef DEBUG_DATASTORE
		if (monitor_ul_device_n() != 0)
		{
			fatalx(EXIT_FAILURE, "Failed to monitor device.");
		}
#endif
	}
	else
	{
		fatalx(EXIT_FAILURE, "Device not found");
	}

	update_all_state();

	/* Add all instcmds */
	for (int i = 0; i < instcmds_length; i++)
	{
		dstate_addcmd(instcmds[i]->command_directive);
	}

	/* Add all R/W variables */
	for (int i = 0; i < updated_usages_length; i++)
	{
		mapped_usage_t *current_mapped = updated_usages[i];

		if (!(current_mapped->flags && APC_UL_FLAG_SETVAR))
		{
			continue;
		}

		usage_t *usage_t = fetchUsage(current_mapped->fqun);

		if (usage_t == NULL)
		{
			upslogx(LOG_WARNING, "Usage not resolved for FQUN (Device does not expose usage_t)");
			continue;
		}

		switch (current_mapped->type)
		{
		case ul_type_String:
		{
			dstate_setflags(current_mapped->info_directive, ST_FLAG_STRING | ST_FLAG_RW);
			dstate_setaux(current_mapped->info_directive, usage_t->data_size);
			break;
		}
		case ul_type_Int:
		{
			int_info_t info = current_mapped->type_info.int_info_t;

			int64_t min = readUsageMinInteger(usage_t, info._signed);
			int64_t max = readUsageMaxInteger(usage_t, info._signed);

			dstate_addrange(current_mapped->info_directive, (int)min, (int)max);

			dstate_setflags(current_mapped->info_directive, ST_FLAG_NUMBER | ST_FLAG_RW);
			break;
		}
		case ul_type_Enum:
		{
			enum_info_t info = current_mapped->type_info.enum_info_t;

			/* TODO: Consider min/max to override ranges */
			for (int i = 0; i < info.ranges_length; i++)
			{
				enum_range_t range = info.ranges[i];

				dstate_addrange(current_mapped->info_directive, (int)range.start, (int)range.end);
			}

			dstate_setflags(current_mapped->info_directive, ST_FLAG_NUMBER | ST_FLAG_RW);
			break;
		}
		case ul_type_Bitfield:
		{
			if (!(current_mapped->flags && APC_UL_FLAG_BITMASK_EXCLUSIVE))
			{
				break;
			}
			bitfield_info_t info = current_mapped->type_info.bitfield_info_t;

			uint8_t string[usage_t->data_size];
			readUsageBitmaskInto(usage_t, string);

			for (int i = 0; i < (usage_t->data_size) * 8; i++)
			{
				if ((string[(usage_t->data_size - 1) - i / 8] & (1 << (i % 8))) != 0)
				{
					char *bitfield_name = "unknown";

					bitfield_value_t *bitfield_values = info.values;

					for (int j = 0; j < info.values_length; j++)
					{
						if (bitfield_values[j].index == i)
						{
							bitfield_name = bitfield_values[j].name;
							break;
						}
					}

					dstate_addenum(current_mapped->info_directive, "%s", bitfield_name);
				}
			}

			dstate_setflags(current_mapped->info_directive, ST_FLAG_RW);
			break;
		}
		case ul_type_DateDays2000:
		{
			dstate_setflags(current_mapped->info_directive, ST_FLAG_STRING | ST_FLAG_RW);
			dstate_setaux(current_mapped->info_directive, 16);
			break;
		}
		}
	}

	upsh.instcmd = instcmd;
	upsh.setvar = setvar;

	upsdebugx(1, "Initinfo setup complete");
}

void upsdrv_help(void) {}

/* list flags and values that you want to receive via -x */
void upsdrv_makevartable(void)
{
	/* allow '-x xyzzy' */
	/* addvar(VAR_FLAG, "xyzzy", "Enable xyzzy mode"); */

	/* allow '-x foo=<some value>' */
	/* addvar(VAR_VALUE, "foo", "Override foo setting"); */
}

void upsdrv_initups(void)
{
	/* don't try to detect the UPS here */
#ifndef DEBUG_DATASTORE
	upsfd = ser_open(device_path);
	ser_set_speed(upsfd, device_path, B9600);
#endif

	/* TODO: Check for com port / USB vid/pid */

	/* APCs VID is 0x051D, PID is 0x0003 or 0x0004 */
}

void upsdrv_cleanup(void)
{
	ser_close(upsfd, device_path);
	free(data_block);
	data_block = NULL;
}
