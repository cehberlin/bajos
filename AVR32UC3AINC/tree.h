/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/* Copyright (c) 2007, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __TREE_H
#define __TREE_H

#define TREE_ADD_ENTRY_FAIL	       0
#define TREE_ADD_ENTRY_OK	       1
#define TREE_ADD_ENTRY_ERASE	   2
#define TREE_CARACTER_NOT_IN_RANGE 3
#define TREE_OVERFLOW              4

#define TREE_SRCH_NB_CAR		0x7F
#define TREE_SRCH_END_STR	    0x0001
#define TREE_SRCH_MAX_LEVEL     32
#define TREE_SRCH_END           ((char *) -1)

typedef void (*_fct1_ptr) (void *);

/* arbre de recherche de chaine de caractère
 *	X - X - X
 *     |   |- X
 *     |   |- X
 *     |
 *     |- X - X
 *         |- X
 *
 */
typedef struct
{
	void *_ep;
	void *__next_e_tree[TREE_SRCH_NB_CAR];
}_e_tree_srch;

_e_tree_srch *tree_srch_create();
void tree_srch_free(_e_tree_srch *_s_e_tree);
void tree_srch_free_all(_e_tree_srch *_s_e_tree, _fct1_ptr _free_fct);
int tree_srch_add_entry(_e_tree_srch *_s_e_tree, char *_str, void *_ep);
void tree_srch_fprintf(void *_file, _e_tree_srch *_s_e_tree);
void *tree_srch_is_str(_e_tree_srch *_s_e_tree, char *_str);
void tree_srch_del(_e_tree_srch **__s_e_tree, char *_str);
char *tree_srch_find_first_ep(_e_tree_srch *_s_e_tree, void **__ep);
char *tree_srch_find_next_ep(void **__ep);

/* arbre right - down
 *	X - X - X
 *    |   |
 *    |   X - X - X
 *    |
 *	X - X
 */
#define TREE_RD_DIR_INIT		0
#define TREE_RD_DIR_RIGHT	    1
#define TREE_RD_DIR_DOWN		2

typedef struct
{
	void *_ep;
	void *__right_tree;
	void *__down_tree;
}_e_tree_rd;

_e_tree_rd *tree_rd_create(void *_ep);
void tree_rd_free(_e_tree_rd *_s_e_tree);
void tree_rd_free_all(_e_tree_rd *_s_e_tree, _fct1_ptr _free_fct);
int tree_rd_add_entry(_e_tree_rd *_s_e_tree, unsigned int dir, void *_ep);
void tree_rd_fprintf(void *_file, _e_tree_rd *_s_e_tree);

/* liste chainée
 *	X - X - X
 */

typedef struct
{
	void *_ep;
	void *__next;
}_e_tree_lc;

_e_tree_lc *tree_lc_create(void *_ep);
void tree_lc_free(_e_tree_lc *_s_e_tree);
void tree_lc_free_all(_e_tree_lc *_s_e_tree, _fct1_ptr _free_fct);
int tree_lc_add_entry(_e_tree_lc *_s_e_tree, void *_ep);
void tree_lc_fprintf(void *_file, _e_tree_lc *_s_e_tree);

#endif
