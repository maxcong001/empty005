#include <iostream>
#include <vector>
#include <string.h>
#include <atomic>
using namespace std;
#define RB_SIZE 200
#define RB_ELEMENT_SIZE 4096
class ring_buffer
{
public:
    ring_buffer():element_num(RB_SIZE),head_index(RB_SIZE/2),tail_index(RB_SIZE/2){init();};

    ~ring_buffer(){destroy();};

    int element_num;

    bool init();
    bool destroy();

    bool add(const char* buf, size_t length);
    bool del();
    bool empty();
    size_t size();

    char* peek_element();
    char* get_element();
    char* peek_tail_element();

    void dump()
    {
        std::cout<<"ring buffer element is \n"<<std::endl;
        for(int i = 0; i < RB_SIZE; i++)
        {
            std::cout<<"element num :"<< i<<" : "<<static_cast<const void*>(_ring_buffer[i])<<std::endl;
        }
    }

    std::vector<char*> _ring_buffer;

    std::atomic<long> head_index;
    // humm this is the next tail index
    std::atomic<long> tail_index;


};
char* ring_buffer::peek_tail_element()
{
    if(!empty())
    {
        return _ring_buffer[tail_index];
    }
    else
    {
        std::cout << "ring buffer is empty!"<<std::endl;
        return NULL;
    }   
}

char* ring_buffer::peek_element()
{
    if(!empty())
    {
        return _ring_buffer[head_index];
    }
    else
    {
        std::cout << "ring buffer is empty!"<<std::endl;
        return NULL;
    }    
}

char* ring_buffer::get_element()
{
    if(!empty())
    {
        char* tmp = _ring_buffer[head_index];
        if (!del())
        {
            std::cout << "del return fail!"<<std::endl;
            return NULL;
        }
        return tmp;
    }
    else
    {
        std::cout << "ring buffer is empty!"<<std::endl;
        return NULL;
    }

}

size_t ring_buffer::size()
{
    return (head_index >= tail_index)?(head_index - tail_index):(RB_SIZE - tail_index + head_index);
}
bool ring_buffer::add(const char* buf, size_t length)
{
    if (length >RB_ELEMENT_SIZE)
    {
        return false;
    }

    long tmp_index = tail_index;
    if (!tail_index)
    {
        if(head_index == (RB_SIZE - 1))
        {
            std::cout << "ring buffer is full"<<std::endl;
            // full actually one place left
            return false;
        }
        tail_index = (RB_SIZE - 1);
    }
    else
    {
        --tail_index;
        if (tail_index == head_index)
        {
            std::cout << "ring buffer is full"<<std::endl;
            ++tail_index;
            //full
            return false;
        }
    }

    char* tmp_queue_ptr = _ring_buffer[tmp_index];
    if (!memcpy(tmp_queue_ptr, buf, length))
    {
        return false;
    }
    return true;
}

bool ring_buffer::del()
{
    
    if (head_index == tail_index)
    {
        std::cout << "ring buffer is empty"<<std::endl;
        return false;
    }

    if (!head_index)
    {      
        head_index = (RB_SIZE - 1);
    }
    else
    {
        --head_index;
    }
    return true;
}
bool ring_buffer::empty()
{
    return (tail_index == head_index);
}

bool ring_buffer::init()
{
    _ring_buffer.reserve(RB_SIZE);
    for (int i = 0; i < RB_SIZE; i++ )
    {
        char* tmp_ele_buffer = new char[RB_ELEMENT_SIZE];
        memset(tmp_ele_buffer, 0, RB_ELEMENT_SIZE);
        if(tmp_ele_buffer)
        {
            _ring_buffer[i] = tmp_ele_buffer;
        }
        else
        {
            // log here
        }
    }
    return true;
}

bool ring_buffer::destroy()
{
    for (int i = 0; i < RB_SIZE; i++ )
    {
        char* tmp_ele_buffer = _ring_buffer[i];
        delete [] tmp_ele_buffer;
    }
    return true; 
}


