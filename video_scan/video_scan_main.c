/*
 * =====================================================================================
 *
 *       Filename:  video_scan_main.c
 *
 *    Description:  scan /dev/video* file
 *
 *        Version:  1.0
 *        Created:  04/03/2015 03:16:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuchangjian, 
 *        Company:  
 *
 * =====================================================================================
 */

/*****************************************************************************************
 *		History:
 *
 * 		Name			Date			Ver			Act
 *-----------------------------------------------------------------------------------------
 *		liuchangjian	2015-04-03		v1.0		create
 *
 *
 * ****************************************************************************************/

#ifndef __VIDEO_SCAN_MAIN_C__
#define __VIDEO_SCAN_MAIN_C__

// headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/videodev2.h>

void print_video_info(int fd,int num_video);

// main()
int main(int argc,char *argv[])
{
	char dev_name[32];
	int num_video_devices=0;
	int rc;
	int dev_fd;

	printf("video_scan_main:-----------------------------\n");
	while(1)
	{
		snprintf(dev_name,sizeof(dev_name),"/dev/video%d",num_video_devices);
		dev_fd = open(dev_name,O_RDONLY);
		
		if(dev_fd < 0){
			if(errno == ENOENT)			// No such file or directory
			{
				printf("Video scan is Over! The number of VideoDevice: %d!\n\n",num_video_devices);		
				close(dev_fd);
				break;
			}
			else
			{
				printf("\n!!!Error!!! Video%d: %s!\n",num_video_devices,strerror(errno));
				close(dev_fd);
				num_video_devices++;
				continue;
			}

		}
		else
			print_video_info(dev_fd,num_video_devices);

		close(dev_fd);

		num_video_devices++;
	}


	return 0;
}
int print_video_cap(int fd)
{
	int rc;
	struct v4l2_capability cap;
	int i;

	rc = ioctl(fd,VIDIOC_QUERYCAP,&cap);

	if(rc < 0){
		printf("Error: Can't querycap!!!\n");
		return rc;
	}
	printf("v4l2 capability:\n");

	//driver
	printf("driver:");
	for(i=0;i<16;i++)
		printf(" %d",cap.driver[i]);
	printf("\n");

	//card
	printf("card:");
	for(i=0;i<32;i++)
		printf(" %d",cap.card[i]);
	printf("\n");

	//bus info
	printf("bus info:");
	for(i=0;i<32;i++)
		printf(" %d",cap.bus_info[i]);
	printf("\n");

	//version
	printf("version: %d\n",cap.version);

	//cap
	printf("capability: 0x%x\n",cap.capabilities);

	//device caps
	printf("device caps: 0x%x\n",cap.device_caps);

	printf("reserved:");
	for(i=0;i<3;i++)
		printf(" %d",cap.reserved[i]);
	printf("\n");
	
	return rc;
}

int print_video_enumfmt(int fd)
{
	int rc;
	struct v4l2_fmtdesc fmt;
	int i;

	rc = ioctl(fd,VIDIOC_ENUM_FMT,&fmt);

	if(rc < 0){
		printf("Error: Can't ENUM FORMAT!!!\n");
		return rc;
	}

	printf("v4l2 enum format:\n");
	//index
	printf("index: %d\n",fmt.index);
	//type
	printf("type: %d\n",fmt.type);
	//flags
	printf("flags: %d\n",fmt.flags);
	//description
	printf("description:");
	for(i=0;i<32;i++)
		printf(" %d",fmt.description[i]);
	printf("\n");
	//pixelformat
	printf("pixelformat: %d\n",fmt.pixelformat);
	//reserved
	printf("reserved:");
	for(i=0;i<4;i++)
		printf(" %d",fmt.reserved[i]);
	printf("\n");

	return rc;
}

void print_video_info(int fd,int num_video)
{
	int i;
	
	printf("\nVideo%d:\n",num_video);
	// cap
	if(print_video_cap(fd)<0)
		return;
	
	// enum fmt
	print_video_enumfmt(fd);
	printf("\n");
}

#endif

