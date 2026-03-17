#pragma once
#include <stdint.h>

// Defined in linker scripts:
extern uint32_t TTB_[];
extern uint32_t TTB_SZ_[];
extern uint32_t TTB_END_[];

extern uint32_t A7_CODE_[];
extern uint32_t A7_CODE_SZ_[];
extern uint32_t A7_CODE_END_[];

extern uint32_t A7_HEAP_[];
extern uint32_t A7_HEAP_SZ_[];
extern uint32_t A7_HEAP_END_[];

extern uint32_t A7_RAM_[];
extern uint32_t A7_RAM_SZ_[];
extern uint32_t A7_RAM_END_[];

extern uint32_t DMABUF_[];
extern uint32_t DMABUF_SZ_[];
extern uint32_t DMABUF_END_[];

// Helpers for C/C++ code:
static const uint32_t TTB = (uint32_t)TTB_;

static const uint32_t A7_CODE = (uint32_t)A7_CODE_;
static const uint32_t A7_CODE_SZ = (uint32_t)A7_CODE_SZ_;
static const uint32_t A7_CODE_END = (uint32_t)A7_CODE_END_;

static const uint32_t A7_HEAP = (uint32_t)A7_HEAP_;
static const uint32_t A7_HEAP_SZ = (uint32_t)A7_HEAP_SZ_;
static const uint32_t A7_HEAP_END = (uint32_t)A7_HEAP_END_;

static const uint32_t A7_RAM = (uint32_t)A7_RAM_;
static const uint32_t A7_RAM_SZ = (uint32_t)A7_RAM_SZ_;
static const uint32_t A7_RAM_END = (uint32_t)A7_RAM_END_;

static const uint32_t DMABUF = (uint32_t)DMABUF_;
static const uint32_t DMABUF_SZ = (uint32_t)DMABUF_SZ_;
static const uint32_t DMABUF_END = (uint32_t)DMABUF_END_;
