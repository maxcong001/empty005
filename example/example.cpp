#include "ringbuffer.hpp"
#include <iostream>
#include <string>
#include "stdio.h"
using namespace std;

int main()
{
    string tmp_str("tst");
    ring_buffer tmp;
    tmp.dump();
    for (int i = 0; i <1000; i++)
    {
        tmp.add(tmp_str.c_str(), tmp_str.size());
        printf("size of ring buffer is %d\n", tmp.size());
        printf("element address is %p, tail is %p\n", tmp.peek_element(), tmp.peek_tail_element());
        if(!(i%7))
        {
            printf("delete element %p\n", tmp.get_element());
        }
        printf("\n");

    }
}
