#include <stdio.h>
#include <stdlib.h>
#include "trace_analizer.h"


struct table *alloc_table(struct dev_set_c *set_info, unsigned int dev_nr)
{
	struct dev_set_c *si = set_info;
	size_t len;
	unsigned long long avail_blk_num = AVAILABLE_BLK_NUM(si->dev_info[dev_nr].physical_start_blk, si->dev_info[dev_nr].total_blk);

	//printf("[alloc_table] | si->dev_info[%u].physical_start_blk : %llu | asi->dev_info[%u].total_blk : %llu | vail_blk_num : %u\n"
//			, dev_nr, si->dev_info[dev_nr].physical_start_blk, dev_nr, si->dev_info[dev_nr].total_blk, avail_blk_num);
	
	len = sizeof(struct table) + sizeof(struct blk_record)*avail_blk_num;

	return malloc(len);
}

void alloc_table_array(struct dev_set_c *set_info, struct table ***table_array)
{
	struct dev_set_c *si = set_info;

	unsigned int total_dev_num = si->total_dev_num;
	unsigned long long avail_blk_num;
	unsigned int i;

	//printf("[alloc_table_array] | total_dev_num : %u\n", total_dev_num);

	*table_array = (struct table **)malloc(sizeof(struct table*)*total_dev_num);

	for ( i = 0; i < total_dev_num; i++ ) {
		(*table_array)[i] = alloc_table(si, i);
		(*table_array)[i]->blk_num = AVAILABLE_BLK_NUM(si->dev_info[i].physical_start_blk, si->dev_info[i].total_blk);
	}
}


void init_table(struct dev_set_c *set_info, struct table **table_array) 
{
	struct dev_set_c *si = set_info;
	unsigned int dev_num = si->total_dev_num;
	unsigned long long blk_num;
	unsigned int i, j;

	for ( i = 0; i < dev_num; i++ ) {
		blk_num = table_array[i]->blk_num;
		for ( j = 0; j < blk_num; j++ ) {
			table_array[i]->blk_nr[j].num_read = 0; 
			table_array[i]->blk_nr[j].num_write = 0; 
			table_array[i]->blk_nr[j].is_valid = 0; 
		}
	}

}

void print_table(struct dev_set_c *set_info, struct table **table_array) 
{
	struct dev_set_c *si = set_info;
	unsigned int dev_num = si->total_dev_num;
	unsigned long long blk_num;
	unsigned int i, j;

	printf("\n");
	printf("[*detail*]\n");
	printf("[Disk#][Block#] | number of read | number of write | YES : 1, NO : 0\n\n");

	for ( i = 0; i < dev_num; i++ ) {
		blk_num = table_array[i]->blk_num;
		for ( j = 0; j < blk_num; j++ ) {
			printf("[%u][%u] | #read : %u | #write : %u | is_valid : %u \n", i, j, table_array[i]->blk_nr[j].num_read, table_array[i]->blk_nr[j].num_write, table_array[i]->blk_nr[j].is_valid); 
		}
	}
	printf("[end]\n");
}


void dtr_table(int i){}

