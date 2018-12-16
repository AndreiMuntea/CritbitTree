#ifndef __CRITBIT_TREE_HPP__ 
#define __CRITBIT_TREE_HPP__ 

#include "unicode_string.h"

class CritBitNode
{
    friend class CritBitTree;
public:
    CritBitNode();
    ~CritBitNode();

private:
    BYTE    Mask;
    USHORT  ByteIndex;
    void*   Children[2];
};

class CritBitTree
{
public:
    CritBitTree() = default;

    ~CritBitTree();

    void  InsertNode(PUNICODE_STRING Node);
    void  InsertNode(PUNICODE_STRING Node, BYTE Direction, BYTE Mask, USHORT ByteIndex);
    void* FindBestMatch(PUNICODE_STRING Node);
    bool  HasDivergentPoint(PUNICODE_STRING Node, USHORT& Index, BYTE& Mask, BYTE& Direction);

    bool IsNodeInternal(void* Node);
    bool IsNodeExternal(void* Node);

    BYTE GetDirection(PUNICODE_STRING Node, USHORT Index, BYTE Mask);

    void* TagNode(void* Node);
    void* UntagNode(void* Node);

    bool IsTreeEmpty();
    CritBitNode* NewNode(PUNICODE_STRING Node, BYTE Direction, BYTE Mask, USHORT ByteIndex);

    void BFS();

private:
    void* root = nullptr;
};

#endif //__CRITBIT_TREE_HPP__