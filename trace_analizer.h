#ifndef __TRACE_ANALIZER_H__
#define __TRACE_ANALIZER_H__

#define GET_DEV_NUM(x)	(((x)-1)/3)		 
#define NUM_ELEMENT_DEV_INFO_STRUCT 3	/* dev_nr, first_sector, last_sector */
#define CONVERT_SECTORNUM_TO_BLKNUM(x) ((x)/8)
#define NO 0
#define YES 1
#define IS_MULTIPLES_OF_8(x) ( ((x)%8) ? NO : YES)


#define AVAILABLE_BLK_NUM(x,y) ((y)-(x))
#define SCALE_RATIO(x,y)  
#define WORKLOAD_DEVICE_NUM_ERROR -1
#define DEVICE_SIZE_ERROR -2
#define MAP_UNKNOWN_ERROR -3
#define MAX_RATIO 8+1 
#define SECTOR_NUM_PER_BLK 8
#define SUCCESS 0


typedef unsigned long long sector_t;
typedef char ** ptr_to_element_t;
typedef int error_t;

struct io_info {
	double time;
	unsigned int devno;
	unsigned long long blkno;
	unsigned int bcount;
	unsigned int is_read;
};

struct last_blk_info {
	unsigned long long blkno;
	unsigned int bcount;
};


struct dev_info {
	unsigned int dev_nr;
	sector_t physical_start_blk;
	sector_t total_blk;
	sector_t avail_blk_num;     /* This can be no used */
	sector_t valid_blk_num;
	sector_t read_num;
	sector_t write_num;
};

struct dev_set_c {
	unsigned int total_dev_num;
	sector_t start_blk; 			/*haven't been considered about physical_start_blk, thus start_blk is 0 */
	sector_t total_blk;
	sector_t avail_blk_num;			/* sum of each device(total_blk - physical_start_blk) */
	sector_t total_valid_blk_num;
	sector_t total_read_num;
	sector_t total_write_num;

	struct dev_info dev_info[0];
};


struct blk_record {
	unsigned long num_read;
	unsigned long num_write;
	unsigned int is_valid;
};	
	
struct table {
	unsigned long blk_num;
	struct blk_record blk_nr[0];
};


#endif
