extern "C"
{
#include <longhair/cauchy_256.h>
}

#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <string.h>
using namespace std;
struct Encoder
{
	int num_real;
	int num_fec;
	void init(int num_real,int num_fec,int max_sz)
	{
		assert(cauchy_256_init()==0);

		this->num_fec=num_fec;
		this->num_real=num_real;
	}
	void encode(char *src[], char *dst[], int sz)
	{
		char dst0[sz*num_fec];
		assert(sz%8==0);
		cauchy_256_encode(num_real,num_fec,(const unsigned char **)src,dst0,sz);
		for(int i=0;i<num_fec;i++)
		{
			memcpy(dst[i],&dst0[i*sz],sz);
		}
	}
};

struct Decoder
{
	int num_real;
	int num_fec;
	void init(int num_real,int num_fec,int max_sz)
	{
		this->num_fec=num_fec;
		this->num_real=num_real;
	}
	void decode(char *pkt[],int index[],int sz)
	{
		Block blocks[num_real];
		for(int i=0;i<num_real;i++)
		{
			blocks[i].row=index[i];
			blocks[i].data=(unsigned char *)pkt[i];
		}

		assert(cauchy_256_decode(num_real,num_fec,blocks,sz)==0);
		//memset(pkt,0,num_real);
		for(int i=0;i<num_real;i++)
		{
			//printf("<%d>", blocks[i].Index);
			pkt[blocks[i].row]=(char *)blocks[i].data;	
		}
		//printf("\n");
	}
};
