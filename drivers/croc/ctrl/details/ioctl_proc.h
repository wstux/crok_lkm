/*
 * crocodile_lkm
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

#ifndef _CROCODILE_LKM_CTRL_IOCTL_PROC_H_
#define _CROCODILE_LKM_CTRL_IOCTL_PROC_H_

#include "types.h"

int ioc_hide_proc(module_dev_t* p_dev, pid_t pid);

int ioc_reset(module_dev_t* p_dev);

int ioc_show_proc(module_dev_t* p_dev, pid_t pid);

#endif /* _CROCODILE_LKM_CTRL_IOCTL_PROC_H_ */

