通过 修改 定义 #define mema_buffer_size 1024*2  调整内存分配池大小


void mema_init(uint8_t *addr); 初始化 内存  填入内存池地址


char *buf=mema_malloc(128);分配内存

mema_free(buf);  释放内存
