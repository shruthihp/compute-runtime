/*
 * Copyright (c) 2018, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "runtime/helpers/ptr_math.h"
#include "runtime/mem_obj/map_operations_handler.h"
#include "test.h"

#include <tuple>

using namespace OCLRT;

struct MockMapOperationsHandler : public MapOperationsHandler {
    using MapOperationsHandler::isOverlapping;
    using MapOperationsHandler::mappedPointers;
};

struct MapOperationsHandlerTests : public ::testing::Test {
    MockMapOperationsHandler mockHandler;
    MapInfo mappedPtrs[3] = {
        {(void *)0x1000, 1, {{1, 2, 3}}, {{4, 5, 6}}},
        {(void *)0x2000, 1, {{7, 8, 9}}, {{10, 11, 12}}},
        {(void *)0x3000, 1, {{13, 14, 15}}, {{16, 17, 18}}},
    };
    cl_map_flags mapFlags = CL_MAP_READ;
};

TEST_F(MapOperationsHandlerTests, givenMapInfoWhenFindingThenReturnCorrectvalues) {
    for (size_t i = 0; i < 3; i++) {
        EXPECT_TRUE(mockHandler.add(mappedPtrs[i].ptr, mappedPtrs[i].ptrLength, mapFlags, mappedPtrs[i].size, mappedPtrs[i].offset));
    }
    EXPECT_EQ(3u, mockHandler.size());

    for (int i = 2; i >= 0; i--) {
        MapInfo receivedMapInfo;
        EXPECT_TRUE(mockHandler.find(mappedPtrs[i].ptr, receivedMapInfo));
        EXPECT_EQ(receivedMapInfo.ptr, mappedPtrs[i].ptr);
        EXPECT_EQ(receivedMapInfo.size, mappedPtrs[i].size);
        EXPECT_EQ(receivedMapInfo.offset, mappedPtrs[i].offset);
    }
}

TEST_F(MapOperationsHandlerTests, givenMapInfoWhenRemovingThenRemoveCorrectPointers) {
    for (size_t i = 0; i < 3; i++) {
        mockHandler.add(mappedPtrs[i].ptr, mappedPtrs[i].ptrLength, mapFlags, mappedPtrs[i].size, mappedPtrs[i].offset);
    }

    for (int i = 2; i >= 0; i--) {
        mockHandler.remove(mappedPtrs[i].ptr);
        MapInfo receivedMapInfo;
        EXPECT_FALSE(mockHandler.find(mappedPtrs[i].ptr, receivedMapInfo));
    }
    EXPECT_EQ(0u, mockHandler.size());
}

TEST_F(MapOperationsHandlerTests, givenMappedPtrsWhenDoubleRemovedThenDoNothing) {
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);
    mockHandler.add(mappedPtrs[1].ptr, mappedPtrs[1].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);

    EXPECT_EQ(2u, mockHandler.size());
    mockHandler.remove(mappedPtrs[1].ptr);
    mockHandler.remove(mappedPtrs[1].ptr);
    EXPECT_EQ(1u, mockHandler.size());

    MapInfo receivedMapInfo;
    EXPECT_FALSE(mockHandler.find(mappedPtrs[1].ptr, receivedMapInfo));
    EXPECT_TRUE(mockHandler.find(mappedPtrs[0].ptr, receivedMapInfo));
}

TEST_F(MapOperationsHandlerTests, givenMapInfoWhenAddedThenSetReadOnlyFlag) {
    mapFlags = CL_MAP_READ;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);
    EXPECT_TRUE(mockHandler.mappedPointers.back().readOnly);
    mockHandler.remove(mappedPtrs[0].ptr);

    mapFlags = CL_MAP_WRITE;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);
    EXPECT_FALSE(mockHandler.mappedPointers.back().readOnly);
    mockHandler.remove(mappedPtrs[0].ptr);

    mapFlags = CL_MAP_WRITE_INVALIDATE_REGION;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);
    EXPECT_FALSE(mockHandler.mappedPointers.back().readOnly);
    mockHandler.remove(mappedPtrs[0].ptr);

    mapFlags = CL_MAP_READ | CL_MAP_WRITE;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);
    EXPECT_FALSE(mockHandler.mappedPointers.back().readOnly);
    mockHandler.remove(mappedPtrs[0].ptr);

    mapFlags = CL_MAP_READ | CL_MAP_WRITE_INVALIDATE_REGION;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);
    EXPECT_FALSE(mockHandler.mappedPointers.back().readOnly);
    mockHandler.remove(mappedPtrs[0].ptr);
}

TEST_F(MapOperationsHandlerTests, givenNonReadOnlyOverlappingPtrWhenAddingThenReturnFalseAndDontAdd) {
    mapFlags = CL_MAP_WRITE;
    mappedPtrs->readOnly = false;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);

    EXPECT_EQ(1u, mockHandler.size());
    EXPECT_FALSE(mockHandler.mappedPointers.back().readOnly);
    EXPECT_TRUE(mockHandler.isOverlapping(mappedPtrs[0]));
    EXPECT_FALSE(mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset));
    EXPECT_EQ(1u, mockHandler.size());
}

TEST_F(MapOperationsHandlerTests, givenReadOnlyOverlappingPtrWhenAddingThenReturnTrueAndAdd) {
    mapFlags = CL_MAP_READ;
    mappedPtrs->readOnly = true;
    mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset);

    EXPECT_EQ(1u, mockHandler.size());
    EXPECT_TRUE(mockHandler.mappedPointers.back().readOnly);
    EXPECT_FALSE(mockHandler.isOverlapping(mappedPtrs[0]));
    EXPECT_TRUE(mockHandler.add(mappedPtrs[0].ptr, mappedPtrs[0].ptrLength, mapFlags, mappedPtrs[0].size, mappedPtrs[0].offset));
    EXPECT_EQ(2u, mockHandler.size());
    EXPECT_TRUE(mockHandler.mappedPointers.back().readOnly);
}

const std::tuple<void *, size_t, void *, size_t, bool> overlappingCombinations[] = {
    // mappedPtrStart, mappedPtrLength, requestPtrStart, requestPtrLength, expectOverlap
    std::make_tuple((void *)5000, 50, (void *)4000, 1, false),  //requested before, non-overlapping
    std::make_tuple((void *)5000, 50, (void *)4999, 10, true),  //requested before, overlapping inside
    std::make_tuple((void *)5000, 50, (void *)4999, 100, true), //requested before, overlapping outside
    std::make_tuple((void *)5000, 50, (void *)5001, 1, true),   //requested inside, overlapping inside
    std::make_tuple((void *)5000, 50, (void *)5001, 100, true), //requested inside, overlapping outside
    std::make_tuple((void *)5000, 50, (void *)6000, 1, false),  //requested after, non-overlapping
    std::make_tuple((void *)5000, 50, (void *)5000, 1, true),   //requested on start, overlapping inside
    std::make_tuple((void *)5000, 50, (void *)5000, 100, true), //requested on start, overlapping outside
};

struct MapOperationsHandlerOverlapTests : public ::testing::WithParamInterface<std::tuple<void *, size_t, void *, size_t, bool>>,
                                          public ::testing::Test {};

TEST_P(MapOperationsHandlerOverlapTests, givenAlreadyMappedPtrWhenAskingForOverlapThenReturnCorrectValue) {
    cl_map_flags mapFlags = CL_MAP_WRITE;
    void *mappedPtr = std::get<0>(GetParam());
    size_t mappedPtrLength = std::get<1>(GetParam());
    void *requestedPtr = std::get<2>(GetParam());
    size_t requestedPtrLength = std::get<3>(GetParam());
    bool expectOverlap = std::get<4>(GetParam());

    // size and offset arrays are ignored
    MapInfo mappedInfo(mappedPtr, mappedPtrLength, {{0, 0, 0}}, {{0, 0, 0}});
    MapInfo requestedInfo(requestedPtr, requestedPtrLength, {{0, 0, 0}}, {{0, 0, 0}});
    requestedInfo.readOnly = false;

    MockMapOperationsHandler mockHandler;
    mockHandler.add(mappedInfo.ptr, mappedInfo.ptrLength, mapFlags, mappedInfo.size, mappedInfo.offset);

    EXPECT_EQ(expectOverlap, mockHandler.isOverlapping(requestedInfo));
}

INSTANTIATE_TEST_CASE_P(MapOperationsHandlerOverlapTests,
                        MapOperationsHandlerOverlapTests,
                        ::testing::ValuesIn(overlappingCombinations));
