add_test([=[WriteTest.NoWrap]=]  /Users/marcelleonmrgic/Desktop/ringbuffers/threadsafer_ringbuffer/cmake-build-debug/hello_test [==[--gtest_filter=WriteTest.NoWrap]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[WriteTest.NoWrap]=]  PROPERTIES WORKING_DIRECTORY /Users/marcelleonmrgic/Desktop/ringbuffers/threadsafer_ringbuffer/cmake-build-debug SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  hello_test_TESTS WriteTest.NoWrap)
