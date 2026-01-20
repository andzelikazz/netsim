#ifndef SORTAGE_TYPES_HPP
#define SORTAGE_TYPES_HPP

#include "package.hxx"
#include <list>
#include <cstddef>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& product) = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual size_t size() const = 0;

    virtual bool empty() const = 0;

    virtual ~IPackageStockpile() = default;
};
class IPackageQueue :public IPackageStockpile {
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue() = delete;

    PackageQueue(PackageQueueType type) : queue_(), queue_type_(type) {}

    bool empty() const override {
        return queue_.empty();
    }
    Package pop() override;

    size_t size() const override {
        return queue_.size();
    }

    const_iterator begin() const override { return queue_.cbegin(); }

    const_iterator cbegin() const override { return queue_.cbegin(); }

    const_iterator end() const override { return queue_.end(); }

    const_iterator cend() const override { return queue_.cend(); }

    void push(Package&& product_) override {
        queue_.emplace_back(std::move(product_));
    }
    PackageQueueType get_queue_type() const override {
        return queue_type_;
    }

    ~PackageQueue() override = default;
private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};

#endif