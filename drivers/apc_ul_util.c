/*
 * apc_ul_util.c - APC Microlink driver descriptor parser and useful functions
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

#include "apc_ul_util.h"

#define APC_UL_PARSE_DEBUG 0

void fletcher16Sum(uint8_t *packet, int length, uint8_t *ret_cs)
{
	uint8_t sum1 = 0;
	uint8_t sum2 = 0;
	for (int i = 0; i < length; ++i)
	{
		sum1 = (sum1 + packet[i]) % 255;
		sum2 = (sum1 + sum2) % 255;
	}
	ret_cs[0] = (uint8_t)sum1;
	ret_cs[1] = (uint8_t)sum2;
}

/*  Use ret_cs in the calculation of the checksum instead of starting with 0 */
void fletcher16SumContinue(uint8_t *packet, int length, uint8_t *ret_cs)
{
	uint8_t sum1 = ret_cs[0];
	uint8_t sum2 = ret_cs[1];
	for (int i = 0; i < length; ++i)
	{
		sum1 = (sum1 + packet[i]) % 255;
		sum2 = (sum1 + sum2) % 255;
	}
	ret_cs[0] = (uint8_t)sum1;
	ret_cs[1] = (uint8_t)sum2;
}

/* Compute checksum and store to ret_cs appended to end of packet */
void calcChecksum(uint8_t *packet, int length, uint8_t *ret_cs)
{
	fletcher16Sum(packet, length, ret_cs);

	uint8_t sum1 = ret_cs[0];
	uint8_t sum2 = ret_cs[1];
	ret_cs[0] = (uint8_t)(~((sum1 + sum2) % 255) & 255);
	ret_cs[1] = (uint8_t)(~((ret_cs[0] + sum1) % 255) & 255);
}

/* Data length is length of the useful part of packet, array size is size of "packet" array */
int addChecksumToMessage(uint8_t *packet, int data_length, int array_size)
{
	if (data_length + 2 > array_size)
		return 1;

	calcChecksum(packet, data_length, packet + data_length);
	return 0;
}

int validateChecksum(uint8_t *packet, int packet_length)
{
	uint8_t calc_cs[2];
	fletcher16Sum(packet, packet_length, calc_cs);
	if (calc_cs[0] == 0 && calc_cs[1] == 0)
	{
		return 0;
	}
	return 1;
}

/* Returns 0 if row is valid */
int validate_row(microlink_row_t row)
{
	if (row.length <= 0)
	{
		return 1;
	}
	if (validateChecksum(row.array, row.length) != 0)
	{
		return 2;
	}
	return 0;
}

/* Converts an FQUN to a string (Either ID 0 is reached or MAX_FQIDS is reached)
Maximum length of output string is ( 13 * MAX_FQIDS ) + 1 or FQUN_AS_STRING_MAX_LENGTH */
int fqun_to_string(const fqid_t *fqun, char *dest)
{
	int ptr_string = 0;
	for (int i = 0; i < MAX_FQIDS && fqun[i].id != 0; i++)
	{
		fqid_t current_fqid = fqun[i];

		snprintf(dest + ptr_string, 3, "%02X", current_fqid.id);
		ptr_string += 2;

		switch (current_fqid.type)
		{
		case fqid_collection:
			if (current_fqid.array_length > 0)
			{
				snprintf(dest + ptr_string, 12, "[%-3i, %-3i]", current_fqid.array_index % 256, current_fqid.array_length % 256);
				ptr_string += 10;
			}
			dest[ptr_string++] = '.';
			break;
		case fqid_namespace:
			dest[ptr_string++] = ':';
			break;
		case fqid_usage:
			break;
		}
	}
	dest[ptr_string++] = '\0';
	return ptr_string;
}

/* Converts a Usage to a string written to dest (FQUN_AS_STRING_MAX_LENGTH + 63) to be safe */
void usage_to_string(usage_t current, char *dest)
{
	int ptr_string = fqun_to_string(current.fqun, dest) - 1;
	ptr_string += snprintf(dest + ptr_string, 60, " Size: %u Data location: %lu",
						   current.data_size, current.data_location);
	dest[ptr_string++] = '\0';
}

/* Stack variables */
fqid_t fqun_stack[MAX_FQIDS];
unsigned int fqid_stack_pointer = 0;

usage_t *parsed_usages = NULL;
unsigned int parsed_usage_pointer = 0;

/* Compare fqids in the FQUN upto and including the depth
Returns 0 if they are identical */
int compareFQUN(const fqid_t *source, const fqid_t *target, int depth)
{
	for (int i = 0; i <= depth; i++)
	{
		if (source[i].id != target[i].id)
		{
			return 1;
		}
		if (source[i].type != target[i].type)
		{
			return 1;
		}
		if (source[i].type == fqid_collection)
		{
			if (source[i].array_length != target[i].array_length && source[i].array_length != COLLECTION_IGNORE_LENGTH)
			{
				return 1;
			}
			if (source[i].array_index != target[i].array_index)
			{
				return 1;
			}
		}
	}
	return 0;
}

int fqun_stack_push(fqid_t new_fqid)
{
	if (fqid_stack_pointer >= MAX_FQIDS)
	{
		return 1;
	}
	fqun_stack[fqid_stack_pointer++] = new_fqid;
	return 0;
}

fqid_t *fqun_stack_pop(void)
{
	if (fqid_stack_pointer < 1)
	{
		return NULL;
	}
	fqid_stack_pointer--;
	return &(fqun_stack[fqid_stack_pointer]);
}

void pushNamespace(uint8_t id)
{
	fqid_t new_fqid = {.id = id, .type = fqid_namespace};
	fqun_stack_push(new_fqid);
}

void pushCollection(uint8_t id)
{
	fqid_t new_fqid = {.id = id,
					   .type = fqid_collection,
					   .array_index = -1,
					   .array_length = -1};
	fqun_stack_push(new_fqid);
}

void pushIndexedCollection(uint8_t id, uint8_t array_size)
{
	fqid_t new_fqid = {.id = id,
					   .type = fqid_collection,
					   .array_index = 0,
					   .array_length = array_size};
	fqun_stack_push(new_fqid);
}

int addUsage(uint8_t id)
{
	if (parsed_usage_pointer >= MAX_USAGES)
	{
		upsdebugx(2, "Failed to add usage_t, max (512) usages exceeded");
		return 1;
	}

	for (unsigned int i = 0; i < fqid_stack_pointer; i++)
	{
		parsed_usages[parsed_usage_pointer].fqun[i] = fqun_stack[i];
	}
	fqid_t usage_fqid = {.id = id, .type = fqid_usage};
	parsed_usages[parsed_usage_pointer].fqun[fqid_stack_pointer] = usage_fqid;
	parsed_usages[parsed_usage_pointer].fqun_stack_size =
		fqid_stack_pointer + 1;

	parsed_usages[parsed_usage_pointer].data_size = 2;
	parsed_usages[parsed_usage_pointer].data_location = 0;

	parsed_usages[parsed_usage_pointer].bitmask_location = -1;
	parsed_usages[parsed_usage_pointer].min_location = -1;
	parsed_usages[parsed_usage_pointer].max_location = -1;

	parsed_usage_pointer++;
	upsdebugx(2, "Added usage_t with id 0x%02X at position %i", id,
			  parsed_usage_pointer);
	return 0;
}

void setUsageSize(uint8_t data_size)
{
	parsed_usages[parsed_usage_pointer - 1].data_size = data_size;
	upsdebugx(2, "Set usage_t size to 0x%02X", data_size);
}

void setMinLocation(int min_location)
{
	parsed_usages[parsed_usage_pointer - 1].min_location = min_location;
}

void setMaxLocation(int max_location)
{
	parsed_usages[parsed_usage_pointer - 1].max_location = max_location;
}

void setBitmaskLocation(int bitmask_location)
{
	parsed_usages[parsed_usage_pointer - 1].bitmask_location = bitmask_location;
}

void exitNamespaceCollection(void)
{
	fqid_t exited = *fqun_stack_pop();

	if (exited.type == fqid_collection && exited.array_length > 0)
	{
		/* Now points to the index of the last usage_t in the stack */
		unsigned int temp_usage_pointer = parsed_usage_pointer - 1;

		while (parsed_usages[temp_usage_pointer].fqun_stack_size > fqid_stack_pointer &&
			   compareFQUN(fqun_stack, parsed_usages[temp_usage_pointer].fqun, fqid_stack_pointer) == 0)
		{
			temp_usage_pointer--;
		}

		/* Points to the beginning and end of the ones to duplicate */
		unsigned int start = temp_usage_pointer + 1;
		unsigned int end = parsed_usage_pointer;

		if (start >= end)
		{
			/* We can assume "end" is valid as parsed_usage_pointer is never updated without first checking */
			upslogx(LOG_ERR, "Parsing error, found incorrect / too many elements in same collection, underflow");
			return;
		}

		for (uint8_t i = 1; i < exited.array_length; i++)
		{
			for (unsigned int j = start; j < end; j++)
			{
				if (parsed_usage_pointer >= MAX_USAGES)
				{
					upslogx(LOG_ERR, "Too many usages in stack, failed!");
					return;
				}
				parsed_usages[parsed_usage_pointer++] = parsed_usages[j];
				parsed_usages[parsed_usage_pointer - 1]
					.fqun[fqid_stack_pointer]
					.array_index = i;
			}
		}
	}
}

void calcDataLocation(size_t data_location)
{
	for (unsigned int i = 0; i < parsed_usage_pointer; i++)
	{
		usage_t current = parsed_usages[i];
		current.data_location = data_location;
		data_location += current.data_size;
		parsed_usages[i] = current;
	}
}

void printMap(void)
{
	upsdebugx(2, "==== Usage map begin ====");
	for (unsigned int i = 0; i < parsed_usage_pointer; i++)
	{
		usage_t current = parsed_usages[i];

		char usage_string[USAGE_AS_STRING_MAX_LENGTH] = {0};
		usage_to_string(current, usage_string);

		upsdebugx(2, "@%03i %s", i, usage_string);
	}
	upsdebugx(2, "==== Usage map end ====");
}

/* Related to parsing the descriptor, returns number of usages read, -1
if failed */
int parseDatastore(uint8_t *data_block, uint8_t number_of_rows,
				   uint8_t row_size, usage_t *usage_array)
{
	fqid_stack_pointer = 0;
	parsed_usage_pointer = 0;
	parsed_usages = usage_array;

	/* Row 0 (the header) bytes 10 and 11 store the beginning of the
	data which is pointed to by the usages */
	size_t data_location = (data_block[10] * 16) + data_block[11];

	int currentPosition = 12;

	int size = 2;

	while (currentPosition < (number_of_rows * row_size))
	{
		uint8_t cur_byte = data_block[currentPosition];

		upsdebugx(2, "Processing byte 0x%02X, on position %i (row %i, value %i)",
				  cur_byte,
				  currentPosition,
				  currentPosition / 16,
				  currentPosition % 16);

		switch (cur_byte)
		{
		case 0xe0: /* 224 */
			return -1;
			break;
		default: /* 0 to 223, 225 to 243 */
			if (cur_byte >= 1 && cur_byte <= 223)
			{
				/* TODO: Don't save it if it's 1,2 or 3 because they are padding. Currently required for data location to be set correctly */

				addUsage(cur_byte);
			}
			else if (cur_byte >= 226 && cur_byte <= 243)
			{
				/* Something involving skip flag. May cause issues */
				return -1;
			}
			else if (cur_byte == 225 || cur_byte == 0)
			{
				/* Makes "Usage ID Out of range" exception in original code */
				return -1;
			}
			size = 2;
			break;
		case 0xf4: /* 244 */
			return -1;
			break;
		case 0xf5: /* 245 */
			break;
		case 0xf6: /* 246 End of descriptor */
			upsdebugx(2, "Finished reading descriptor!");
			currentPosition = number_of_rows * row_size;
			calcDataLocation(data_location);

			printMap();
			break;
		case 0xf7: /* 247 NO-OP, At end of descriptor */

			break;
		case 0xf8: /* 248 namespace */
		{
			uint8_t new_ns = data_block[++currentPosition];

			upsdebugx(2, "Entering namespace: 0x%02X", new_ns);

			pushNamespace(new_ns);

			break;
		}
		case 0xf9: /* 249 Default Value */
		{
			currentPosition += size;

			upsdebugx(2, "Read default value for usage_t.");

			break;
		}
		case 0xfa: /* 250 Max/Min Operator */
		{
			setMaxLocation(currentPosition + 1);
			currentPosition += size;

			setMinLocation(currentPosition + 1);
			currentPosition += size;

			upsdebugx(2, "Read min/max values for usage_t.");

			break;
		}
		case 0xfb: /* 251 Bitmask */
		{
			setBitmaskLocation(currentPosition + 1);
			currentPosition += size;

			upsdebugx(2, "Read mask values for usage_t.");

			break;
		}
		case 0xfc: /* 252 Size operator */
			size = data_block[++currentPosition];

			setUsageSize(size);

			break;
		case 0xfd: /* 253 Collection (array) */
		{
			uint8_t id_char = data_block[++currentPosition];
			uint8_t array_size = data_block[++currentPosition];

			upsdebugx(2, "Entering %i indexed collections. ID: 0x%02X",
					  (int)array_size, id_char);

			pushIndexedCollection(id_char, array_size);
		}
		break;
		case 0xfe: /* 254 Collection (normal) */
		{
			char id_char = data_block[++currentPosition];

			upsdebugx(2, "Entering collection 0x%02X", id_char);

			pushCollection(id_char);
		}
		break;
		case 0xff:
			upsdebugx(2, "Leaving namespace/collection");

			exitNamespaceCollection();

			break;
		}
		currentPosition++;
	}
	return parsed_usage_pointer;
}

/* slave_password is a 4 byte destination array. Header is first 8 bytes of data
block. Serial number (14 bytes) and password (4b) can be obtained through
usages */
void create_slave_password(uint8_t *master_password, int master_password_length,
						   uint8_t *microlink_header, int header_length,
						   uint8_t *serial_number, int serial_num_length,
						   uint8_t *slave_password)
{
	/* Can be set to anything (can be random) */
	slave_password[0] = 0x00;
	slave_password[1] = (master_password[1] + 1) % 256;

	uint8_t sum[2] = {
		microlink_header[4],
		microlink_header[3]};

	fletcher16SumContinue(microlink_header, header_length, sum);
	fletcher16SumContinue(serial_number, serial_num_length, sum);
	fletcher16SumContinue(master_password, master_password_length, sum);

	slave_password[2] = (uint8_t)sum[0];
	slave_password[3] = (uint8_t)sum[1];
}

/* Takes slave password (first 2 characters), header and serial number.
Returns last 2 bytes of master password for comparsion (stored in master_password_verify) */
void create_master_password_verify(uint8_t *slave_password, int slave_password_length,
								   uint8_t *microlink_header, int header_length,
								   uint8_t *serial_number, int serial_num_length,
								   uint8_t *master_password_verify)
{
	uint8_t sum[2] = {
		microlink_header[4],
		microlink_header[3]};

	fletcher16SumContinue(microlink_header, header_length, sum);
	fletcher16SumContinue(serial_number, serial_num_length, sum);
	fletcher16SumContinue(slave_password, slave_password_length, sum);

	master_password_verify[0] = (uint8_t)sum[0];
	master_password_verify[1] = (uint8_t)sum[1];
}