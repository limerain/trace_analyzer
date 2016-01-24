#include <stdio.h>
#include <stdlib.h>
#include "trace_analizer.h"


/* You are able to change this fuction and struct dev_info as you want */
void a_dev_info_init(unsigned int dev_idx, struct dev_info *ptr_dev_info, char **argc) 
{

	ptr_to_element_t ptr_str = argc;
	sector_t physical_start_sector;
	sector_t total_sector;

	sscanf(*ptr_str, "%u", &ptr_dev_info->dev_nr);
	ptr_str++; 
	printf("	[Disk%u] | ", dev_idx);

	sscanf(*ptr_str, "%llu", &physical_start_sector);
	ptr_dev_info->physical_start_blk = CONVERT_SECTORNUM_TO_BLKNUM(physical_start_sector);
	ptr_str++;
	printf("%llu(%llu) ", ptr_dev_info->physical_start_blk, physical_start_sector);

	sscanf(*ptr_str, "%llu", &total_sector);
	if (IS_MULTIPLES_OF_8(total_sector) == NO) { /* 1BLK = 8*SECTOR */ 
		printf("[DANGER] Need to consider again! ");
	}
	ptr_dev_info->total_blk = CONVERT_SECTORNUM_TO_BLKNUM(total_sector);
	ptr_str++;

	ptr_dev_info->avail_blk_num = ptr_dev_info->total_blk - ptr_dev_info->physical_start_blk;
	ptr_dev_info->valid_blk_num = 0;
	ptr_dev_info->read_num = 0;
	ptr_dev_info->write_num = 0;
	
	printf("%llu(%llu) ", ptr_dev_info->total_blk, total_sector);
	printf("| %llu\n", ptr_dev_info->avail_blk_num);
	
}

inline struct dev_set_c *alloc_context(unsigned int dev_num)
{
	size_t len;

	/* Actually at here, required to check if the size exceeds the max size of size_t, however, I have skipped it */

	len = sizeof(struct dev_set_c) + (sizeof(struct dev_info) * dev_num);

	return malloc(len);
}

void get_devs_info(int argv, char **argc, struct dev_set_c **set_info) 
{
	unsigned int dev_idx;
	unsigned int dev_num;

	dev_num = GET_DEV_NUM(argv);
	(*set_info) = alloc_context(dev_num);
	memset(*set_info, 0, sizeof(struct dev_set_c));

	(*set_info)->total_dev_num = dev_num;
	printf("[devs_info] | #block(#sector) #block(#sector) | #available block\n"); 
	(*set_info)->start_blk = 0; /* would be required to fix this someday if need*/

	for ( dev_idx = 0, argc += 2;  dev_idx < dev_num; dev_idx++, argc+=NUM_ELEMENT_DEV_INFO_STRUCT) {
		//printf("	[Disk%s] | ", *argc);
		a_dev_info_init(dev_idx, &(*set_info)->dev_info[dev_idx], argc);
	}

	for ( dev_idx = 0; dev_idx < dev_num; dev_idx++) {
			(*set_info)->total_blk += (*set_info)->dev_info[dev_idx].total_blk;
			(*set_info)->avail_blk_num += (*set_info)->dev_info[dev_idx].total_blk - (*set_info)->dev_info[dev_idx].physical_start_blk;
	}

	(*set_info)->total_valid_blk_num = 0;
	(*set_info)->total_read_num = 0;
	(*set_info)->total_write_num = 0;

//	for ( dev_idx = 0; dev_idx < dev_num; dev_idx++) 
	//	printf("	[debug : %u] | %u %llu %llu\n", dev_idx, (*set_info)->dev_info[dev_idx].dev_nr,  (*set_info)->dev_info[dev_idx].physical_start_blk, (*set_info)->dev_info[dev_idx].total_blk);
}

void print_devs_info(struct dev_set_c * dev_set_c) 
{
	struct dev_set_c * sc = dev_set_c;
	unsigned int total_dev_num = sc->total_dev_num;
	int i;
	
	printf("[*result*]\n");
	printf("[Disk array] | #block : %llu | #valid block : %llu | #read : %llu | #write : %llu | valid block(%) : %Lf | invalid block(%) : %Lf\n", sc->total_blk, sc->total_valid_blk_num, sc->total_read_num, sc->total_write_num, (sc->total_valid_blk_num/(long double)sc->total_blk)*100, (1-(sc->total_valid_blk_num/(long double)sc->total_blk))*100 );

	for ( i = 0; i < total_dev_num; i++ ) {
	printf(" 	 [Disk%d] | #block : %llu | #valid block : %llu | #read : %llu | #write : %llu | valid block(%) : %Lf | invalid block(%) : %Lf\n", i, sc->dev_info[i].total_blk, sc->dev_info[i].valid_blk_num, sc->dev_info[i].read_num, sc->dev_info[i].write_num, (sc->dev_info[i].valid_blk_num/(long double)sc->dev_info[i].total_blk)*100, (1-(sc->dev_info[i].valid_blk_num/(long double)sc->dev_info[i].total_blk))*100);
	}
}










































































	
	
