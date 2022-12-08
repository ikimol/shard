// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <benchpress.hpp>

void run_benchmarks(const benchpress::options& opts) {
    std::regex match_r(opts.get_bench());
    auto benchmarks = benchpress::registration::get_ptr()->get_benchmarks();
    for (auto& info : benchmarks) {
        if (std::regex_match(info.get_name(), match_r)) {
            benchpress::context c(info, opts);
            auto r = c.run();
            std::cout << std::setw(35) << std::left << info.get_name() << r.to_string() << std::endl;
        }
    }
}

benchpress::registration* benchpress::registration::d_this;

int main(int /* argc */, char* argv[]) {
    using clock_type = std::chrono::high_resolution_clock;
    auto start = clock_type::now();
    benchpress::options opts;
    run_benchmarks(opts);
    auto d = std::chrono::duration_cast<std::chrono::milliseconds>(clock_type::now() - start).count() / 1000.f;
    std::cout << argv[0] << " " << d << "s" << std::endl;
    return 0;
}
