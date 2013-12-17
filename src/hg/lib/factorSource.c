/* factorSource.c was originally generated by the autoSql program, which also 
 * generated factorSource.h and factorSource.sql.  This module links the database and
 * the RAM representation of objects. */

#include "common.h"
#include "linefile.h"
#include "dystring.h"
#include "jksql.h"
#include "basicBed.h"
#include "factorSource.h"


char *factorSourceCommaSepFieldNames = "chrom,chromStart,chromEnd,name,score,expCount,expNums,expScores";

struct factorSource *factorSourceLoad(char **row);

struct factorSource *factorSourceLoadAll(char *fileName) 
/* Load all factorSource from a whitespace-separated file.
 * Dispose of this with factorSourceFreeList(). */
{
struct factorSource *list = NULL, *el;
struct lineFile *lf = lineFileOpen(fileName, TRUE);
char *row[8];

while (lineFileRow(lf, row))
    {
    el = factorSourceLoad(row);
    slAddHead(&list, el);
    }
lineFileClose(&lf);
slReverse(&list);
return list;
}

struct factorSource *factorSourceLoadAllByChar(char *fileName, char chopper) 
/* Load all factorSource from a chopper separated file.
 * Dispose of this with factorSourceFreeList(). */
{
struct factorSource *list = NULL, *el;
struct lineFile *lf = lineFileOpen(fileName, TRUE);
char *row[8];

while (lineFileNextCharRow(lf, chopper, row, ArraySize(row)))
    {
    el = factorSourceLoad(row);
    slAddHead(&list, el);
    }
lineFileClose(&lf);
slReverse(&list);
return list;
}

void factorSourceFree(struct factorSource **pEl)
/* Free a single dynamically allocated factorSource such as created
 * with factorSourceLoad(). */
{
struct factorSource *el;

if ((el = *pEl) == NULL) return;
freeMem(el->chrom);
freeMem(el->name);
freeMem(el->expNums);
freeMem(el->expScores);
freez(pEl);
}

void factorSourceFreeList(struct factorSource **pList)
/* Free a list of dynamically allocated factorSource's */
{
struct factorSource *el, *next;

for (el = *pList; el != NULL; el = next)
    {
    next = el->next;
    factorSourceFree(&el);
    }
*pList = NULL;
}

void factorSourceOutput(struct factorSource *el, FILE *f, char sep, char lastSep) 
/* Print out factorSource.  Separate fields with sep. Follow last field with lastSep. */
{
if (sep == ',') fputc('"',f);
fprintf(f, "%s", el->chrom);
if (sep == ',') fputc('"',f);
fputc(sep,f);
fprintf(f, "%u", el->chromStart);
fputc(sep,f);
fprintf(f, "%u", el->chromEnd);
fputc(sep,f);
if (sep == ',') fputc('"',f);
fprintf(f, "%s", el->name);
if (sep == ',') fputc('"',f);
fputc(sep,f);
fprintf(f, "%u", el->score);
fputc(sep,f);
fprintf(f, "%u", el->expCount);
fputc(sep,f);
{
int i;
if (sep == ',') fputc('{',f);
for (i=0; i<el->expCount; ++i)
    {
    fprintf(f, "%u", el->expNums[i]);
    fputc(',', f);
    }
if (sep == ',') fputc('}',f);
}
fputc(sep,f);
{
int i;
if (sep == ',') fputc('{',f);
for (i=0; i<el->expCount; ++i)
    {
    fprintf(f, "%g", el->expScores[i]);
    fputc(',', f);
    }
if (sep == ',') fputc('}',f);
}
fputc(lastSep,f);
}

/* -------------------------------- End autoSql Generated Code -------------------------------- */

// NOTE:  Removed factorSourceCommaIn() from autoSql gened code, as it is unneeded and 
// would need customizing for the two schemas

// NOTE: make loader able to handle V2 (BED15) and V3 (factorSource) schemas.  

boolean isStrandField(char *field)
/* Check for '.' strand field */
{
if (field[0] == '.')
    return TRUE;
return FALSE;
}

struct factorSource *factorSourceLoad(char **row)
/* Load a factorSource from row fetched with select * from factorSource
 * from database.  Dispose of this with factorSourceFree(). */
{
struct factorSource *ret;
int sizeOne; // autoSql

AllocVar(ret);
ret->chrom = cloneString(row[0]);
ret->chromStart = sqlUnsigned(row[1]);
ret->chromEnd = sqlUnsigned(row[2]);
ret->name = cloneString(row[3]);
ret->score = sqlUnsigned(row[4]);
if (isStrandField(row[5]))
    {
    // V2 format (BED15) has sparse info -- filter out zero-scored experiments
    struct bed *bed;
    AllocVar(bed);
    ret->expCount = sqlUnsigned(row[12]);
    sqlSignedDynamicArray(row[13], &bed->expIds, &sizeOne);
    assert(sizeOne == ret->expCount);
    sqlFloatDynamicArray(row[14], &bed->expScores, &sizeOne);
    assert(sizeOne == ret->expCount);

    // count non-zero experiments
    ret->expCount = 0;
    int i;
    for (i=0; i<sizeOne; i++)
        {
        float score = bed->expScores[i];
        if (score > 0.0)
            ret->expCount++;
        }
    // populate arrays with non-zero experiments
    AllocArray(ret->expNums, ret->expCount);
    AllocArray(ret->expScores, ret->expCount);
    int j = 0;
    for (i=0; i<sizeOne; i++)
        {
        float score = bed->expScores[i];
        if (score > 0.0)
            {
            ret->expNums[j] = bed->expIds[i];
            ret->expScores[j] = bed->expScores[i];
            j++;
            }
        }
    }
else
    {
    // V3 format
    ret->expCount = sqlUnsigned(row[5]);
    sqlUnsignedDynamicArray(row[6], &ret->expNums, &sizeOne);
    assert(sizeOne == ret->expCount);
    sqlFloatDynamicArray(row[7], &ret->expScores, &sizeOne);
    assert(sizeOne == ret->expCount);
}
return ret;
}
