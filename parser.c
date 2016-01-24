#include <stdio.h>
#include <stdlib.h>
#include "trace_analizer.h"


void find_last_blk_in_workload(FILE *input_file, struct last_blk_info *last_blk_info)
{
	char line[201];

	double dump0;
	unsigned int dump1, dump2;
	unsigned long long last_blkno = 0;	
	unsigned int last_blkno_bcnt = 0;

	while ( fgets(line, 200, input_file ) != NULL) {

		if ( sscanf(line, "%lf %u %llu %u %x\n", &dump0, &dump1, &last_blk_info->blkno, &last_blk_info->bcount, &dump2) != 5 ) {
			fprintf(stderr, "Wrong number of arguments for I/O trace event type\n");
			fprintf(stderr, "line : %s", line);
		}
		
		if ( last_blk_info->blkno > last_blkno ) {
			last_blkno = last_blk_info->blkno;
			last_blkno_bcnt = last_blk_info->bcount;
//			printf("[find_last_blk_in_workload] | last_blkno : %llu | last_blk_info->blkno : %llu\n", last_blkno, last_blk_info->blkno);
		}

	}

	last_blk_info->blkno = last_blkno;
	last_blk_info->bcount = last_blkno_bcnt;

	printf("\n");
//	printf("[workload_info] | last block number : %llu\n", last_blkno + last_blkno_bcnt);

//	printf("[find_last_blk_in_workload] | fgets returned NULL\n");
	return;
}

long double find_scale_ratio(struct dev_set_c *dev_set_c, struct last_blk_info *last_blk_info) 
{
	struct dev_set_c *sc = dev_set_c;
	struct last_blk_info *lbi = last_blk_info;
	unsigned long long my_dev_total_blkno = sc->avail_blk_num;
	unsigned long long workload_total_blkno = lbi->blkno + lbi->bcount;
	
	printf("[total block number] | workload : %llu | disks : %llu\n", workload_total_blkno, my_dev_total_blkno);

	long double ratio = workload_total_blkno /(long double)my_dev_total_blkno;
	
	printf("             [ratio] | %LF\n", ratio);

	return ratio;
}

/*
void parser_for_one_line(FILE *input_file, struct io_info *one_req_info)
{
	char line[201];

	if ( fgets(line, 200, input_file ) == NULL) {
		printf("fgets returned NULL\n");
		return;
	}

	if (sscanf(line, "%lf %u %llu %u %x\n", &one_req_info->time, &one_req_info->devno, &one_req_info->blkno, &one_req_info->bcount, &one_req_info->is_read) != 5) {
		fprintf(stderr, "Wrong number of arguments for I/O trace event type\n");
		fprintf(stderr, "line : %s", line);
	}
}
*/

