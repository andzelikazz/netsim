#ifndef NODES_HPP_
#define NODES_HPP_
NODES_HPP_

#include <map>
#include <memory>
#include <optional>

#include "helpers.cpp"
#include "storage_types.hxx"
#include "types.hxx"

enum class ReceiverType { WORKER, STOREHOUSE };

class IPackageReceiver {
public:
  virtual void receive_package(Package &&p) = 0;
  virtual ElementID get_id() const = 0;
  virtual ReceiverType get_receiver_type() const = 0;

  virtual IPackageStockpile::const_iterator begin() const = 0;
  virtual IPackageStockpile::const_iterator end() const = 0;
  virtual IPackageStockpile::const_iterator cbegin() const = 0;
  virtual IPackageStockpile::const_iterator cend() const = 0;

  virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
  using preferences_t = std::map<IPackageReceiver *, double>;
  using const_iterator = preferences_t::const_iterator;

  explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator)
      : pg_(std::move(pg)) {}
  void add_receiver(IPackageReceiver *r);
  void remove_receiver(IPackageReceiver *r);
  IPackageReceiver *choose_receiver();
  const preferences_t &get_preferences() const { return preferences_; };

  const_iterator begin() const { return preferences_.begin(); };
  const_iterator end() const { return preferences_.end(); };
  const_iterator cbegin() const { return preferences_.cbegin(); };
  const_iterator cend() const { return preferences_.cend(); };

private:
  preferences_t preferences_;
  ProbabilityGenerator pg_;

  void normalize();
};

class PackageSender {
public:
  PackageSender() = default;
  PackageSender(PackageSender &&) = default;
  void send_package();
  const std::optional<Package> &get_sending_buffer() const { return buffer_; }

  ReceiverPreferences receiver_preferences;

protected:
  void push_package(Package &&p) { buffer_.emplace(std::move(p)); }
  std::optional<Package> buffer_;
};

class Ramp : public PackageSender {
public:
  Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {};
  void deliver_goods(Time t);
  TimeOffset get_delivery_interval() const { return di_; };
  ElementID get_id() const { return id_; };

private:
  ElementID id_;
  TimeOffset di_;
  Time t_;
};

class Worker : public PackageSender, public IPackageReceiver {
public:
  Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
      : id_(id), pd_(pd), q_(std::move(q)) {}

  void do_work(Time t); //
  void receive_package(Package &&p) override; //
  ElementID get_id() const override { return id_; }
  ReceiverType get_receiver_type() const override {
    return ReceiverType::WORKER;
  };
  IPackageStockpile::const_iterator begin() const override {
    return q_->begin();
  };
  IPackageStockpile::const_iterator end() const override { return q_->end(); };
  IPackageStockpile::const_iterator cbegin() const override {
    return q_->cbegin();
  };
  IPackageStockpile::const_iterator cend() const override {
    return q_->cend();
  };

  TimeOffset get_processing_duration() const { return pd_; };
  Time get_package_processing_start_time() const {return t_;};

private:
  ElementID id_;
  TimeOffset pd_;
  Time t_ = 0;
  std::unique_ptr<IPackageQueue> q_;
  std::optional<Package> processing_buffer_;
};

class Storehouse : public IPackageReceiver {
public:
  Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d)
      : id_(id), d_(std::move(d)) {}

  void receive_package(Package &&p) override { d_->push(std::move(p)); }
  ElementID get_id() const override { return id_; }

  ReceiverType get_receiver_type() const override {
    return ReceiverType::STOREHOUSE;
  };
  IPackageStockpile::const_iterator begin() const override {
    return d_->begin();
  };
  IPackageStockpile::const_iterator end() const override { return d_->end(); };
  IPackageStockpile::const_iterator cbegin() const override {
    return d_->cbegin();
  };
  IPackageStockpile::const_iterator cend() const override {
    return d_->cend();
  };

private:
  ElementID id_;
  std::unique_ptr<IPackageStockpile> d_;
};

#endif /*NODES_HPP_*/
