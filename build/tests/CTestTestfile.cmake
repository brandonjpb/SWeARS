# CMake generated Testfile for 
# Source directory: C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests
# Build directory: C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(AllTests "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/build/tests/Debug/SWeARS_Tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;23;add_test;C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(AllTests "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/build/tests/Release/SWeARS_Tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;23;add_test;C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(AllTests "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/build/tests/MinSizeRel/SWeARS_Tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;23;add_test;C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(AllTests "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/build/tests/RelWithDebInfo/SWeARS_Tests.exe")
  set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;23;add_test;C:/Users/brand/OneDrive/Desktop/Programs/CIS-25/SWeARS2.0/tests/CMakeLists.txt;0;")
else()
  add_test(AllTests NOT_AVAILABLE)
endif()
subdirs("../_deps/googletest-build")
