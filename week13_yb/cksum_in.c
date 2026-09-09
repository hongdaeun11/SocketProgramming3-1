unsigned short
cksum_in(unsigned short *addr, int len)
{       
        unsigned long  sum = 0;
        unsigned short answer = 0;
        unsigned short *w = addr;
        int     nleft = len;
        
        while(nleft > 1) {
                sum += *w++;
		if(sum & 0x80000000)
			sum = (sum & 0xffff) + (sum >> 16);
                nleft -= 2;
        }
        
        if (nleft == 1) {
                *(unsigned char *)(&answer) = *(unsigned char *)w;
                sum += answer;
        }
        
	while(sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

        return(sum==0xffff)?sum:~sum; 
}       
