#include "ft_ping.h"

unsigned short in_cksum(void *ptr, int len)
{
	unsigned short	*p;
	unsigned int 	checksum;

	checksum = 0;
	p = ptr;
	while (len > 1)
	{
		checksum += *p++;
		len -= sizeof(unsigned short);
	}
	if (len == 1)
		checksum += *(unsigned char*)ptr;
	checksum = (checksum >> 16) + (checksum & 0xFFFF);
	return ((unsigned short)~checksum);
}