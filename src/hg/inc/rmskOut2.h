/* rmskOut2.h was originally generated by the autoSql program, which also 
 * generated rmskOut2.c and rmskOut2.sql.  This header links the database and
 * the RAM representation of objects. */

#ifndef RMSKOUT2_H
#define RMSKOUT2_H

#ifndef LINEFILE_H
#include "linefile.h"
#endif

#define RMSKOUT2_NUM_COLS 16

struct rmskOut2
/* RepeatMasker .out record */
    {
    struct rmskOut2 *next;  /* Next in singly linked list. */
    unsigned swScore;	/* Smith Waterman alignment score */
    unsigned milliDiv;	/* Base mismatches in parts per thousand */
    unsigned milliDel;	/* Bases deleted in parts per thousand */
    unsigned milliIns;	/* Bases inserted in parts per thousand */
    char *genoName;	/* Genomic sequence name */
    unsigned genoStart;	/* Start in genomic sequence */
    unsigned genoEnd;	/* End in genomic sequence */
    int genoLeft;	/* -#bases after match in genomic sequence */
    char strand[2];	/* Relative orientation + or - */
    char *repName;	/* Name of repeat */
    char *repClass;	/* Class of repeat */
    char *repFamily;	/* Family of repeat */
    int repStart;	/* Start in repeat sequence - regardless of orient. */
    unsigned repEnd;	/* End in repeat sequence - reglardless of orient. */
    int repLeft;	/* -#bases after match in repeat sequence - regardless of orient */
    unsigned id;	/* The ID of the hit. Used to link related fragments */
    };

void rmskOut2StaticLoad(char **row, struct rmskOut2 *ret);
/* Load a row from rmskOut2 table into ret.  The contents of ret will
 * be replaced at the next call to this function. */

struct rmskOut2 *rmskOut2Load(char **row);
/* Load a rmskOut2 from row fetched with select * from rmskOut2
 * from database.  Dispose of this with rmskOut2Free(). */

struct rmskOut2 *rmskOut2LoadAll(char *fileName);
/* Load all rmskOut2 from whitespace-separated file.
 * Dispose of this with rmskOut2FreeList(). */

struct rmskOut2 *rmskOut2LoadAllByChar(char *fileName, char chopper);
/* Load all rmskOut2 from chopper separated file.
 * Dispose of this with rmskOut2FreeList(). */

#define rmskOut2LoadAllByTab(a) rmskOut2LoadAllByChar(a, '\t');
/* Load all rmskOut2 from tab separated file.
 * Dispose of this with rmskOut2FreeList(). */

struct rmskOut2 *rmskOut2CommaIn(char **pS, struct rmskOut2 *ret);
/* Create a rmskOut2 out of a comma separated string. 
 * This will fill in ret if non-null, otherwise will
 * return a new rmskOut2 */

void rmskOut2Free(struct rmskOut2 **pEl);
/* Free a single dynamically allocated rmskOut2 such as created
 * with rmskOut2Load(). */

void rmskOut2FreeList(struct rmskOut2 **pList);
/* Free a list of dynamically allocated rmskOut2's */

void rmskOut2Output(struct rmskOut2 *el, FILE *f, char sep, char lastSep);
/* Print out rmskOut2.  Separate fields with sep. Follow last field with lastSep. */

#define rmskOut2TabOut(el,f) rmskOut2Output(el,f,'\t','\n');
/* Print out rmskOut2 as a line in a tab-separated file. */

#define rmskOut2CommaOut(el,f) rmskOut2Output(el,f,',',',');
/* Print out rmskOut2 as a comma separated list including final comma. */

/* -------------------------------- End autoSql Generated Code -------------------------------- */

struct rmskOut2 *rmskOut2Read(char *fileName);
/* Read all records in .out file and return as list. */

void rmskOut2OpenVerify(char *fileName, struct lineFile **retFile, boolean *retEmpty);
/* Open repeat masker .out file and verify that it is good.
 * Set retEmpty if it has header characteristic of an empty file. */

struct rmskOut2 *rmskOut2ReadNext(struct lineFile *lf);
/* Read next record from repeat masker file.  Return NULL at EOF. */

void rmskOut2WriteHead(FILE *f);
/* Write out rmsk header lines. */

void rmskOut2WriteOneOut(struct rmskOut2 *rmsk, FILE *f);
/* Write one rmsk in .out format to file. */

void rmskOut2WriteAllOut(char *fileName, struct rmskOut2 *rmskList);
/* Write .out format file containing all in rmskList. */

struct binKeeper *readRepeats2(char *chrom, char *rmskFileName, struct hash *tSizeHash);
/* read all repeats for a chromosome of size size, returns results in binKeeper structure for fast query*/

struct hash *readRepeatsAll2(char *sizeFileName, char *rmskDir);
/* read all repeats for a all chromosomes getting sizes from sizeFileNmae , returns results in hash of binKeeper structure for fast query*/

#endif /* RMSKOUT2_H */

