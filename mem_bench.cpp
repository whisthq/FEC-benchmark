#include <common.h>

#include <vector>
#include <random>

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


void malloc_test(int malloc_size,int malloc_num)
{
    //const int malloc_num=1000,malloc_size=1280;
    char buf[malloc_size];
    for(int i=0;i<malloc_size;i++)
        buf[i]=rand()%256;


    vector<char *> buffers(malloc_num);
    
    {
        u64_t t1=get_current_time_us();

        for(int i=0;i<malloc_num;i++)
        {
            buffers[i]=(char *)malloc(malloc_size);
        }

        u64_t t2=get_current_time_us();
        printf("malloc_time=%f  ", (t2-t1)/1000.0);
    }

    {
        u64_t t1=get_current_time_us();

        for(int i=0;i<malloc_num;i++)
        {
            memcpy(buffers[i],buf,malloc_size);
        }

        u64_t t2=get_current_time_us();
        printf("memcpy_time=%f  ", (t2-t1)/1000.0);
    }

    {
        u64_t t1=get_current_time_us();
        for(int i=0;i<malloc_num;i++)
        {
            free(buffers[i]);
        }
        u64_t t2=get_current_time_us();
        printf("free_time=%f\n", (t2-t1)/1000.0);
    }
}
int main()
{

    printf("segment_size=%d, num=%d\n",segment_size,num_mem_test_segments);
    for(int i=0;i<iterations;i++)
    {
        if(use_sleep) usleep(1000*1000);
        malloc_test(segment_size,num_mem_test_segments);
    }

    return 0;
}
