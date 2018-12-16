#ifndef __CRITBIT_TREE_HPP__ 
#define __CRITBIT_TREE_HPP__ 

#include "unicode_string.h"

class CritBitNode
{
    friend class CritBitTree;
public:
    CritBitNode()  = default;
    ~CritBitNode() = default;

private:
    BYTE    Mask        = 0;
    USHORT  ByteIndex   = 0;
    void*   Children[2] = { nullptr, nullptr };
};

class CritBitTree
{
public:
    CritBitTree() = default;
    ~CritBitTree();

    void InsertNode(PUNICODE_STRING Node);
    bool IsTreeEmpty();
    void BFS();

private:
    void* root = nullptr;

    bool IsNodeInternal(void* Node);
    bool IsNodeExternal(void* Node);    
    void* TagNode(void* Node);
    CritBitNode* UntagNode(void* Node);
    BYTE GetDirection(PUNICODE_STRING Node, USHORT Index, BYTE Mask);
    void InsertNode(PUNICODE_STRING Node, BYTE Direction, BYTE Mask, USHORT ByteIndex);
    CritBitNode* NewNode(PUNICODE_STRING Node, BYTE Direction, BYTE Mask, USHORT ByteIndex);
    bool HasDivergentPoint(PUNICODE_STRING Node, USHORT& Index, BYTE& Mask, BYTE& Direction);
    PUNICODE_STRING FindBestMatch(PUNICODE_STRING Node);
};

#endif //__CRITBIT_TREE_HPP__