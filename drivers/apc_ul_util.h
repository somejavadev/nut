/*
 * apc_ul_util.h - APC Microlink driver defines for descriptor parser and
 *                 useful functions
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

#ifndef NUT_APC_UL_UTIL_H_SEEN
#define NUT_APC_UL_UTIL_H_SEEN 1

#include "apc_ul.h"
#include "main.h"

#define MAX_USAGES	512

#define MAX_FQIDS	8

#define FQUN_AS_STRING_MAX_LENGTH	(13 * MAX_FQIDS) + 1
#define USAGE_AS_STRING_MAX_LENGTH	(FQUN_AS_STRING_MAX_LENGTH + 63)

#define COLLECTION_IGNORE_LENGTH	-2

#define FQID(id_i, type_i)	{.id = (id_i), .type = (type_i), .array_index = -1, .array_length = -1}
#define FQID_INDEX(id_i, type_i, index_i, length_i)	{.id = (id_i), .type = (type_i), .array_index = (index_i), .array_length = (length_i)}

#define FQUN(...) \
	{__VA_ARGS__}

enum fqid_type
{
	fqid_namespace = 0,
	fqid_collection = 1,
	fqid_usage = 2
};

typedef struct
{
	uint8_t id;
	enum fqid_type type;
	/* Array details, negative length used for undetermined size */
	int array_index;
	int array_length;
} fqid_t;

typedef struct
{
	fqid_t fqun[MAX_FQIDS];	/* Fixed array of fqids, designed to be used as the */
	size_t fqun_stack_size;	/* Current number of valid fqids in the stack */

	size_t data_location;	/* Data location of the Usage (in the data block) */

	size_t bitmask_location;/* Data location for the mask */
	size_t min_location;	/* Data location for the minimum value */
	size_t max_location;	/* Data location for the maximum value */

	uint8_t data_size;
} usage_t;

enum ul_type
{
	ul_type_String = 0,
	ul_type_Int = 1,
	ul_type_Enum = 2,
	ul_type_Bitfield = 3,
	ul_type_DateDays2000 = 4,
};

/* For integers (e.g. signed short int), can be decimal, negative etc. */
typedef struct
{
	int _signed;
	int decimal_point;
} int_info_t;

/* For enum (e.g. for efficiency), is always 2 bytes and signed (so far) */
typedef struct
{
	char *name;
	/* If show_value is 1, the interpreted value is shown, if not, the name is shown */
	int show_value;
	int start;
	int end;
} enum_range_t;

typedef struct
{
	enum_range_t *ranges;
	int ranges_length;
	int _signed;
	int decimal_point;
} enum_info_t;

/* For string, empty for now
typedef struct {
} string_info; */

/* For bitfield */
typedef struct
{
	char *name;
	int index;
} bitfield_value_t;

typedef struct
{
	/* bitfield_cmd *commands;
	int commands_length; */
	bitfield_value_t *values;
	int values_length;
} bitfield_info_t;

/* Date info, empty for now
typedef struct {
} date_info; */

typedef union
{
	int_info_t int_info_t;
	enum_info_t enum_info_t;
	bitfield_info_t bitfield_info_t;
} usage_type_info_t;

#define APC_UL_FLAG_SETVAR		(1 << 0)
#define APC_UL_FLAG_BITMASK_EXCLUSIVE	(1 << 1)
#define APC_UL_FLAG_IGNORE_MISSING	(1 << 2)

typedef struct
{
	const fqid_t *fqun;
	char *info_directive;

	enum ul_type type;
	uint8_t flags;
	usage_type_info_t type_info;

	int size;
} mapped_usage_t;

typedef struct
{
	const fqid_t *fqun;
	char *command_directive;

	uint8_t *command;
	int command_length;
} mapped_command_t;

enum ul_state
{
	ul_state_disconnected = 0,
	ul_state_connected = 1,
	ul_state_authenticated = 2
};

typedef struct
{
	uint8_t *array;
	int length;
	uint8_t row_num;
} microlink_row_t;

int fqun_to_string(const fqid_t *fqun, char *dest);

int compareFQUN(const fqid_t *source, const fqid_t *target, int depth);

int validate_row(microlink_row_t row);

int addChecksumToMessage(uint8_t *packet, int data_length, int array_size);

int parseDatastore(uint8_t *data_block, uint8_t number_of_rows,
				   uint8_t row_size, usage_t *usage_array);

void create_slave_password(uint8_t *master_password, int master_password_length,
						   uint8_t *microlink_header, int header_length,
						   uint8_t *serial_number, int serial_num_length,
						   uint8_t *slave_password);

void create_master_password_verify(uint8_t *slave_password, int slave_password_length,
								   uint8_t *microlink_header, int header_length,
								   uint8_t *serial_number, int serial_num_length,
								   uint8_t *master_password_verify);

#endif /* NUT_APC_UL_UTIL_H_SEEN */
