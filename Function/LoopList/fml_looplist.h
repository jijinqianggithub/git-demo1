#ifndef FML_LOOPLIST_H_
#define FML_LOOPLIST_H_

#ifndef FML_LOOPLIST_C_
#define LOOPLIST_Ex_ extern
#else
#define LOOPLIST_Ex_
#endif

#include "stm32f4xx.h"
	

#define min(a, b) (a)<(b)?(a):(b)                   ///< 获取最小值

#define Usart1ReadLen 512

/** 环形缓冲区数据结构 */
typedef struct {
    uint32_t  rbCapacity;//空间大小
    char  *rbHead; //头
    char  *rbTail; //尾
    char  *rbBuff; //数组的首地址
}rb_t;

LOOPLIST_Ex_ rb_t pRb;

void rbCreate(rb_t *rb,void *Buff,uint32_t BuffLen);//创建或者说初始化环形缓冲区
void rbDelete(rb_t* rb);
int32_t rbCapacity(rb_t *rb);//得到环形大小
int32_t rbCanRead(rb_t *rb);//能读出数据的个数
int32_t rbCanWrite(rb_t *rb);//还剩余的空间
int32_t rbRead(rb_t *rb, void *data, uint32_t count);//读取数据
int32_t rbWrite(rb_t *rb, const void *data, uint32_t count);
int32_t PutData(rb_t *rb ,void *buf, uint32_t len);

#endif





























