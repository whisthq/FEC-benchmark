#include <rs/rs.h>

struct Encoder
{
	void *code;
	int num_real;
	int num_fec;
	void init(int num_real,int num_fec,int max_sz)
	{
		code=fec_new(num_real,num_real+num_fec);
		this->num_fec=num_fec;
		this->num_real=num_real;
	}
	void encode(char *src[], char *dst[], int sz)
	{
		for(int i=0;i<num_fec;i++)
		{
			fec_encode(code,(void**)src,dst[i],num_real+i,sz);
		}
	}
};

struct Decoder
{
	void *code;
	void init(int num_real,int num_fec,int max_sz)
	{
		code=fec_new(num_real,num_real+num_fec);
	}
	void decode(char *pkt[],int index[],int sz)
	{
		fec_decode(code,(void **)pkt,index,sz);
	}
};
