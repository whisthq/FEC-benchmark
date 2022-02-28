extern "C"
{
	#include "leopard.h"
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
	vector<char *> work_buf;
	void init(int num_real,int num_fec,int max_sz)
	{
		assert(leo_init()==0);

		int work_cnt=leo_encode_work_count(num_real, num_fec);
		work_buf.resize(work_cnt);
		//printf("<encode_work_cnt=%d>",work_cnt);
		for(int i=0;i<work_cnt;i++)
		{
			work_buf[i]=(char *)malloc(max_sz);
		}
		this->num_fec=num_fec;
		this->num_real=num_real;
	}
	void encode(char *src[], char *dst[], int sz)
	{
		assert(sz%64==0);
		int r=leo_encode(sz,num_real,num_fec,work_buf.size(),(void**)src, (void**)&work_buf[0]);
		//printf("[[[%d]]]",r);
		for(int i=0;i<num_fec;i++)
		{
			memcpy(dst[i],work_buf[i],sz);
		}
	}
};

struct Decoder
{
	int num_real;
	int num_fec;
	vector<char *> work_buf;
	vector<char *> real_buf; 
	vector<char *> fec_buf; 
	void init(int num_real,int num_fec,int max_sz)
	{
		this->num_fec=num_fec;
		this->num_real=num_real;
		int work_cnt=leo_decode_work_count(num_real,num_fec);
		work_buf.resize(work_cnt);
		//printf("<decode_work_cnt=%d>",work_cnt);
		for(int i=0;i<work_cnt;i++)
		{
			work_buf[i]=(char *)malloc(max_sz);
		}
		real_buf.resize(num_real);
		fec_buf.resize(num_fec);

	}
	void decode(char *pkt[],int index[],int sz)
	{
		assert(sz%64==0);
		int real_cnt=0;
		int fec_cnt=0;
		for(int i=0;i<num_real;i++)
			real_buf[i]=0;
		for(int i=0;i<num_fec;i++)
			fec_buf[i]=0;
			
		for(int i=0;i<num_real;i++)
		{
			if(index[i]<num_real)
			{
				real_buf[index[i]]=pkt[i];
				real_cnt++;
			}
			else{
				fec_buf[index[i]-num_real]=pkt[i];
				fec_cnt++;
			}
		}
		int r=leo_decode(sz,num_real,num_fec,work_buf.size(),(void**)&real_buf[0],(void**)&fec_buf[0],(void**)&work_buf[0]);
		//printf("<<<%d>>>",r);
		for(int i=0;i<num_real;i++)
		{
			//printf("<%d>", blocks[i].Index);
			if(real_buf[i])
			pkt[i]=real_buf[i];
			else
			pkt[i]=work_buf[i];	
		}
		//printf("\n");*/
	}
};
