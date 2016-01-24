#include <stdio.h>
#include <stdlib.h>
#include "trace_analizer.h"

int main(int argv, char **argc)
{
	FILE *input_file;
	char *filename = argc[1];
	struct dev_set_c *set_info;
 	struct table **table_array; /* table_bdev[bdev_nr]*/
	error_t error_msg;

	if ( !(input_file = fopen(filename, "r")) ) {
			printf("No such file %s\n", filename);
			return -1;
	}

	get_devs_info(argv, argc, &set_info);

	alloc_table_array(set_info, &table_array);

	init_table(set_info, table_array);
	
	if ( map(input_file, set_info, table_array) < 0) {
		printf("[ERROR] : can not do mapping workload to  your device\n");	
		return -1;
	}

	print_devs_info(set_info);

	print_table(set_info, table_array);

	
	//ratio = find_scale_ratio(set_info, &last_blk_info);

	//printf("%lf %u %llu %u %x\n", one_req_info.time, one_req_info.devno, one_req_info.blkno, one_req_info.bcount, one_req_info.is_read);

	return 0;
}


