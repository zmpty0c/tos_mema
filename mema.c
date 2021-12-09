#include <mema.h>
#include "rtt_log.h"
tos_mema_ptr *rootmema;
uint8_t membuf[mema_buffer_size];

/********************************************************************************
func: 内存初始化
time: 2015-09-14
ver.: v1.0
http://www.trtos.com/
by:童金吕
note; 传入内存堆地址
********************************************************************************/
void mema_init(uint8_t *addr)
{
	rootmema=(tos_mema_ptr *)addr;//绑定位置到内存块头
	rootmema->nextptr=0;
	rootmema->size=sizeof(tos_mema_ptr);
}
/********************************************************************************
func: 分配内存
time: 2015-09-14
ver.: v1.0
http://www.trtos.com/
by:童金吕
note; 去掉了父节点
********************************************************************************/
void *mema_malloc(uint16_t size)
{
	tos_mema_ptr *p=rootmema,*tp;
	size+=sizeof(tos_mema_ptr);
	while(p->nextptr!=0)
	{
		if(((uint32_t)p->nextptr-(uint32_t)p-p->size)>=size)//当当前节点和后节点之间能插入请求块
		{
			tp=(tos_mema_ptr *)((uint32_t)p+p->size);
			tp->size=size;
			tp->nextptr=p->nextptr;
			p->nextptr=tp;
			return (uint8_t *)((uint32_t)tp+sizeof(tos_mema_ptr));
		}
		p=p->nextptr;
	}
	tp=(tos_mema_ptr *)((uint32_t)p+p->size);
    if(((uint32_t)rootmema+mema_buffer_size)<=((uint32_t)p+size))
    {
        rtt_error("not free mema");
        return 0;
    }
	tp->nextptr=0;
	tp->size=size;
	p->nextptr=tp;
	return (uint8_t *)((uint32_t)tp+sizeof(tos_mema_ptr));
}
/********************************************************************************
func: 计算内存使用情况
time: 2015-09-14
ver.: v1.0
http://www.trtos.com/
by:童金吕
note; 去掉了父节点
********************************************************************************/
uint32_t mema_count()
{
	tos_mema_ptr *p=rootmema;
	uint32_t size=p->size;
	while(p->nextptr!=0)
	{
		p=p->nextptr;
		size+=p->size;
	}
	return size;
}
/********************************************************************************
func:释放占用内存
time: 2015-09-14
ver.: v1.0
http://www.trtos.com/
by:童金吕
note; 去掉了父节点
********************************************************************************/
void mema_free(void *addr)
{
	tos_mema_ptr *p=rootmema,*tp=0;
	while(p->nextptr)
	{
		if((uint32_t)p==((uint32_t)(uint8_t *)addr-sizeof(tos_mema_ptr)))goto free;
		tp=p;
		p=p->nextptr;
	}
	if(tp)tp->nextptr=0;return;
free:
	tp->nextptr=p->nextptr;//当本节点存在子节点父节点的子节点等于自身节点子节点
}
/********************************************************************************
func:获取当前内存长度
time: 2015-09-14
ver.: v1.0
http://www.trtos.com/
by:童金吕
note; 
********************************************************************************/
uint32_t mema_getsize(void *addr)
{
	tos_mema_ptr *p;
	 p=(tos_mema_ptr *)((uint32_t)addr-sizeof(tos_mema_ptr));
	return (p->size-sizeof(tos_mema_ptr));
}
uint16_t buffercoppy(void *s,void *out,uint16_t length)
{
	uint16_t count=0;
	uint8_t *ps,*pd;
	ps=(uint8_t *)s;
	pd=(uint8_t *)out;
	count =length;
	while (length--)
	{
		(*pd++)=(*ps++);
	}
	return count;
} 
/********************************************************************************
func:改变容量
time: 2015-11-12
ver.: v1.0
http://www.trtos.com/
by:童金吕
note; 
********************************************************************************/
void *mema_extend(void *addr,uint16_t size)
{
	uint8_t *pr;
	tos_mema_ptr *p;
	pr=mema_malloc(size);
	p=(tos_mema_ptr *)((uint8_t *)addr-sizeof(tos_mema_ptr));
	buffercoppy(addr,pr,p->size-sizeof(tos_mema_ptr));
	mema_free(addr);
	return pr;
}

