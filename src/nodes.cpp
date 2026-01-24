#include "nodes.hxx"

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
  if (!r)
    return;
  preferences_[r] = 0.0f;
  normalize();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
  preferences_.erase(r);
  normalize();
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
  if (preferences_.empty())
    return nullptr;

  double p = probability_generator();
  double cumulative = 0.0;
  for (auto &[receiver, prob] : preferences_) {
    cumulative += prob;
    if (p <= cumulative)
      return receiver;
  }
  return preferences_.rbegin()->first;
}

void ReceiverPreferences::normalize() {
  if (preferences_.empty())
    return;

  float sum = 0.0f;
  for (auto &[_, prob] : preferences_)
    sum += prob;
  if (sum == 0.0f) {
    float equal_prob = 1.0f / preferences_.size();
    for (auto &[_, prob] : preferences_)
      prob = equal_prob;
  } else {
    for (auto &[_, prob] : preferences_)
      prob /= sum;
  }
}

void PackageSender::send_package() {
  if (!buffer_) {
    return;
  }
  IPackageReceiver *receiver = receiver_preferences_.choose_receiver();
  receiver->receive_package(std::move(*buffer_));
  buffer_.reset();
}

void Ramp::deliver_goods(Time t) {
  if (!buffer_) {
    push_package(Package());
    buffer_.emplace(id_);
    t_ = t;
  } else if (t - di_ == t_) {
    push_package(Package());
  }
}

void Worker::receive_package(Package &&p) { q_->push(std::move(p)); }

void Worker::do_work(Time t) {
  if (!buffer_.has_value() && !q_->empty()) {
    t_ = t;
    buffer_.emplace(q_->pop());
  }

  if (t - t_ + 1 == pd_) {
    push_package(Package(buffer_.value().get_id()));
    buffer_.reset();
  }
}
