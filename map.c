#include <stdio.h>
#include <stdlib.h>
#include "trace_analizer.h"

#define ROUNDUP(x) (((x)+0.5))

extern void find_last_blk_in_workload(FILE *, struct last_blk_info *);
extern long double find_scale_ratio(struct dev_set_c *, struct last_blk_info *);

int map(FILE *input_file, struct dev_set_c *set_info, struct table **table_array)
{
	char line[201];
	struct last_blk_info last_blk_info;
	struct dev_set_c *si = set_info;
	long double ratio;  
	unsigned long long _ratio;

	find_last_blk_in_workload(input_file, &last_blk_info);
	ratio = find_scale_ratio(set_info, &last_blk_info);

	_ratio = (unsigned long long)ratio; 
	
	if ( _ratio > MAX_RATIO ) {
		printf("\n");
		printf("[ERROR] : The total number of sectors of your devices(%llu) is too small that it's hard to scale down the given workload. I recommend you adjust the total number of sectors of your devices to ", si->avail_blk_num * SECTOR_NUM_PER_BLK);
		printf("%llu ~ %llu\n", (last_blk_info.blkno / 8) * SECTOR_NUM_PER_BLK, last_blk_info.blkno * SECTOR_NUM_PER_BLK);	
		return DEVICE_SIZE_ERROR;
	}
	printf("\n");

	{
		double dump0;
		unsigned int dump1;
		unsigned long long wl_blkno = 0;	     /* workload_block_number */
		unsigned int wl_bcount = 0; 			 /* workload_block_count */
		unsigned int wl_is_read = 3; 			 /* workload_is_read */
	
		unsigned int my_dev_num = set_info->total_dev_num;			   		/* the number of my devices */
		unsigned int my_table_idx = 0;										 /* my devive number */
		unsigned long long my_blknr = 0; 									 /* block number of a device */
		unsigned long long gen_blknr = 0; 									 /* general block number if devices are grouped by device mapper */
		unsigned int my_bcount = 0;
		unsigned int my_is_read = 3;
		int i;	

		rewind(input_file);

		while ( fgets(line, 200, input_file) != NULL ) {

			if ( sscanf(line, "%lf %u %llu %u %x\n", &dump0, &dump1, &wl_blkno, &wl_bcount, &wl_is_read) != 5 ) { 
				fprintf(stderr, "Wrong number of arguments for I/O trace event type\n");
				fprintf(stderr, "line : %s", line);
			}

			if ( dump1 != 0 ) {
				printf("[ERROR_map] : I assumed workload device num is just one, so need to change map function.\n");
				return WORKLOAD_DEVICE_NUM_ERROR;
			}

			gen_blknr = (unsigned long long)ROUNDUP(wl_blkno / ratio); 
			my_bcount = (unsigned int)ROUNDUP(wl_bcount / ratio);
			my_is_read = wl_is_read;

	//		printf("[map] | ratio : %Lf | wl_blkno : %llu | wl_bcount : %u | wl_is_read : %u | gen_blknr : %llu\n", ratio, wl_blkno, wl_bcount, wl_is_read, gen_blknr);

			for ( i = 0; i < my_dev_num; i++ ) {
			
				if ( gen_blknr <= table_array[i]->blk_num ) {
					my_table_idx = i;
					my_blknr = gen_blknr;
					break;
				}

				gen_blknr -= table_array[i]->blk_num;
			}

	//		printf("[map] | my_table_idx : %u | my_blknr : %llu | my_bcount : %u | my_is_read : %u\n", my_table_idx, my_blknr, my_bcount, my_is_read);

			
			if ( my_is_read == YES ) { /*READ*/
				for ( i = 0; i < my_bcount; i++ ) {
		//			printf("[map] | [READ] | i : %d | my_bcount : %u\n", i, my_bcount);
				 	table_array[my_table_idx]->blk_nr[my_blknr+i].num_read++;
					si->dev_info[my_table_idx].read_num++;
					si->total_read_num++;
				}
			} else if ( my_is_read == NO ) { /*WRITE*/
				for ( i = 0; i < my_bcount; i++ ) {

					if ( table_array[my_table_idx]->blk_nr[my_blknr+i].num_write == 0 ) {	
		//					printf("[map] | [WRITE] | valid_blk_num++");
							si->total_valid_blk_num++;
							si->dev_info[my_table_idx].valid_blk_num++;
					}

		//			printf("[map] | [WRITE] | i : %d | my_bcount : %u\n", i, my_bcount);
					table_array[my_table_idx]->blk_nr[my_blknr+i].num_write++;
					table_array[my_table_idx]->blk_nr[my_blknr+i].is_valid = YES;
					si->dev_info[my_table_idx].write_num++;
					si->total_write_num++;
				}
			} else {
				printf("[ERROR_map] : unknown error\n");
				return MAP_UNKNOWN_ERROR;
			}

		}

//		printf("[map] | EOF\n");

	}	

	return SUCCESS;
}



