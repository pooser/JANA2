cmake_minimum_required(VERSION 3.13)
project(jana2)

set(CMAKE_CXX_STANDARD 14)

# Enable -fPIC for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# For janapy plugin
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(src/lib)
include_directories(src/lib/JANA)
include_directories(src/plugins/JTest)
include_directories(src/programs/jana)
include_directories(src/programs/tests)

add_executable(tests
        src/programs/tests/catch.hpp
        src/programs/tests/ActivableTests.cc
        src/programs/tests/JServiceLocatorTests.cc
        src/programs/tests/JServiceLocatorTests.h
        src/programs/tests/QueueTests.cc
        src/programs/tests/TopologyTests.cc
        src/programs/tests/SchedulerTests.cc
        src/programs/tests/JEventTests.cc
        src/programs/tests/JEventTests.h
        src/programs/tests/PerformanceTests.cc
        src/programs/tests/PerformanceTests.h
        src/programs/tests/TestTopology.cc
        src/programs/tests/TestTopology.h
        src/programs/tests/ExactlyOnceTests.cc
        src/programs/tests/ExactlyOnceTests.h
        src/programs/tests/TerminationTests.cc
        src/programs/tests/TerminationTests.h
        src/programs/tests/UserExceptionTests.cc
        src/programs/tests/UserExceptionTests.h
        )

add_executable(playground
        src/programs/playground/playground.cc
        src/programs/playground/factory_gens_iter_1.cc
        src/programs/playground/Factory.cc
        src/programs/playground/Factory.h
        src/programs/playground/JContext.cc
        src/programs/playground/JContext.h
        src/programs/playground/FactoryGenerator.h
        src/programs/playground/Factory.cc
        src/programs/playground/JEvent2.h
        src/programs/playground/JAbstractFactory.h
        src/programs/playground/JLazyCollection.h
        )


add_library(jana2 STATIC
        src/lib/JANA/JApplication.cc
        src/lib/JANA/JApplication.h
        src/lib/JANA/JEvent.h
        src/lib/JANA/JEventProcessor.h
        src/lib/JANA/JEventSource.h
        src/lib/JANA/JEventSourceGenerator.h
        src/lib/JANA/JEventSourceGeneratorT.h
        src/lib/JANA/JException.h
        src/lib/JANA/JFactory.h
        src/lib/JANA/JFactoryGenerator.h
        src/lib/JANA/JFactorySet.cc
        src/lib/JANA/JFactorySet.h
        src/lib/JANA/JFactoryT.h
        src/lib/JANA/JObject.h
        src/lib/JANA/JSourceFactoryGenerator.h
        src/lib/JANA/JCsvWriter.h
        src/lib/JANA/Engine/JEventProcessorArrow.cc
        src/lib/JANA/Engine/JEventProcessorArrow.h
        src/lib/JANA/Engine/JEventSourceArrow.cc
        src/lib/JANA/Engine/JEventSourceArrow.h
        src/lib/JANA/Engine/JScheduler.cc
        src/lib/JANA/Engine/JWorker.h
        src/lib/JANA/Engine/JWorker.cc
        src/lib/JANA/Engine/JSubeventArrow.cc
        src/lib/JANA/Engine/JSubeventArrow.h
        src/lib/JANA/Engine/JArrowMetrics.h
        src/lib/JANA/Engine/JWorkerMetrics.h
        src/lib/JANA/Engine/JArrowProcessingController.cc
        src/lib/JANA/Engine/JArrowProcessingController.h
        src/lib/JANA/Engine/JArrowPerfSummary.cc
        src/lib/JANA/Engine/JArrowPerfSummary.h
        src/lib/JANA/Services/JParameter.cc
        src/lib/JANA/Services/JParameter.h
        src/lib/JANA/Services/JParameterManager.cc
        src/lib/JANA/Services/JParameterManager.h
        src/lib/JANA/Services/JLoggingService.h
        src/lib/JANA/Services/JServiceLocator.cc
        src/lib/JANA/Services/JProcessingController.h
        src/lib/JANA/Services/JPluginLoader.cc
        src/lib/JANA/Services/JPluginLoader.h
        src/lib/JANA/Status/JStatus.cc
        src/lib/JANA/Status/JStatus.h
        src/lib/JANA/Status/JVersion.h
        src/lib/JANA/Status/JPerfMetrics.cc
        src/lib/JANA/Status/JPerfMetrics.h
        src/lib/JANA/Status/JPerfSummary.h
        src/lib/JANA/Status/JComponentSummary.h
        src/lib/JANA/Status/JComponentSummary.cc
        src/lib/JANA/Utils/JCpuInfo.cc
        src/lib/JANA/Utils/JCpuInfo.h
        src/lib/JANA/Utils/JResettable.h
        src/lib/JANA/Utils/JResourcePool.h
        src/lib/JANA/Utils/JTypeInfo.h
        src/lib/JANA/Utils/JPerfUtils.cc
        src/lib/JANA/Utils/JProcessorMapping.h
        src/lib/JANA/Utils/JProcessorMapping.cc
        src/lib/JANA/Utils/JEventPool.h
        src/lib/JANA/Services/JComponentManager.cc
        src/lib/JANA/Services/JComponentManager.h
        src/lib/JANA/Engine/JArrowTopology.cc
        src/lib/JANA/Engine/JArrowTopology.h
        src/lib/JANA/JLogger.h
        src/lib/JANA/Utils/JBacktrace.h)


add_library(jtest SHARED
        src/plugins/JTest/JTestMain.cc
        src/plugins/JTest/JTestDataObjects.h
        src/plugins/JTest/JTestParser.h
        src/plugins/JTest/JTestDisentangler.h
        src/plugins/JTest/JTestTracker.h
        src/plugins/JTest/JTestPlotter.h
        src/plugins/JTest/JTestFactoryGenerator.h
        src/plugins/JTest/JTestCalibrationService.h
        )

add_library(janapy SHARED
        src/plugins/janapy/janapy.cc
        )

add_executable(jana
        src/programs/jana/jana.cc
        src/programs/jana/JBenchmarker.h
        src/programs/jana/JBenchmarker.cc
        src/programs/jana/JSignalHandler.h
        src/programs/jana/MyProcessor.cc
        src/programs/jana/MyProcessor.h
        )


# Add jana static lib
target_link_libraries(jana jana2)
target_link_libraries(jtest jana2)
target_link_libraries(janapy jana2)
target_link_libraries(tests jana2)
target_link_libraries(playground jana2)

# Add libdl
target_link_libraries(jana2 ${CMAKE_DL_LIBS})

# Add pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(jana2 Threads::Threads)
target_link_libraries(jana Threads::Threads)
target_link_libraries(jtest Threads::Threads)
target_link_libraries(playground Threads::Threads)
target_link_libraries(tests Threads::Threads)

# janpy plugin needs libpython
target_link_libraries(janapy ${PYTHON_LIBRARIES})

# Remove "lib" prefix from plugins
set_target_properties(jtest PROPERTIES PREFIX "" SUFFIX ".so")
set_target_properties(janapy PROPERTIES PREFIX "")



