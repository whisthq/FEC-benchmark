extern "C"
{
#include <fecal/fecal.h>
}

#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
struct Encoder
{
	int num_real;
	int num_fec;
	FecalEncoder encoder;
	void init(int num_real,int num_fec,int max_sz)
	{
		fecal_init();
		//assert(cauchy_256_init()==0);

		this->num_fec=num_fec;
		this->num_real=num_real;

		//wirehair_encoder_create(code );
	}
	void encode(char *src[], char *dst[], int sz)
	{
		/*
		char buf[num_real*sz];
		for(int i=0;i<num_real;i++)
		{
			memcpy(&buf[i*sz], src[i],sz);
		}*/

		encoder=fecal_encoder_create(num_real, (void **)src , num_real*sz);

		for(int i=0;i<num_fec;i++)
		{

			FecalSymbol symbol;
			symbol.Index=i;
			symbol.Bytes=sz;
			symbol.Data=dst[i];

			int r=fecal_encode(encoder, &symbol);
			//printf("<%d %d %d>\n",r,sz,out_sz);
			//assert(out_sz==sz);
		}
	}
};

struct Decoder
{
	int num_real;
	int num_fec;
	FecalDecoder decoder;

	vector<char *> real_pkt; 

	int ok=0;

	void init(int num_real,int num_fec,int max_sz)
	{
		this->num_fec=num_fec;
		this->num_real=num_real;
		ok=0;

		decoder=fecal_decoder_create(num_real,num_real*max_sz);
		real_pkt.resize(num_real);
		for(int i=0;i<num_real;i++)
		{
			real_pkt[i]=0;
		}
	}

	void feed(int index,char *pkt,int sz)
	{
		if(index<num_real)
		{
			FecalSymbol symbol;
			symbol.Data=pkt;
			symbol.Index=index;
			symbol.Bytes=sz;
			fecal_decoder_add_original(decoder,&symbol);
			real_pkt[index]=pkt;
		}
		else
		{
			FecalSymbol symbol;
			symbol.Data=pkt;
			symbol.Index=index-num_real;
			symbol.Bytes=sz;
			fecal_decoder_add_recovery(decoder,&symbol);
		}
	}
	
	int try_decode(char *pkt[],int sz)
	{
	
		RecoveredSymbols recovered_symbols;

		int r= fecal_decode(decoder, &recovered_symbols);
		
		if(r!=0) return -1;

		//printf("<<%d>>", recovered_symbols.Count);
		//assert(recovered_symbols.Count==num_real);
		for(int i=0;i<num_real;i++)
		{
			if(real_pkt[i])
			{
				pkt[i]=real_pkt[i];
			}
		}
		for(int i=0;i<recovered_symbols.Count;i++)
		{
			int idx=recovered_symbols.Symbols[i].Index;
			pkt[idx]=(char*)recovered_symbols.Symbols[i].Data;
		}
		//printf("<count=%d>",recovered_symbols.Count);
		return 0;
	}
};
