#include "simulation.hxx"
#include <stdexcept>

void simulate(Factory& f, TimeOffset duration, std::function<void(Factory&, Time)> rf) {
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


SpecificTurnsReportNotifier::SpecificTurnsReportNotifier(std::set<Time> turns)
    : turns_(std::move(turns)) {
}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) const {
    return turns_.find(t) != turns_.end();
}


IntervalReportNotifier::IntervalReportNotifier(TimeOffset interval)
    : interval_(interval) {
}

bool IntervalReportNotifier::should_generate_report(Time t) const {
    if (interval_ == 0) return false; 
    return (t - 1) % interval_ == 0;
}