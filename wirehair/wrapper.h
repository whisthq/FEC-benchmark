extern "C"
{
#include <wirehair/wirehair.h>
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
	WirehairCodec code;
	void init(int num_real,int num_fec,int max_sz)
	{
		wirehair_init();
		//assert(cauchy_256_init()==0);

		this->num_fec=num_fec;
		this->num_real=num_real;

		//wirehair_encoder_create(code );
	}
	void encode(char *src[], char *dst[], int sz)
	{
		char buf[num_real*sz];
		for(int i=0;i<num_real;i++)
		{
			memcpy(&buf[i*sz], src[i],sz);
		}

		code=wirehair_encoder_create(0, buf, num_real*sz, sz);

		unsigned int out_sz=111;
		for(int i=0;i<num_fec;i++)
		{
			int r=wirehair_encode(code,num_real+i,dst[i],sz,&out_sz);
			//printf("<%d %d %d>\n",r,sz,out_sz);
			//assert(out_sz==sz);
		}
	}
};

struct Decoder
{
	int num_real;
	int num_fec;
	WirehairCodec code;

	int ok=0;

	void init(int num_real,int num_fec,int max_sz)
	{
		this->num_fec=num_fec;
		this->num_real=num_real;
		ok=0;

		code=wirehair_decoder_create(0,num_real*max_sz,max_sz);
	}

	void feed(int index,char *pkt,int sz)
	{
		int r=wirehair_decode(code,index,pkt,sz);
		if(r==0) ok=1;
	}

	int try_decode(char *pkt[],int sz)
	{
		if(ok==0) return -1;
		char buf[num_real*sz];

		wirehair_recover(code,buf,num_real*sz);
		for(int i=0;i<num_real;i++)
		{
			memcpy(pkt[i],&buf[i*sz],sz);
		}
		return 0;
	}
};
