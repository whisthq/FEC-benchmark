#include <common.h>

#include <vector>
#include <random>

#ifdef LUGI
#include <rs/wrapper.h>
#endif

#ifdef ZFEC
#include <zfec/wrapper.h>
#endif

#ifdef CM256
#include <cm256/wrapper.h>
#endif

#ifdef LH
#include <longhair/wrapper.h>
#endif

#ifdef LEO
#include <leopard/wrapper.h>
#endif

#include <string.h>
#include <assert.h>
#include <unistd.h>
using namespace std;


typedef unsigned long long u64_t; 
u64_t get_current_time_us()
{
	timespec tmp_time;
	clock_gettime(CLOCK_MONOTONIC, &tmp_time);
	return tmp_time.tv_sec*1000*1000l+tmp_time.tv_nsec/(1000l);
}

int my_rand()
{
	static random_device rd;
	static mt19937 g(rd());
	static uniform_int_distribution<> dis(1, 1000 * 1000 * 1000);
	return dis(g);
}


vector<char *> make_buffers(int num,int segment_size)
{
	vector<char *> buffers(num);
	for(int i=0;i<num;i++)
	{
		buffers[i]=(char *)malloc(segment_size);
	}
	return buffers;
}
void split_copy(string &block, char *output[], int segment_size)
{
	assert(block.size()%segment_size==0);
	int num=block.size()/segment_size;
	for(int i=0;i<num;i++)
	{
		memcpy(output[i], &block[i*segment_size],segment_size);
	}
}

string combine_copy(vector<char *> buffers,int segment_size)
{
    string res;
    for(int i=0;i<buffers.size();i++)
    {
        string tmp(segment_size,'a');
        memcpy(&tmp[0],buffers[i],segment_size);
        res+=tmp;
    }
    return res;
}
void one_test(int segment_size, int num_real,int num_fec)
{
	string input(segment_size*num_real,'a');
	for(int i=0;i<input.size();i++)
	{
		input[i]=my_rand()%256;
	}

	auto buffers=make_buffers(num_real+num_fec, segment_size);
	split_copy(input, &buffers[0],segment_size);

	Encoder encoder;
	encoder.init(num_real,num_fec,segment_size);

    {
        u64_t t1=get_current_time_us();
        encoder.encode(&buffers[0],&buffers[num_real],segment_size);
        u64_t t2=get_current_time_us();
        printf("encode_time=%f; ", (t2-t1)/1000.0);
    }


	vector<char*> pkt;
	vector<int> index;

    vector<int> shuffle;
    for(int i=0;i<num_real+num_fec;i++)
    {
        shuffle.push_back(i);
    }

    for(int i=0;i<(int)shuffle.size();i++)
    {
        swap(shuffle[i],shuffle[ my_rand()%shuffle.size()]);
    }



	for(int i=num_fec+num_real-1,cnt=0;cnt<num_real;i--,cnt++)
	{
		int idx;
		if(use_shuffle)
			idx=shuffle[i];
		else idx=i;
		pkt.push_back(buffers[idx]);
		index.push_back(idx);
	}

    //printf("<%d %d>\n",(int)pkt.size(),(int)index.size());

	Decoder decoder;
	decoder.init(num_real,num_fec,segment_size);
    {
        u64_t t1=get_current_time_us();
        decoder.decode(&pkt[0],&index[0],segment_size);
        u64_t t2=get_current_time_us();
        printf("decode_time=%f\n", (t2-t1)/1000.0);
    }

    string output;
    output=combine_copy(pkt,segment_size);

    //printf("<%d %d>\n",(int)input.size(),(int)output.size());


    /*
    for(int i=0;i<input.size();i++)
    {
        printf("[%02x]",(int)(unsigned char)input[i]);
    }
    printf("\n");

    for(int i=0;i<output.size();i++)
    {
        printf("[%02x]",(int)(unsigned char)output[i]);
    }
    printf("\n");*/

    assert(input.size()==output.size());
    assert(input==output);

}

int main()
{

    printf("segment_size=%d num_real=%d,num_fec=%d\n",segment_size,num_real,num_fec);

    for(int i=0;i<iterations;i++)
    {

        if(use_sleep) usleep(1000*1000);
        one_test(segment_size,num_real,num_fec);
    }
    return 0;
}
