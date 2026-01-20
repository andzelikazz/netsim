#include "storage_types.hxx"



Package PackageQueue::pop() {
    Package package;

    switch (queue_type_) {
    case PackageQueueType::LIFO:
        package = std::move(queue_.back());
        queue_.pop_back();
        break;
    case PackageQueueType::FIFO:
        package = std::move(queue_.front());
        queue_.pop_front();
        break;
    }
    return package;
}