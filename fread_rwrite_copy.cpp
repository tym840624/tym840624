#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <sstream>
#include <unistd.h>


using namespace std;

void copytest(char* fileSrc, char* fileDes)
{
    FILE* fpSrc = fopen(fileSrc, "r");

    if( fpSrc == nullptr){
        std::cout << "open fileSrc faile" << std::endl;
        return ;
    }

    unsigned int index = 0;
    bool has_open_file = false;
    FILE* fpDes = nullptr;
    string filename = "";

    while( !feof(fpSrc)){

        if(has_open_file){
            struct stat st = { 0 };
            stat(filename.c_str(), &st);
            int size = st.st_size;
            if(size >= 1024){
                fflush(fpDes);
                fclose(fpDes);
                cout << "close " << filename << " success!" << endl;
                has_open_file = false;
            }else{
                char buf[10] = { 0 };
                int res = fread(buf, sizeof(char), 1, fpSrc);
                fwrite(buf, sizeof(char), res, fpDes);
                index++;
            }
        }else{
            std::stringstream ss;
            ss <<"file_copy_" << (time(NULL)) << ".txt";
            filename = ss.str();
            fpDes = fopen(filename.c_str(), "w+");
            cout << "open " << filename << " success!" << endl;
            has_open_file = true;
            
        }
        fflush(fpDes);
        usleep(1000)
    }
    fclose(fpSrc);
    fclose(fpDes);
    std::cout << "index:" << index << std::endl;
}

int main(int arge, char **arges)
{
    if(arge < 2){
        cout << "usag: prog fileSrc" << endl;
        return 0;
    }
    copytest(arges[1], arges[2]);
    return 0;
}

/*  每次拷贝10k的数据。如果文件小于10k则一次性拷贝完，大于10k的的，则用10k的临时空间来完成复制
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#define NUM 1024 * 10 //10KB

void mycopy_dynamic(char *str1, char *str2)
{
    FILE *p = fopen(str1, "rb");
    FILE *p1 = fopen(str2, "wb");
    struct stat st = { 0 };
    stat(str1, &st);
    int size = st.st_size;
    if(size >= NUM)
        size = NUM;
    char *buffer = malloc(size);
    unsigned int index = 0;
    while(!feof(p))
    {
        int res = fread(buffer, sizeof(char), size, p);
        fwrite(buffer, sizeof(char), res, p1);
        index++;
    }
    free(buffer);
    fclose(p);
    fclose(p1);
    printf("%d\n", index);
}
int main(int arge, char **arges)
{
    if(arge < 3)
        return 0;
    mycopy_dynamic(arges[1], arges[2]);
    return 0;
}

void mycopy_test(char *str1, char *str2)
{
    FILE *p = fopen(str1, "rb");
    FILE *p1 = fopen(str2, "wb");
    unsigned int index = 0;
    while(!feof(p))
    {
        char buf[10] = { 0 };
        fread(buf, sizeof(char), 3, p);
        fwrite(buf, sizeof(char), 3, p1);   //这个会多复制1个或2个字节
        index++;
    }
    fclose(p);
    fclose(p1);
    printf("%d\n", index);
}


void mycopy_test(char *str1, char *str2)
{
    FILE *p = fopen(str1, "rb");
    FILE *p1 = fopen(str2, "wb");
    unsigned int index = 0;
    while(!feof(p))
    {
        char buf[10] = { 0 };
        int res = fread(buf, sizeof(char), 3, p);
        fwrite(buf, sizeof(char), res, p1);
        index++;
    }
    fclose(p);
    fclose(p1);
    printf("%d\n", index);
}
*/