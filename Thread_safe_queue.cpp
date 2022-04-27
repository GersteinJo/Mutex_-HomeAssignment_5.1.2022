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

    template<class Arg, class ... Args>
    void emplace(const Arg& fst_arg, const Args&... rest_args)
    {
        queue_s.emplace(fst_arg, rest_args...);
    };

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
    size_t size()
    {
        std::lock_guard lock_queue(queue_mutex);
        return queue_s.size();
    }
};


queue_safe<int> global_queue;

void push_nums_in_range(int first, int last)
{
    for(int i = last; i>= first; i--) global_queue.push(i);
}

int main()
{

    for(int i = 0; i<= 10000; i++) global_queue.push(i);
    std::thread other(push_nums_in_range, 0,10000);
    for(int i = 0; i<= 20000; i++) global_queue.pop();
    other.join();
    std::cout<<global_queue.size()<<"\n";
}
