#include "critbit.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <bitset>


CritBitTree::~CritBitTree()
{
    /// todo
}

void
CritBitTree::InsertNode(
    PUNICODE_STRING Node
)
{
    USHORT index = 0;
    BYTE mask = 0;
    BYTE direction = 0;

    if (IsTreeEmpty())
    {
        root = (void*)(Node);
        return;
    }

    if (!HasDivergentPoint(Node, index, mask, direction))
    {
        return;
    }

    InsertNode(Node, direction, mask, index);
}

void CritBitTree::InsertNode(PUNICODE_STRING Node, BYTE Direction, BYTE Mask, USHORT ByteIndex)
{
    auto current = &root;
    auto newNode = NewNode(Node, Direction, Mask, ByteIndex);
    if (!newNode)
    {
        return;
    }

    while (IsNodeInternal(*current))
    {
        auto node = (CritBitNode*)UntagNode(*current);

        if (node->ByteIndex > newNode->ByteIndex)
        {
            break;
        }

        if (node->ByteIndex == newNode->ByteIndex && node->Mask > newNode->Mask)
        {
            break;
        }

        auto direction = GetDirection(Node, node->ByteIndex, node->Mask);
        current = &node->Children[direction];
    }

    newNode->Children[1 - Direction] = *current;
    *current = TagNode(newNode);
}

void *
CritBitTree::FindBestMatch(
    PUNICODE_STRING Node
)
{
    void* currentNode = root;

    while (!IsNodeExternal(currentNode))
    {
        auto node = (CritBitNode*)(UntagNode(currentNode));
        auto direction = GetDirection(Node, node->ByteIndex, node->Mask);
        currentNode = node->Children[direction];
    }

    return currentNode;
}

bool
CritBitTree::HasDivergentPoint(
    PUNICODE_STRING Node,
    USHORT&         Index,
    BYTE&           Mask,
    BYTE&           Direction
)
{
    auto bestMatch = (PUNICODE_STRING)(FindBestMatch(Node));
    auto bestMatchBuffer = (BYTE*)(bestMatch->Buffer);
    auto nodeBuffer = (BYTE*)(Node->Buffer);
    auto length = min(Node->Length, bestMatch->Length);

    Index = 0;
    Mask = 0;
    Direction = 0;

    // Find first byte where the 2 strings diverge
    for (Index = 0; Index < length; ++Index)
    {
        if (bestMatchBuffer[Index] != nodeBuffer[Index])
        {
            break;
        }
    }

    // The 2 strings are the same. No divergent point.
    if (Index == bestMatch->Length && Index == Node->Length)
    {
        return false;
    }

    // Mask = bestMatch[idx] ^ node[idx]
    Mask ^= (Index < Node->Length) ? nodeBuffer[Index] : 0;
    Mask ^= (Index < bestMatch->Length) ? bestMatchBuffer[Index] : 0;

    // Find MSB and invert all other bits
    while (Mask & (Mask - 1))
    {
        Mask &= Mask - 1;
    }
    Mask ^= 255;

    Direction = 1 - GetDirection(bestMatch, Index, Mask);
    return true;
}

bool
CritBitTree::IsNodeInternal(
    void * Node
)
{
    size_t node = (size_t)(Node);
    return ((node & 1) == 1);       // Check if last bit is 1
}

bool
CritBitTree::IsNodeExternal(
    void * Node
)
{
    size_t node = (size_t)(Node);
    return ((node & 1) == 0);       // Check if last bit is 0
}

BYTE
CritBitTree::GetDirection(
    PUNICODE_STRING Node,
    USHORT Index,
    BYTE Mask
)
{
    if (Node->Length < Index)       // We consider a string to have an infinity of zeros at the end.
    {
        return 0;
    }

    BYTE* buffer = (BYTE*)(Node->Buffer);
    return ((Mask | buffer[Index]) + 1) >> 8;
}

void*
CritBitTree::TagNode(
    void * Node
)
{
    size_t node = (size_t)(Node);
    return (void*)(node | 1);       // Set last bit
}

void*
CritBitTree::UntagNode(
    void * Node
)
{
    size_t node = (size_t)(Node);
    return (void*)(node - 1);       // Unset last bit
}

bool
CritBitTree::IsTreeEmpty()
{
    return (root == nullptr);
}

CritBitNode*
CritBitTree::NewNode(
    PUNICODE_STRING Node,
    BYTE Direction,
    BYTE Mask,
    USHORT ByteIndex
)
{
    auto node = new CritBitNode();
    if (IsNodeInternal((void*)(node))) // Check node to be aligned 
    {
        delete node;
        return nullptr;
    }

    node->ByteIndex = ByteIndex;
    node->Mask = Mask;
    node->Children[Direction] = (void*)Node;
    return node;
}

void CritBitTree::BFS()
{
    std::vector<void*> q;
    size_t current = 0;

    q.push_back(root);

    while (current < q.size())
    {
        auto c = q[current++];
        if (IsNodeExternal(c)) continue;

        auto node = (CritBitNode*)UntagNode(c);
        q.push_back(node->Children[0]);
        q.push_back(node->Children[1]);
    }

    for (const auto& c : q)
    {
        std::wcout << "================================" << std::endl;
        if (IsNodeInternal(c))
        {
            auto node = (CritBitNode*)UntagNode(c);
            std::wcout << "Address   " << &node << std::endl;
            std::wcout << "ByteIndex " << node->ByteIndex << std::endl;
            std::wcout << "Mask      " << std::bitset<8>(node->Mask) << std::endl;
            std::wcout << "Child 0   " << node->Children[0] << std::endl;
            std::wcout << "Child 1   " << node->Children[1] << std::endl;
        }
        else
        {
            auto node = (PUNICODE_STRING)c;
            std::wcout << "Address   " << node << std::endl;
            std::wcout << "Length    " << node->Length << std::endl;
            std::wcout << "MaxLength " << node->MaximumLength << std::endl;
            std::wcout << "Buffer    " << std::wstring(node->Buffer, node->Length >> 1) << std::endl;
        }

        std::wcout << "================================" << std::endl;
    }
}

CritBitNode::CritBitNode()
{
    this->Children[0] = nullptr;
    this->Children[1] = nullptr;
    this->ByteIndex = 0;
    this->Mask = 0;
}

CritBitNode::~CritBitNode()
{
    this->Children[0] = nullptr;
    this->Children[1] = nullptr;
    this->ByteIndex = 0;
    this->Mask = 0;
}
