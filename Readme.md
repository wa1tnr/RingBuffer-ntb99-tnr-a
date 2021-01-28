# Ring Buffer 轻量级环形缓冲区

## 简介
Ring Buffer 是一个基于C语言实现的轻量级环形缓冲区库，用户可新建一个普通的 uint8_t 数组作为缓冲区物理内存，通过与Ring Buffer操作句柄绑定，该数组即可转换为环形缓冲区并使用本库提供的函数对该缓冲区进行读写等操作；

Ring Buffer 适用于单片机串口收发等应用场景，与普通的数组缓冲区相比，Ring Buffer 的读写操作不需要记录地址、数据量加减和覆写等复杂的操作，Ring Buffer 仅使用少量的函数即可实现单字节、多字节读写、查询、清空环形缓冲区；Ring Buffer 用简洁的代码实现了核心的功能，小巧的内存占用和较高的运行效率非常适合移植到各单片机应用中；

## 使用方法
1. 复制库文件至工程中，在源代码中引用 Ring Buffer 头文件；
2. 新建一个 uint8_t 数组，数组的大小即是环形缓冲区的大小；
3. 新建 Ring Buffer 操作句柄；
4. 初始化 Ring Buffer 操作句柄，将其与刚才新建的数组绑定，转换成环形缓冲区；
5. 这时候我们就可以使用库提供的各种函数对新建的缓冲区进行读写等操作；

## 示例代码
展示了环形缓冲区实例化的过程和几个核心功能的使用方法，仅用于参考，并未将所有的函数都在示例中展示出来，函数详细内容请在编程的过程中参考ring_buffer.c内的注释；如果您在使用本库的过程中发现异常或确定BUG，请及时[Github](https://github.com/netube99/RingBuffer)向我反馈，谢谢；

**基础用法：**

```c
#include <stdio.h>
#include <ring_buffer.h>

#define Read_BUFFER_SIZE	256

int main()
{
    //新建缓冲区数组与Ring Buff操作句柄
    uint8_t buffer[Read_BUFFER_SIZE] ;
    ring_buffer RB ;

    //初始化Ring Buff操作句柄，绑定缓冲区数组；
    Ring_Buffer_Init(&RB, buffer, Read_BUFFER_SIZE);

    //向环形缓冲区写入一段字节和一个字节
    Ring_Buffer_Write_String(&RB, "hello world", 11);
    Ring_Buffer_Write_Byte(&RB, '!');

    //获取已储存的数据长度，读出环形缓冲区中的数据并打印
    uint32_t num = Ring_Buffer_Get_Lenght(&RB);
    uint8_t get[16] ;
    Ring_Buffer_Read_String(&RB, get, num);
    printf("%s", get);
    
    return 0 ;
}
```

**可用于串口收发的用法：**

```c
#include <stdio.h>
#include <ring_buffer.h>

#define Read_BUFFER_SIZE	256

//设定一个分隔关键词和关键词的长度（字节）
#define SEPARATE_SIGN       0xCCFB22AA
#define SEPARATE_SIGN_SIZE  4

int main()
{
    //新建缓冲区数组与Ring Buff操作句柄
    uint8_t buffer[Read_BUFFER_SIZE] ;
    ring_buffer RB ;

    //初始化Ring Buff操作句柄，绑定缓冲区数组；
    Ring_Buffer_Init(&RB, buffer, Read_BUFFER_SIZE);

    //记录段落数量
    uint8_t String_Count = 0 ;

    //向环形缓冲区写入三段数据，每段之间插入一个分隔关键词
    Ring_Buffer_Write_String(&RB, "ABCDEFGHIJK\r\n", 13);//写入一段数据
    Ring_Buffer_Insert_Keyword(&RB, SEPARATE_SIGN, SEPARATE_SIGN_SIZE);//插入一个分隔关键词
    String_Count ++ ;//记录段落数量 +1

    Ring_Buffer_Write_String(&RB, "abcdefg\r\n", 9);
    Ring_Buffer_Insert_Keyword(&RB, SEPARATE_SIGN, SEPARATE_SIGN_SIZE);
    String_Count ++ ;

    Ring_Buffer_Write_String(&RB, "1234\r\n", 6);
    Ring_Buffer_Insert_Keyword(&RB, SEPARATE_SIGN, SEPARATE_SIGN_SIZE);
    String_Count ++ ;

    while(String_Count != 0)
    {
        uint8_t get[16] ;
        //获得头指针到关键词高位的距离，距离-1得到第一段数据的长度
        uint8_t lenght = Ring_Buffer_Find_Keyword(&RB, SEPARATE_SIGN, SEPARATE_SIGN_SIZE) - 1 ;
        Ring_Buffer_Read_String(&RB, get, lenght);//读取一段数据，保存到get数组
        printf("%s", get);//打印数据
        Ring_Buffer_Delete(&RB, SEPARATE_SIGN_SIZE);//删除分隔关键词的长度的数据，即删除关键词
        String_Count -- ;//记录段落数量 -1
    }

    return 0 ;
}
```
## 更新日志
2021.01.19 v1.0.0 发布第一版本  
2021.01.24 v1.1.0 增加匹配字符查找函数  
2021.01.27 v1.2.0 重制匹配字符查找函数，现已支持8位到32位关键词查询  
2021.01.28 v1.3.0 复位函数修改为删除函数、增加关键词插入函数（自适应大小端）  