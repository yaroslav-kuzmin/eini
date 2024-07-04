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

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "eini.h"

/*****************************************************************************/
#define SECTION_EQUAL 0
#define KEY_EQUAL     0
#define QUOTES_OPEN   1
#define QUOTES_CLOSE  0

static char *strdup(const char *src)
{
	char *dest;
  
	if (src == NULL) {
		return NULL;
	}
	
	dest = (char *)malloc(strlen(src) + 1);
 
	if (dest != NULL) {
		strcpy(dest, src);
	}
	 
	return dest;
}
/*****************************************************************************/
struct entry
{
	char* section;
	char* key;
	char* value;
	char* comment;
};
typedef struct entry s_entry;

static s_entry* alloc_entry(char* section, char* key, char* value, char* comment)
{
	s_entry* entry;

	if ((key == NULL) || (value == NULL))
		return NULL;

	entry = calloc(1, sizeof(s_entry));
	if (entry == NULL)
		return entry;

	if (section != NULL)
		entry->section = strdup(section);

	entry->key = strdup(key);
	entry->value = strdup(value);

	if (comment != NULL)
		entry->comment = strdup(comment);

	return entry;
}

static void free_entry(s_entry* entry)
{
	if (entry != NULL) {
		if (entry->section != NULL)
			free(entry->section);
		free(entry->key);
		free(entry->value);
		if (entry->comment != NULL)
			free(entry->comment);
		free(entry);
	}
}

static int compare_entry_section(s_entry* entry_0, s_entry* entry_1)
{
	if ((entry_0->section == NULL) && (entry_1->section != NULL))
		return 1;
	if ((entry_0->section != NULL) && (entry_1->section == NULL))
		return 1;

	return strcmp(entry_0->section, entry_1->section);
}

struct node
{
	s_entry* entry;
	struct node* next;
};
typedef struct node s_node;

struct ini
{
	s_node* list;
};
typedef struct ini s_ini;

s_ini* alloc_ini(void)
{
	s_ini* ini = NULL;

	ini = malloc(sizeof(s_ini));
	if (ini == NULL)
		return ini;

	ini->list = NULL;
	return ini;
}

void free_ini(s_ini* ini)
{
	if (ini != NULL) {
		s_node* curr = ini->list;
		for (;;) {
			if (curr == NULL) {
				break;
			}
			else {
				s_node* next = curr->next;
				free_entry(curr->entry);
				free(curr);
				curr = next;
			}
		}
		free(ini);
	}
}

int set_ini_string(s_ini* ini, char* section, char* key, char* value, char* comment)
{
	s_node* new;
	s_entry* entry;

	assert(ini);

	entry = alloc_entry(section, key, value, comment);
	if (entry == NULL) {
		return ERROR_INI_ALLOC_ENTRY;
	}
	new = calloc(1, sizeof(s_node));
	if (new == NULL) {
		free_entry(entry);
		return ERROR_INI_CALLOC_NODE;
	}

	new->entry = entry;

	if (ini->list == NULL) {
		ini->list = new;
	}
	else {
		if (new->entry->section == NULL) {
			new->next = ini->list;
			ini->list = new;
		}
		else {
			s_node* prev = NULL;
			s_node* curr = ini->list;

			for (;;) {
				if (curr->next == NULL) {
					curr->next = new;
					break;
				}

				if (compare_entry_section(new->entry, curr->entry) == SECTION_EQUAL) {
					new->next = curr;
					if (prev == NULL) {
						ini->list = new;
					}
					else {
						prev->next = new;
					}
					break;
				}
				prev = curr;
				curr = curr->next;
			}
		}
	}

	return SUCCESS;
}

static char* get_entry_string(s_entry* entry, char* section, char* key)
{
	char* str = NULL;
	s_entry entry_temp;

	assert(entry);
	entry_temp.section = section;

	if (compare_entry_section(entry, &entry_temp) == SECTION_EQUAL) {
		if (strcmp(entry->key, key) == KEY_EQUAL) {
			str = entry->value;
		}
	}

	return str;
}

static char* get_entry_comment(s_entry* entry, char* section, char* key)
{
	char* str = NULL;
	s_entry entry_temp;

	assert(entry);

	entry_temp.section = section;

	if (compare_entry_section(entry, &entry_temp) == SECTION_EQUAL) {
		if (strcmp(entry->key, key) == KEY_EQUAL) {
			str = entry->comment;
		}
	}

	return str;
}

char* get_ini_string(s_ini* ini, char* section, char* key)
{
	s_node* curr;
	char* str = NULL;

	assert(ini);
	curr = ini->list;

	for (;;) {
		if (curr == NULL)
			break;
		str = get_entry_string(curr->entry, section, key);
		if (str != NULL)
			break;
		curr = curr->next;
	}

	return str;
}

char* get_ini_comment(s_ini* ini, char* section, char* key)
{
	s_node* curr;
	char* str = NULL;

	assert(ini);
	curr = ini->list;

	for (;;) {
		if (curr == NULL)
			break;
		str = get_entry_comment(curr->entry, section, key);
		if (str != NULL)
			break;
		curr = curr->next;
	}

	return str;
}

int rm_ini_section(s_ini* ini, char* section)
{
	s_node* prev;
	s_node* curr;

	assert(ini);

	if (section == NULL)
		return SUCCESS;

	prev = NULL;
	curr = ini->list;

	for (;;) {
		s_entry* entry;
		if (curr == NULL)
			break;
		entry = curr->entry;
		char* curr_section = entry->section;
		if (curr_section == NULL) {
			prev = curr;
			curr = curr->next;
			continue;
		}
		if (strcmp(curr_section, section) == SECTION_EQUAL) {
			if (prev == NULL)
				ini->list = curr->next;
			else
				prev->next = curr->next;

			free_entry(curr->entry);
			free(curr);
			curr = prev->next;
			continue;
		}
		prev = curr;
		curr = curr->next;
	}

	return SUCCESS;
}

int rm_ini_key(s_ini* ini, char* section, char* key)
{
	s_node* prev;
	s_node* curr;

	assert(ini);

	if (key == NULL)
		return SUCCESS;

	prev = NULL;
	curr = ini->list;

	for (;;) {
		s_entry* entry;
		if (curr == NULL)
			break;

		entry = curr->entry;
		char* curr_section = entry->section;
		if (curr_section == NULL) {
			if (strcmp(entry->key, key) == KEY_EQUAL) {
				if (prev == NULL)
					ini->list = curr->next;
				else
					prev->next = curr->next;

				free_entry(curr->entry);
				free(curr);
				curr = prev->next;
				continue;
			}
		}
		else {
			if (strcmp(curr_section, section) == SECTION_EQUAL) {
				if (strcmp(entry->key, key) == KEY_EQUAL) {
					if (prev == NULL)
						ini->list = curr->next;
					else
						prev->next = curr->next;

					free_entry(curr->entry);
					free(curr);
					curr = prev->next;
					continue;
				}
			}
		}

		prev = curr;
		curr = curr->next;
	}

	return SUCCESS;
}

static int check_space(char* str)
{
	int space = 0;
	for (;;) {
		if (*str == 0)
			break;
		if ((*str == ' ') || (*str == '\t')) {
			space = (int)*str;
			break;
		}
		++str;
	}
	return space;
}

int write_ini_file(s_ini* ini, char* file_name)
{
	int fd;
	s_node* curr;
	char* section = NULL;
	int space = 0;
	size_t size;

	assert(ini);
	assert(file_name);

	fd = open(file_name, O_CREAT|O_WRONLY);
	if (fd == -1) {
		return ERROR_INI_OPEN_WRITE_FILE;
	}

	/*TODO проверить запись в файл*/
	write(fd, "\n", 1);
	curr = ini->list;

	for (;;) {
		if (curr == NULL)
			break;
		s_entry* entry = curr->entry;
		if (entry != NULL) {
			if (entry->section != NULL) {
				if (section == NULL) {
					section = entry->section;
					write(fd, "\n[", 2);
					size = strlen(section);	
					write(fd, section, size);
					write(fd, "]\n", 2);
				}
				else {
					if (strcmp(section, entry->section) != SECTION_EQUAL) {
						section = entry->section;
						write(fd, "\n[", 2);
						size = strlen(section);	
						write(fd, section, size);
						write(fd, "]\n", 2);
					}
				}
			}

			if (entry->comment != NULL) {
				write(fd, "#", 1);
				size = strlen(entry->comment);	
				write(fd, entry->comment, size);
				write(fd, "\n", 1);
			}
			space = check_space(entry->value);

			size = strlen(entry->key);	
			write(fd, entry->key, size);
			write(fd, "=", 1);

			if (space)
				write(fd, "\"", 1);

			size = strlen(entry->value);	
			write(fd, entry->value, size);

			if (space)
				write(fd, "\"", 1);

			write(fd, "\n", 1);
		}

		curr = curr->next;
	}

	close(fd);
	return SUCCESS;
}

static int parser_ini_file(s_ini* ini, char* buff, size_t size)
{
	char* begin_str = buff;
	char* curr;
	char* comment;
	char* section;
	char* key;
	char* value;
	int quotes;
	int rc = SUCCESS;

	curr = begin_str;
	comment = NULL;
	section = NULL;
	key = NULL;
	value = NULL;
	quotes = QUOTES_CLOSE;

	for (; size > 0;) {
		if (*begin_str == 0)
			break;

		if ((*begin_str == ' ') || (*begin_str == '\t') || (*begin_str == '\n')) {
			++begin_str;
			curr = begin_str;
			--size;
			continue;
		}

		/*TODO добавить многострочный комент*/
		if (*begin_str == '#') {
			comment = begin_str;
			++comment;
			for (;;) {
				if (*curr == 0)
					break;
				if (*curr == '\n') {
					*curr = 0;
					++curr;
					begin_str = curr;
					--size;
					break;
				}
				++curr;
				--size;
			}
			continue;
		}

		if (*begin_str == '[') {
			section = begin_str;
			++section;
			for (;;) {
				if (*curr == 0)
					break;
				if (*curr == ']') {
					*curr = 0;
					++curr;
					--size;
					continue;
				}
				if (*curr == '\n') {
					++curr;
					begin_str = curr;
					--size;
					break;
				}
				++curr;
				--size;
			}
			continue;
		}

		key = begin_str;

		for (;;) {
			if (*curr == 0)
				break;
			if (*curr == '=') {
				*curr = 0;
				++curr;
				--size;
				break;
			}
			if ((*curr == ' ') || (*curr == '\t')) {
				*curr = 0;
				++curr;
				--size;
				continue;
			}
			if (*curr == '\n') {
				++curr;
				begin_str = curr;
				--size;
				key = NULL;
				break;
			}
			++curr;
			--size;
		}
		if (key == NULL)
			continue;

		for (;;) {
			if (*curr == 0)
				break;
			if ((*curr != ' ') && (*curr != '\t'))
				break;
			++curr;
			--size;
		}

		value = curr;

		if ((*value == '\'') || (*value == '"')) {
			++value;
			++curr;
			--size;
			quotes = QUOTES_OPEN;
		}

		for (;;) {
			if (*curr == 0)
				break;
			if ((*curr == ' ') || (*curr == '\t')) {
				if (quotes == QUOTES_CLOSE) {
					*curr = 0;
					++curr;
					--size;
					continue;
				}
			}

			if ((*curr == '\'') || (*curr == '"')) {
				if (quotes == QUOTES_OPEN) {
					*curr = 0;
					++curr;
					--size;
					quotes = QUOTES_CLOSE;
					continue;
				}
			}

			if (*curr == '\n') {
				*curr = 0;
				++curr;
				begin_str = curr;
				--size;
				break;
			}
			++curr;
			--size;
		}
		if (*value == 0) {
			key = NULL;
			value = NULL;
			continue;
		}

		rc = set_ini_string(ini, section, key, value, comment);
		if (rc != SUCCESS) {
			break;
		}

		comment = NULL;
		key = NULL;
		value = NULL;
	}

	return rc;
}

#define MAX_SIZE_INI_FILE 0x00030000 /* 196 Kb*/

int read_ini_file(s_ini* ini, char* file_name)
{
	int rc;
	struct stat buf_stat;
	char* buff = NULL;
	size_t size = 0;
	int fd = 0;

	assert(ini);
	assert(file_name);
	
	rc = stat(file_name, &buf_stat);
	if (rc != 0) {
		return ERROR_INI_READ_FILE_STAT;
	}
	/*TODO проверить доступ*/
	size = buf_stat.st_size;
	if (size > MAX_SIZE_INI_FILE) {
		return ERROR_INI_READ_FILE_MAX_SIZE;
	}

	buff = malloc(size + 1);
	if (buff == NULL) {
		return ERROR_INI_READ_FILE_ALLOC;
	}

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		return ERROR_INI_READ_FILE_OPEN;
	}

	rc = read(fd, buff, size);
	if (rc == -1) {
		close(fd);
		return ERROR_INI_READ_FILE_READ;
	}
	if (rc != size) {
		close(fd);
		return ERROR_INI_READ_FILE_SIZE;
	}
	close(fd);

	buff[size] = 0;
	rc = parser_ini_file(ini, buff, size);

	free(buff);
	return rc;
}

/*****************************************************************************/
