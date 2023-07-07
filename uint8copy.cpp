
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;

typedef unsigned char __uint8_t;
typedef __uint8_t uint8_t;


#define NUM 100000
static uint8_t cache_packet[1600];
static uint32_t cache_size = 0;
static uint32_t cache_seq = 0;
static uint32_t lost_num = 0;
static bool is_sent = false;

int main()
{
    //uint8_t data[1600]={0};
    char srcdata[1600] = {0};

    for(int i=0; i<NUM; i++){

        if( (i > 0) 
            && (i % 5000 == 0)
            && lost_num < 10 ){
            sprintf(srcdata, "data:%d", i);
            memcpy(cache_packet, srcdata, strlen(srcdata));
            cache_size = strlen(srcdata);
            cache_seq = i;
            lost_num++;
            cout << "save seq = " << i << ", size = " << cache_size << ",lost_num:" << lost_num << endl;
            is_sent = true;
        }

        if( (i > 0) 
            && (i - cache_seq == 1000)
            && lost_num < 11 
            && is_sent){
            cout << "send seq = " << cache_seq << ", size = " << cache_size << ", " << cache_packet << ",lost_num:" << lost_num << endl;
            memset(cache_packet, 0, sizeof(cache_packet));
            usleep(100000);
        }
    }
    return 0;

 

}