# Copyright (c) 2012-2024 Leonid Yuriev <leo@yuriev.ru>.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#

if(NOT DEFINED STDTHREAD_WORKS)
  if(CMAKE_VERSION VERSION_LESS 3.8.2)
    cmake_minimum_required(VERSION 3.0.2)
  elseif(CMAKE_VERSION VERSION_LESS 3.12)
    cmake_minimum_required(VERSION 3.8.2)
  else()
    cmake_minimum_required(VERSION 3.12)
  endif()

  cmake_policy(PUSH)
  cmake_policy(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})
  include(CMakePushCheckState)
  cmake_push_check_state()
  set(stdthread_probe_save_libraries ${CMAKE_REQUIRED_LIBRARIES})
  set(stdthread_probe_save_flags ${CMAKE_REQUIRED_FLAGS})
  set(stdthread_probe_flags ${CMAKE_REQUIRED_FLAGS})
  unset(stdthread_probe_clear_cxx_standard)
  if(NOT DEFINED CMAKE_CXX_STANDARD)
    list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_14 HAS_CXX14)
    list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_17 HAS_CXX17)
    list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_11 HAS_CXX11)
    if(NOT HAS_CXX17 LESS 0
       AND NOT (CMAKE_COMPILER_IS_CLANG AND CMAKE_CXX_COMPILER_VERSION
                                            VERSION_LESS 5))
      set(CMAKE_CXX_STANDARD 17)
    elseif(NOT HAS_CXX14 LESS 0)
      set(CMAKE_CXX_STANDARD 14)
    elseif(NOT HAS_CXX11 LESS 0)
      set(CMAKE_CXX_STANDARD 11)
    elseif(CXX_FALLBACK_GNU11)
      set(stdthread_probe_flags ${stdthread_probe_flags} "-std=gnu++11")
    else()
      set(stdthread_probe_flags ${stdthread_probe_flags} "-std=c++11")
    endif()
    set(stdthread_probe_clear_cxx_standard ON)
  endif()
  set(CMAKE_REQUIRED_FLAGS ${stdthread_probe_flags})

  if(NOT DEFINED THREADS_PREFER_PTHREAD_FLAG)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  endif()
  find_package(Threads)
  if(CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
                                 ${CMAKE_REQUIRED_LIBRARIES})
  else()
    set(CMAKE_REQUIRED_LIBRARIES Threads::Threads ${CMAKE_REQUIRED_LIBRARIES})
  endif()

  set(stdthread_probe
      [[
    #include <thread>
    void thread_proc() {}
    int main() {
    std::thread instance(thread_proc);
    instance.join();
    return 0;
    }
    ]])

  if(CMAKE_CROSSCOMPILING)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("${stdthread_probe}" STDTHREAD_WORKS)
    if(STDTHREAD_WORKS)
      message(
        WARNING
          "Cannot run probe in CrossCompiling mode to check whether `std::thread` works. "
          "So assuming it will works because compiled successfully.")
    endif()
  else()
    include(CheckCXXSourceRuns)
    check_cxx_source_runs("${stdthread_probe}" STDTHREAD_WORKS)
  endif()
  set(CMAKE_REQUIRED_FLAGS ${stdthread_probe_save_flags})
  set(CMAKE_REQUIRED_LIBRARIES ${stdthread_probe_save_libraries})
  if(stdthread_probe_clear_cxx_standard)
    unset(CMAKE_CXX_STANDARD)
  endif()
  unset(stdthread_probe_clear_cxx_standard)
  unset(stdthread_probe_save_libraries)
  unset(stdthread_probe_save_flags)
  unset(stdthread_probe_flags)
  unset(stdthread_probe)
  cmake_pop_check_state()

  set(STDTHREAD_WORKS
      "${STDTHREAD_WORKS}"
      CACHE BOOL "Whether C++11 std::thread works." FORCE)
  cmake_policy(POP)
endif()
