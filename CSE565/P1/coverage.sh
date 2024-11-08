#!/bin/sh
bazel coverage -s --instrument_test_targets --experimental_cc_coverage --combined_report=lcov --coverage_report_generator=@bazel_tools//tools/test/CoverageOutputGenerator/java/com/google/devtools/coverageoutputgenerator:Main //...

genhtml -o coverage_report ./bazel-testlogs/heap_test/coverage.dat

python -m http.server --directory ./coverage_report