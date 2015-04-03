/*
 * =====================================================================================
 *
 *       Filename:  media_scan_main.c
 *
 *    Description:  scan /dev/media* file
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

#ifndef __MEDIA_SCAN_MAIN_C__
#define __MEDIAN_SCAN_MAIN_C__

// headers
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/media.h>

void print_mdev_info(int num_media,struct media_device_info *p_dev);
void print_entity(struct media_entity_desc *p_entity);

// main()
int main(int argc,char *argv[])
{
	char dev_name[32];
	struct media_device_info mdev_info;
	int num_media_devices=0;
	int rc;
	int dev_fd;
	unsigned int num_entities = 1u;
	struct media_entity_desc entity;

	printf("media_scan_main:-----------------------------\n");
	while(1)
	{
		snprintf(dev_name,sizeof(dev_name),"/dev/media%d",num_media_devices);
		dev_fd = open(dev_name,O_RDWR|O_NONBLOCK);
		if(dev_fd < 0){
			printf("\nMedia scan is Over! num_media_devices:%d!\n",num_media_devices);
			break;
		}

		rc=ioctl(dev_fd,MEDIA_IOC_DEVICE_INFO,&mdev_info);
		if(rc < 0)
		{
			printf("Error: ioctl media_dev failed: %s!\n", strerror(errno));
			close(dev_fd);
			dev_fd = -1;
			break;
		}
		
		print_mdev_info(num_media_devices,&mdev_info);

		num_entities = 1u;

		while(1)
		{
			memset(&entity, 0, sizeof(entity));
			entity.id = num_entities++;

            rc = ioctl(dev_fd, MEDIA_IOC_ENUM_ENTITIES, &entity);
            if (rc < 0) {
                printf("Done enumerating media entities\n");
                rc = 0;
                break;
            }
			
			print_entity(&entity);
		}

		num_media_devices++;
	}


	return 0;
}

void print_mdev_info(int num_media,struct media_device_info *p_dev)
{
	int i;
	printf("\n/dev/media%d:\n",num_media);
	printf("drver: %s\n",p_dev->driver);
	printf("model: %s\n",p_dev->model);
	printf("serial: %s\n",p_dev->serial);
	printf("bus_info: %s\n",p_dev->bus_info);

	printf("media_version: %d\n",p_dev->media_version);
	printf("hw_revision: %d\n",p_dev->hw_revision);
	printf("driver_version: %d\n",p_dev->driver_version);

	printf("reserved: ");
	for(i=0;i<31;i++)
		printf(" %d",p_dev->reserved[i]);
	printf("\n");
}

void print_entity(struct media_entity_desc *p_entity)
{
	int i;
	printf("\nentity:\n");
	printf("id: %d\n",p_entity->id);
	printf("name: %s\n",p_entity->name);
	printf("type: %d\n",p_entity->type);
	printf("revision: %d\n",p_entity->revision);
	printf("flags: %d\n",p_entity->flags);
	printf("group_id: %d\n",p_entity->group_id);
	printf("pads: %d\n",p_entity->pads);
	printf("links: %d\n",p_entity->links);
	printf("reserved: ");
	for(i=0;i<4;i++)
		printf(" %d",p_entity->reserved[i]);
	printf("\n");
}
#endif

