/****************************************************************************
 Copyright (c) 2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#include <stdint.h>

#include "../Macro.h"
#include "../gfx/VertexFormat.h"
#include "../gfx/VertexBuffer.h"
#include "../gfx/IndexBuffer.h"

RENDERER_BEGIN

class ModelBatcher;

/**
 * @addtogroup scene
 * @{
 */

/**
 *  @brief The buffer which stores mesh render datas, including the vertices data and the indices data.
 *  It can be used as a global buffer shared by multiple render handles and eventually shared by Models
 */
class MeshBuffer
{
public:
    /**
     *  @brief It describes a range of buffer in the global buffer, it contains result when you request the buffer.
     */
    struct OffsetInfo
    {
        /** bytes count of the requested buffer */
        uint32_t vByte;
        /** offset in index buffer */
        uint32_t index;
        /** offset in vertex buffer */
        uint32_t vertex;
    };
    
    /**
     *  @brief Constructor
     *  @param[in] batcher The ModelBatcher which creates the current buffer
     *  @param[in] fmt The vertex format of vertex data
     */
    MeshBuffer(ModelBatcher* batcher, VertexFormat* fmt);
    /**
     *  @brief Destructor
     */
    ~MeshBuffer();
    
    /**
     *  @brief Requests a range of buffer for the given count of vertices and indices
     *  @param[in] vertexCount Requested count of vertices
     *  @param[in] indexCount Requested count of indices
     *  @param[out] offset The result indicates the allocated buffer range
     */
    bool request(uint32_t vertexCount, uint32_t indexCount, OffsetInfo* offset);
    bool requestStatic(uint32_t vertexCount, uint32_t indexCount);
    
    /**
     *  @brief Upload data to GPU memory
     */
    void uploadData();
    /**
     *  @brief Reset all states.
     */
    void reset();
    /**
     *  @brief Destroy the mesh buffer.
     */
    void destroy();
    
    /**
     *  @brief Gets the current byte offset which indicates the start of empty range
     *  @return Byte offset.
     */
    uint32_t getByteOffset() const { return _byteOffset; };
    /**
     *  @brief Gets the current vertex start offset since last time updateOffset is invoked
     *  @return Vertex start.
     */
    uint32_t getVertexStart() const { return _vertexStart; };
    /**
     *  @brief Gets the current vertex offset, which should equals to total allocated vertex count.
     *  @return Vertex offset.
     */
    uint32_t getVertexOffset() const { return _vertexOffset; };
    /**
     *  @brief Gets the current index start offset since last time updateOffset is invoked
     *  @return Index start.
     */
    uint32_t getIndexStart() const { return _indexStart; };
    /**
     *  @brief Gets the current index offset, which should equals to total allocated index count.
     *  @return Index offset.
     */
    uint32_t getIndexOffset() const { return _indexOffset; };
    
    /**
     *  @brief Update the current allocated offsets to the start offsets.
     */
    void updateOffset()
    {
        _byteStart = _byteOffset;
        _vertexStart = _vertexOffset;
        _indexStart = _indexOffset;
    };
    
    /**
     *  @brief Gets the vertex buffer.
     */
    VertexBuffer* getVertexBuffer() const { return _vb; };
    /**
     *  @brief Gets the index buffer.
     */
    IndexBuffer* getIndexBuffer() const { return _ib; };
    
    /**
     *  @brief The vertex data storage in memory
     */
    std::vector<float> vData;
    /**
     *  @brief The index data storage in memory
     */
    std::vector<uint16_t> iData;
    /**
     *  @brief Vertex format of the vertex data.
     */
    VertexFormat* _vertexFmt;
    
    static const int INIT_VERTEX_COUNT = 256;
    static const uint8_t VDATA_BYTE = sizeof(float);
    static const uint8_t IDATA_BYTE = sizeof(uint16_t);
protected:
    void reallocBuffers();
    
private:
    uint32_t _byteStart;
    uint32_t _byteOffset;
    uint32_t _indexStart;
    uint32_t _indexOffset;
    uint32_t _vertexStart;
    uint32_t _vertexOffset;
    uint32_t _bytesPerVertex;
    uint32_t _vDataCount;
    uint32_t _iDataCount;
    bool _dirty;
    
    ModelBatcher* _batcher;
    VertexBuffer* _vb;
    IndexBuffer* _ib;
};

// end of scene group
/// @}

RENDERER_END
