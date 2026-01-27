#include "simulation.hxx"
#include <stdexcept>

void simulate(Factory &f, TimeOffset duration,
              std::function<void(Factory &, Time)> rf) {
  if (!f.is_consistent()) {
    throw std::logic_error("Network is inconsistent");
  }

  for (Time t = 1; t <= duration; ++t) {

    f.do_deliveries(t);
    f.do_package_passing();
    f.do_work(t);

    rf(f, t);
  }
}

