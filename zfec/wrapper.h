extern "C"
{
#include <zfec/zfec.h>
}
#include <algorithm>
using namespace std;
struct Encoder
{
	fec_t *code;
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
		unsigned int index[num_real];
		for(int i=0;i<num_fec;i++)
		{
			index[i]=num_real+i;
		}
		fec_encode(code,(gf**)src,(gf**)dst,index,num_fec,sz);
	}
};

struct Decoder
{
	fec_t *code;
	int num_real;
	int num_fec;
	vector<char *> out;
	void init(int num_real,int num_fec,int max_sz)
	{
		code=fec_new(num_real,num_real+num_fec);
		this->num_fec=num_fec;
		this->num_real=num_real;

		out.resize(num_real);
		for(int i=0;i<num_real;i++)
		{
			out[i]=(char *)malloc(65535);
		}
	}
	void decode(char *pkt[],int index[],int sz)
	{
		for(int i=0;i<num_real;i++)
		{
			int idx=index[i];
			if(idx<num_real && idx!=i)
			{
				swap(index[idx],index[i]);
				swap(pkt[idx],pkt[i]);
				i--;
			}
		}

		fec_decode(code,(gf **)pkt,(gf**)&out[0],(unsigned int *)index,sz);
		for(int i=0;i<num_real;i++)
		{
			if(index[i]!=i)
			{
				pkt[i]=out[i];
			}
		}
	}
};
