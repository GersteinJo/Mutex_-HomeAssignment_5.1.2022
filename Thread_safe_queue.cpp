#include <queue>
#include <thread>
#include <mutex>
#include <iostream>

template<typename T>
class queue_safe
{
private:
    std::queue<T> queue_s;
    std::mutex queue_mutex;
public:
    queue_safe() = default;
    ~queue_safe() = default;

    void push(T elem)
    {
        std::lock_guard lock_queue(queue_mutex);
        queue_s.push(elem);
    }

//    template<class Arg, class ... Args>
//    void emplace(const Arg& fst_arg, const Args&... rest_args)
//    {
//        queue_s.emplace(fst_arg, rest_args);
//    };
    void pop()
    {
        std::lock_guard lock_queue(queue_mutex);
        queue_s.pop();
    }
    T peek()
    {
        std::lock_guard lock_queue(queue_mutex);
        return queue_s.peek();
    }
    T& front()
    {
        std::lock_guard lock_queue(queue_mutex);
        return queue_s.front();
    }
    T& back()
    {
        std::lock_guard lock_queue(queue_mutex);
        return queue_s.back();
    }
};


queue_safe<int> global_queue;

void push_nums_in_range(int first, int last)
{
    for(int i = last; i>= first; i--) global_queue.push(i);
}

int main()
{
    std::thread other(push_nums_in_range, 1,10);
    push_nums_in_range(11,20);
    other.join();
    int front = global_queue.front();
    int back = global_queue.back();
    std::cout<<front<<" "<<back<<"\n";
}
