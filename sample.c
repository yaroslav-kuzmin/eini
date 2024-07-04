/*****************************************************************************/
/*                                                                           */
/* sample - program demonstrating how to use the eini library                */
/*                                                                           */
/*  Copyright (C) 2024 Kuzmin Yaroslav <kuzmin.yaroslav@gmail.com>           */
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

#include <stdio.h>
#include "eini.h"

/*****************************************************************************/
#define FAILURE   -1

/*****************************************************************************/
int main(int argc,char * argv[])
{
	int rc;
	char *str;
	s_ini *ini;

 	printf("Begin testing : ini\n");

	ini = alloc_ini();
	if (ini == NULL) {
		printf("error ini alloc\n");
		return FAILURE;
	}

	rc = set_ini_string(ini, NULL, NULL, NULL, "comment000");
	if (rc != SUCCESS) {
		printf("error 0001 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", NULL, NULL, "comment00000");
	if (rc != SUCCESS) {
		printf("error 0002 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_1", NULL, "comment00000");
	if (rc != SUCCESS) {
		printf("error 0003 : %d\n", rc);
	}

	rc = set_ini_string(ini, NULL, NULL, NULL, NULL);
	if (rc != SUCCESS) {
		printf("error 0004 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_5", "value_1001", "comment key_5 ");
	if (rc != SUCCESS) {
		printf("error 0005 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_0", "value_011", "comment key_0");
	if (rc != SUCCESS) {
		printf("error 0006 : %d\n", rc);
	}

	rc = set_ini_string(ini, NULL, "key_0", "value_101", NULL);
	if (rc != SUCCESS) {
		printf("error 0007 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_8", "value_189", NULL);
	if (rc != SUCCESS) {
		printf("error 0008 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_7", "value_789", NULL);
	if (rc != SUCCESS) {
		printf("error 0009 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_8", "value_9", NULL);
	if (rc != SUCCESS) {
		printf("error 0010 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section2", "key_8", "value_967", NULL);
	if (rc != SUCCESS) {
		printf("error 0011 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_458", "value_2349", NULL);
	if (rc != SUCCESS) {
		printf("error 0012 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section5", "key_890", "value_967", NULL);
	if (rc != SUCCESS) {
		printf("error 0013 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section1", "key_890", "value_967", NULL);
	if (rc != SUCCESS) {
		printf("error 0014 : %d\n", rc);
	}

	rc = set_ini_string(ini, "section2", "key_890", "value_967", NULL);
	if (rc != SUCCESS) {
		printf("error 0015 : %d\n", rc);
	}

	write_ini_file(ini, "sample_0.ini");

	rm_ini_key(ini, "section1", "key_890");
	rm_ini_key(ini, "section1", "key_");
	rm_ini_section(ini, "section2");

	write_ini_file(ini, "sample_1.ini");
	free_ini(ini);

	ini = alloc_ini();
	rc = read_ini_file(ini, "sample_2.ini");

	str = get_ini_string(ini, "section1", "key_0");
	if (str != NULL)
		printf("section1 : key_0 : %s\n", str);
	else
		printf("section1 : key_0 : NULL\n");

	write_ini_file(ini, "sample_3.ini");
	free_ini(ini);

	printf("Done\n");
	return SUCCESS;
}

/*****************************************************************************/
