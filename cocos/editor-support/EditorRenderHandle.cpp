/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
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

#include "EditorRenderHandle.h"
#include "renderer/scene/ModelBatcher.hpp"
#include "EditorManager.h"

using namespace cocos2d;
using namespace cocos2d::renderer;
using namespace editor;

namespace editor {
    
    EditorRenderHandle::EditorRenderHandle()
    {
        _editorMgr = EditorManager::getInstance();
        _useModel = true;
    }
    
    EditorRenderHandle::~EditorRenderHandle()
    {
        for (std::size_t i = 0, n = _iaPool.size(); i < n; i++)
        {
            auto ia = _iaPool[i];
            delete ia;
        }
        _iaPool.clear();
    }
    
    void EditorRenderHandle::updateIA(std::size_t index, int start, int count)
    {
        auto size = _iaPool.size();
        InputAssembler* ia = nullptr;
        if (index == size) {
            ia = new InputAssembler();
            _iaPool.push_back(ia);
        }
        else if (index < size)
        {
            ia = _iaPool[index];
        }
        else
        {
            cocos2d::log("EditorRenderHandle:updateIA index:%lu is out of range", index);
            return;
        }
        
        auto newIACount = index + 1;
        if (_iaCount < newIACount)
        {
            _iaCount = newIACount;
        }
        
        ia->setVertexBuffer(_editorMgr->getVB());
        ia->setIndexBuffer(_editorMgr->getIB());
        ia->setCount(count);
        ia->setStart(start);
    }
    
    void EditorRenderHandle::renderIA(std::size_t index, ModelBatcher* batcher)
    {
        if (index >= _iaCount)
        {
            cocos2d::log("EditorRenderHandle:renderIA index:%lu out of range", index);
            return;
        }
        
        batcher->flushIA(_iaPool[index]);
    }
}
