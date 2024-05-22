cc_library(
    name = "heapsort",
    copts = [
      "-std=c++20",
      "-fprofile-arcs",
      "-ftest-coverage"
    ],
    hdrs = [
			"src/heapsort.hpp",
      "src/rearrange_digits.hpp"
		], 
    srcs = [
      "src/heapsort.cpp",
      "src/rearrange_digits.cpp"
    ],
    linkopts = [
		"-coverage",
		"-fprofile-arcs",
		"-ftest-coverage"
  ],
)

cc_binary(
	name = "heap",
	copts = [
    "-std=c++20",
    "-fprofile-arcs",
    "-ftest-coverage"
  ],
	srcs = [
		"src/main.cpp",
	],
  deps = [
    ":heapsort",
	]
)

cc_test(
  name = "heap_test",
  size = "small",
	copts = [
    "-std=c++20",
    "-fprofile-arcs",
    "-ftest-coverage",
    "-fprofile-instr-generate",
    "-fcoverage-mapping",
  ],
  linkopts = [
		"-coverage",
		"-fprofile-arcs",
		"-ftest-coverage"
  ],
  srcs = ["src/heapsort_test.cpp"],
  deps = [
    ":heapsort",
		"@com_google_googletest//:gtest",
		"@com_google_googletest//:gtest_main"
	],
)