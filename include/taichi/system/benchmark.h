/*******************************************************************************
    Taichi - Physically based Computer Graphics Library

    Copyright (c) 2016 Yuanming Hu <yuanmhu@gmail.com>

    All rights reserved. Use of this source code is governed by
    the MIT license as written in the LICENSE file.
*******************************************************************************/

#pragma once

#include <taichi/common/meta.h>
#include <taichi/system/timer.h>

TC_NAMESPACE_BEGIN

    class Benchmark : public Unit {
    protected:
        int dummy;
        int warm_up_iterations;
        int64 workload;
        bool returns_time;
        virtual void setup() {};
        virtual void iterate() = 0;
        virtual void finalize() {};
    public:
        virtual void initialize(const Config &config) override {
            warm_up_iterations = config.get("warm_up_iterations", 16);
            workload = config.get("workload", 1024LL);
            returns_time = config.get("returns_time", false);
        }
        virtual real run(int iterations=16) {
            setup();
            for (int i = 0; i < warm_up_iterations; i++) {
                iterate();
            }
            double start_t;
            if (returns_time)
                start_t = Time::get_time();
            else
                start_t = (double)Time::get_cycles();
            for (int i = 0; i < iterations; i++) {
                iterate();
            }
            double end_t;
            if (returns_time)
                end_t = Time::get_time();
            else
                end_t = (double)Time::get_cycles();
            real elapsed = (real)(end_t - start_t);
            finalize();
            return elapsed / (iterations * workload);
        }
        virtual bool test() const override {
            return true;
        }
    };
    TC_INTERFACE(Benchmark)


TC_NAMESPACE_END
