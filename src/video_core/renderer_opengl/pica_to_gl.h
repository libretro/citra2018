// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <cstddef>
#include <glad/glad.h>
#include "common/assert.h"
#include "common/common_types.h"
#include "common/logging/log.h"
#include "core/core.h"
#include "video_core/regs_framebuffer.h"
#include "video_core/regs_lighting.h"
#include "video_core/regs_texturing.h"

using GLvec2 = std::array<GLfloat, 2>;
using GLvec3 = std::array<GLfloat, 3>;
using GLvec4 = std::array<GLfloat, 4>;

using GLuvec2 = std::array<GLuint, 2>;
using GLuvec3 = std::array<GLuint, 3>;
using GLuvec4 = std::array<GLuint, 4>;

using GLivec2 = std::array<GLint, 2>;
using GLivec3 = std::array<GLint, 3>;
using GLivec4 = std::array<GLint, 4>;

namespace PicaToGL {

inline GLenum TextureFilterMode(Pica::TexturingRegs::TextureConfig::TextureFilter mode) {
    static constexpr std::array<GLenum, 2> filter_mode_table{{
        GL_NEAREST, // TextureFilter::Nearest
        GL_LINEAR,  // TextureFilter::Linear
    }};

    const auto index = static_cast<std::size_t>(mode);

    // Range check table for input
    if (index >= filter_mode_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown texture filtering mode {}", index);
        UNREACHABLE();

        return GL_LINEAR;
    }

    GLenum gl_mode = filter_mode_table[index];

    // Check for dummy values indicating an unknown mode
    if (gl_mode == 0) {
        LOG_CRITICAL(Render_OpenGL, "Unknown texture filtering mode {}", index);
        UNIMPLEMENTED();

        return GL_LINEAR;
    }

    return gl_mode;
}

inline GLenum WrapMode(Pica::TexturingRegs::TextureConfig::WrapMode mode) {
    static constexpr std::array<GLenum, 8> wrap_mode_table{{
        GL_CLAMP_TO_EDGE,   // WrapMode::ClampToEdge
        GL_CLAMP_TO_BORDER, // WrapMode::ClampToBorder
        GL_REPEAT,          // WrapMode::Repeat
        GL_MIRRORED_REPEAT, // WrapMode::MirroredRepeat
        // TODO(wwylele): ClampToEdge2 and ClampToBorder2 are not properly implemented here. See the
        // comments in enum WrapMode.
        GL_CLAMP_TO_EDGE,   // WrapMode::ClampToEdge2
        GL_CLAMP_TO_BORDER, // WrapMode::ClampToBorder2
        GL_REPEAT,          // WrapMode::Repeat2
        GL_REPEAT,          // WrapMode::Repeat3
    }};

    const auto index = static_cast<std::size_t>(mode);

    // Range check table for input
    if (index >= wrap_mode_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown texture wrap mode {}", index);
        UNREACHABLE();

        return GL_CLAMP_TO_EDGE;
    }

    if (index > 3) {
        LOG_WARNING(Render_OpenGL, "Using texture wrap mode {}", index);
    }

    GLenum gl_mode = wrap_mode_table[index];

    // Check for dummy values indicating an unknown mode
    if (gl_mode == 0) {
        LOG_CRITICAL(Render_OpenGL, "Unknown texture wrap mode {}", index);
        UNIMPLEMENTED();

        return GL_CLAMP_TO_EDGE;
    }

    return gl_mode;
}

inline GLenum BlendEquation(Pica::FramebufferRegs::BlendEquation equation) {
    static constexpr std::array<GLenum, 5> blend_equation_table{{
        GL_FUNC_ADD,              // BlendEquation::Add
        GL_FUNC_SUBTRACT,         // BlendEquation::Subtract
        GL_FUNC_REVERSE_SUBTRACT, // BlendEquation::ReverseSubtract
        GL_MIN,                   // BlendEquation::Min
        GL_MAX,                   // BlendEquation::Max
    }};

    const auto index = static_cast<std::size_t>(equation);

    // Range check table for input
    if (index >= blend_equation_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown blend equation {}", index);

        // This return value is hwtested, not just a stub
        return GL_FUNC_ADD;
    }

    return blend_equation_table[index];
}

inline GLenum BlendFunc(Pica::FramebufferRegs::BlendFactor factor) {
    static constexpr std::array<GLenum, 15> blend_func_table{{
        GL_ZERO,                     // BlendFactor::Zero
        GL_ONE,                      // BlendFactor::One
        GL_SRC_COLOR,                // BlendFactor::SourceColor
        GL_ONE_MINUS_SRC_COLOR,      // BlendFactor::OneMinusSourceColor
        GL_DST_COLOR,                // BlendFactor::DestColor
        GL_ONE_MINUS_DST_COLOR,      // BlendFactor::OneMinusDestColor
        GL_SRC_ALPHA,                // BlendFactor::SourceAlpha
        GL_ONE_MINUS_SRC_ALPHA,      // BlendFactor::OneMinusSourceAlpha
        GL_DST_ALPHA,                // BlendFactor::DestAlpha
        GL_ONE_MINUS_DST_ALPHA,      // BlendFactor::OneMinusDestAlpha
        GL_CONSTANT_COLOR,           // BlendFactor::ConstantColor
        GL_ONE_MINUS_CONSTANT_COLOR, // BlendFactor::OneMinusConstantColor
        GL_CONSTANT_ALPHA,           // BlendFactor::ConstantAlpha
        GL_ONE_MINUS_CONSTANT_ALPHA, // BlendFactor::OneMinusConstantAlpha
        GL_SRC_ALPHA_SATURATE,       // BlendFactor::SourceAlphaSaturate
    }};

    const auto index = static_cast<std::size_t>(factor);

    // Range check table for input
    if (index >= blend_func_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown blend factor {}", index);
        UNREACHABLE();

        return GL_ONE;
    }

    return blend_func_table[index];
}

inline GLenum LogicOp(Pica::FramebufferRegs::LogicOp op) {
    static constexpr std::array<GLenum, 16> logic_op_table{{
        GL_CLEAR,         // Clear
        GL_AND,           // And
        GL_AND_REVERSE,   // AndReverse
        GL_COPY,          // Copy
        GL_SET,           // Set
        GL_COPY_INVERTED, // CopyInverted
        GL_NOOP,          // NoOp
        GL_INVERT,        // Invert
        GL_NAND,          // Nand
        GL_OR,            // Or
        GL_NOR,           // Nor
        GL_XOR,           // Xor
        GL_EQUIV,         // Equiv
        GL_AND_INVERTED,  // AndInverted
        GL_OR_REVERSE,    // OrReverse
        GL_OR_INVERTED,   // OrInverted
    }};

    const auto index = static_cast<std::size_t>(op);

    // Range check table for input
    if (index >= logic_op_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown logic op {}", index);
        UNREACHABLE();

        return GL_COPY;
    }

    return logic_op_table[index];
}

inline GLenum CompareFunc(Pica::FramebufferRegs::CompareFunc func) {
    static constexpr std::array<GLenum, 8> compare_func_table{{
        GL_NEVER,    // CompareFunc::Never
        GL_ALWAYS,   // CompareFunc::Always
        GL_EQUAL,    // CompareFunc::Equal
        GL_NOTEQUAL, // CompareFunc::NotEqual
        GL_LESS,     // CompareFunc::LessThan
        GL_LEQUAL,   // CompareFunc::LessThanOrEqual
        GL_GREATER,  // CompareFunc::GreaterThan
        GL_GEQUAL,   // CompareFunc::GreaterThanOrEqual
    }};

    const auto index = static_cast<std::size_t>(func);

    // Range check table for input
    if (index >= compare_func_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown compare function {}", index);
        UNREACHABLE();

        return GL_ALWAYS;
    }

    return compare_func_table[index];
}

inline GLenum StencilOp(Pica::FramebufferRegs::StencilAction action) {
    static constexpr std::array<GLenum, 8> stencil_op_table{{
        GL_KEEP,      // StencilAction::Keep
        GL_ZERO,      // StencilAction::Zero
        GL_REPLACE,   // StencilAction::Replace
        GL_INCR,      // StencilAction::Increment
        GL_DECR,      // StencilAction::Decrement
        GL_INVERT,    // StencilAction::Invert
        GL_INCR_WRAP, // StencilAction::IncrementWrap
        GL_DECR_WRAP, // StencilAction::DecrementWrap
    }};

    const auto index = static_cast<std::size_t>(action);

    // Range check table for input
    if (index >= stencil_op_table.size()) {
        LOG_CRITICAL(Render_OpenGL, "Unknown stencil op {}", index);
        UNREACHABLE();

        return GL_KEEP;
    }

    return stencil_op_table[index];
}

inline GLvec4 ColorRGBA8(const u32 color) {
    return {{
        (color >> 0 & 0xFF) / 255.0f,
        (color >> 8 & 0xFF) / 255.0f,
        (color >> 16 & 0xFF) / 255.0f,
        (color >> 24 & 0xFF) / 255.0f,
    }};
}

inline std::array<GLfloat, 3> LightColor(const Pica::LightingRegs::LightColor& color) {
    return {{
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
    }};
}

} // namespace PicaToGL
