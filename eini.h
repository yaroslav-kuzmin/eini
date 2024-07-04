/*****************************************************************************/
/*                                                                           */
/* eini - library for parsing ini files .                                    */
/*                                                                           */
/* Copyright © 2024 ☭ Kuzmin Yaroslav <kuzmin.yaroslav@gmail.com>            */
/*                                                                           */
/* eini is free software: you can redistribute it and/or modify it           */
/* under the terms of the GNU General Public License as published by the     */
/* Free Software Foundation, either version 3 of the License, or             */
/* (at your option) any later version.                                       */
/*                                                                           */
/* eini is distributed in the hope that it will be useful, but               */
/* WITHOUT ANY WARRANTY; without even the implied warranty of                */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                      */
/* See the GNU General Public License for more details.                      */
/*                                                                           */
/* You should have received a copy of the GNU General Public License along   */
/* with this program.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                           */
/*****************************************************************************/

#ifndef INI_H
#define INI_H

#define SUCCESS                      0x0000

#define ERROR_INI_CALLOC_LIST        0x0001
#define ERROR_INI_ALLOC_ENTRY        0x0002
#define ERROR_INI_CALLOC_NODE        0x0003
#define ERROR_INI_OPEN_WRITE_FILE    0x0004
#define ERROR_INI_READ_FILE_STAT     0x0005
#define ERROR_INI_READ_FILE_MAX_SIZE 0x0006
#define ERROR_INI_READ_FILE_OPEN     0x0007
#define ERROR_INI_READ_FILE_ALLOC    0x0008
#define ERROR_INI_READ_FILE_READ     0x0009
#define ERROR_INI_READ_FILE_SIZE     0x000A

typedef struct ini s_ini;

s_ini* alloc_ini(void);
void free_ini(s_ini* ini);
int set_ini_string(s_ini* ini, char* section, char* key, char* value, char* comment);
char* get_ini_string(s_ini* ini, char* section, char* key);
char* get_ini_comment(s_ini* ini, char* section, char* key);
int rm_ini_section(s_ini* ini, char* section);
int rm_ini_key(s_ini* ini, char* section, char* key);
int write_ini_file(s_ini* ini, char* file_name);
int read_ini_file(s_ini* ini, char* file_name);

#endif

/*****************************************************************************/
