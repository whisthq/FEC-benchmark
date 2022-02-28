extern "C"
{
#include <cm256/cm256.h>
}
#include <algorithm>
#include <assert.h>
#include <stdio.h>
using namespace std;
struct Encoder
{
	cm256_encoder_params_t param;
	int num_real;
	int num_fec;
	void init(int num_real,int num_fec,int max_sz)
	{
		assert(cm256_init()==0);

		this->num_fec=num_fec;
		this->num_real=num_real;
		param.OriginalCount=num_real;
		param.RecoveryCount=num_fec;
	}
	void encode(char *src[], char *dst[], int sz)
	{
		param.BlockBytes=sz;
		cm256_block blocks[num_real];
		for(int i=0;i<num_real;i++)
		{
			blocks[i].Block=src[i];
			blocks[i].Index=i;
		}
		for(int i=0;i<num_fec;i++)
		{
			cm256_encode_block(param,blocks,num_real+i,dst[i]);
		}
	}
};

struct Decoder
{
	cm256_encoder_params_t param;
	int num_real;
	int num_fec;
	void init(int num_real,int num_fec,int max_sz)
	{
		this->num_fec=num_fec;
		this->num_real=num_real;
		param.OriginalCount=num_real;
		param.RecoveryCount=num_fec;
	}
	void decode(char *pkt[],int index[],int sz)
	{
		param.BlockBytes=sz;
		cm256_block blocks[num_real];
		for(int i=0;i<num_real;i++)
		{
			blocks[i].Index=index[i];
			blocks[i].Block=pkt[i];
		}
		assert(cm256_decode(param,blocks)==0);
		//memset(pkt,0,num_real);
		for(int i=0;i<num_real;i++)
		{
			//printf("<%d>", blocks[i].Index);
			pkt[blocks[i].Index]=(char *)blocks[i].Block;	
		}
		//printf("\n");
	}
};
