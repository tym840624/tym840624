

https://www.bilibili.com/video/BV1cu411e78i/?spm_id_from=333.788.recommend_more_video.4&vd_source=769e850dd5e1193096d8cdf92f49862c
鲍松山



void(*func)();

把0地地址转为函数指针, 并调用此函数

( void(*)() ) 0;

( * ( void(*)() )0 )   ();

( *( void(*)() )0 ) ();

与上面等价
((void(*)())0) ()



//////函数func一个int入参，返回值为指向函数的指针。以下编译不过，
void(*)() func(int a)  --> 得用这个: void(*func(int))()

/* 不能这么写
void(*)() func(int a)
{
    return 0;
}
*/

void(*func(int))()
{
    return 0;
}


/////函数func2一个int入参，返回值为指向数组的指针。
int (*func2(int a))[5]
{
    return 0;
}



int *(*(*func)(int *))[10];


int (*func)(int *, int (*)(int*));

int (*func[5])(int *);
int (*(*func)[5])(int *)
int (*(*func)(int *))[5];

(*(void(*)())0)();

void(*set_handle(void(*f)()))();


int a;
double b;
int ar[10];
struct Test t;
int (*pfun)();



int (*(*func)[5])(int*);  --> int (*)(int*) (*func)[5]
(*func)[5]  指向数组的指针，每个元素是函数指针
typedef int(*pFun)(int*);  //重定义pFun为函数指针类型
pFun (*func)[5];  //func为指向数组的指针，元素为pFunc




