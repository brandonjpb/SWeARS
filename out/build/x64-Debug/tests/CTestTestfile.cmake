# CMake generated Testfile for 
# Source directory: C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests
# Build directory: C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/out/build/x64-Debug/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/out/build/x64-Debug/tests/SWeARS_Tests.exe")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;23;add_test;C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
