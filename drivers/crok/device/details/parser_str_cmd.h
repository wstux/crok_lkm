/*
 * crok_lkm
 * Copyright (C) 2023  Chistyakov Alexander
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CROK_LKM_DEVICE_PARSER_STR_CMD_H_
#define _CROK_LKM_DEVICE_PARSER_STR_CMD_H_

#include <linux/types.h>

int parse_str_cmd(char* p_cmd_str, size_t count, unsigned int* p_cmd, unsigned long* p_arg);

#endif /* _CROK_LKM_DEVICE_PARSER_STR_CMD_H_ */

