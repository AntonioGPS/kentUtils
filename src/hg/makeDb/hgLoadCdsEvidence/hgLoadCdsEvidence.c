/* hgLoadCdsEvidence - Load table with cdsEvidence.. */
#include "common.h"
#include "linefile.h"
#include "hash.h"
#include "options.h"
#include "jksql.h"
#include "dystring.h"

boolean oldTable = FALSE;	/* Don't redo table. */

void usage()
/* Explain usage and exit. */
{
errAbort(
  "hgLoadCdsEvidence - Load table with cdsEvidence from file.\n"
  "usage:\n"
  "   hgLoadCdsEvidence database table file\n"
  "options:\n"
  "   -oldTable add to existing table\n"
  );
}

static struct optionSpec options[] = {
   {"oldTable", OPTION_BOOLEAN},
   {NULL, 0},
};

char *createSql = 
"# cdsEvidence.sql was originally generated by the autoSql program, which also \n"
"# generated cdsEvidence.c and cdsEvidence.h.  This creates the database representation of\n"
"# an object which can be loaded and saved from RAM in a fairly \n"
"# automatic way.\n"
"\n"
"#Evidence for CDS boundary within a transcript\n"
"CREATE TABLE %s (\n"
"    name varchar(255) not null,	# Name of transcript\n"
"    start int not null,	# CDS start within transcript, zero based\n"
"    end int not null,	# CDS end, non-inclusive\n"
"    source varchar(255) not null,	# Source of evidence\n"
"    accession varchar(255) not null,	# Genbank/uniProt accession\n"
"    score double not null,	# 0-1000, higher is better\n"
"    startComplete tinyint unsigned not null,	# Starts with ATG?\n"
"    endComplete tinyint unsigned not null,	# Ends with stop codon?\n"
"    cdsCount int not null,	# Number of CDS blocks\n"
"    cdsStarts longblob not null,	# Start positions of CDS blocks\n"
"    cdsSizes longblob not null,	# Sizes of CDS blocks\n"
"              #Indices\n"
"    INDEX(name(32))\n"
");\n" ;

void hgLoadCdsEvidence(char *database, char *table, char *inFile)
/* hgLoadCdsEvidence - Load table with cdsEvidence.. */
{
struct dyString *dy = dyStringNew(4*1024);
struct sqlConnection *conn = sqlConnect(database);
if (!oldTable)
    {
    sqlDyStringPrintf(dy, createSql, table);
    sqlRemakeTable(conn, table, dy->string);
    dyStringClear(dy);
    }
sqlLoadTabFile(conn, inFile, table, 0);
sqlDisconnect(&conn);
}

int main(int argc, char *argv[])
/* Process command line. */
{
optionInit(&argc, argv, options);
oldTable = optionExists("oldTable");
if (argc != 4)
    usage();
hgLoadCdsEvidence(argv[1], argv[2], argv[3]);
return 0;
}
