// 
//   Copyright (C) 2018 BitDefender S.R.L. 
//   Author(s)    : ANDREI-MARIUS MUNTEA (amuntea@bitdefender.com) 
//

#include "critbit.hpp"
#include <cassert>
#include <iostream>

void TestAll();
void TestTagging();
void TestUntagging();
void TestIsInternal();
void TestIsExternal();
void TestInsert1();

int main()
{
    TestAll();
    return 0;
}


void TestAll()
{
    TestTagging();
    TestUntagging();
    TestIsInternal();
    TestIsExternal();
    TestInsert1();
}

void TestTagging()
{
    CritBitTree tree;
    size_t tag = 0x190;
    auto result = tree.TagNode((void*)(tag));

    assert((size_t)(result) == tag + 1);
}

void TestUntagging()
{
    CritBitTree tree;
    size_t tag = 0x171;
    auto result = tree.UntagNode((void*)(tag));

    assert((size_t)(result) == tag - 1);
}

void TestIsInternal()
{
    CritBitTree tree;
    size_t externalNode = 0x1A0;
    size_t internalNode = 0x1F1;
    assert(tree.IsNodeInternal((void*)(internalNode)));
    assert(!tree.IsNodeInternal((void*)(externalNode)));
}
void TestIsExternal()
{
    CritBitTree tree;
    size_t externalNode = 0x120;
    size_t internalNode = 0x151;
    assert(!tree.IsNodeExternal((void*)(internalNode)));
    assert(tree.IsNodeExternal((void*)(externalNode)));
}

void TestInsert1()
{
    CritBitTree tree;

    wchar_t str1[] = L"ana";        UNICODE_STRING ustr1 = { (USHORT)(wcslen(str1) * 2), (USHORT)(wcslen(str1) * 2), (PWCHAR)(str1) };
    wchar_t str2[] = L"anab";       UNICODE_STRING ustr2 = { (USHORT)(wcslen(str2) * 2), (USHORT)(wcslen(str2) * 2), (PWCHAR)(str2) };
    wchar_t str3[] = L"anbc";       UNICODE_STRING ustr3 = { (USHORT)(wcslen(str3) * 2), (USHORT)(wcslen(str3) * 2), (PWCHAR)(str3) };
    wchar_t str4[] = L"abbc";       UNICODE_STRING ustr4 = { (USHORT)(wcslen(str4) * 2), (USHORT)(wcslen(str4) * 2), (PWCHAR)(str4) };
    wchar_t str5[] = L"bnbc";       UNICODE_STRING ustr5 = { (USHORT)(wcslen(str5) * 2), (USHORT)(wcslen(str5) * 2), (PWCHAR)(str5) };


    tree.InsertNode(&ustr1);
    tree.InsertNode(&ustr2);
    tree.InsertNode(&ustr3);
    tree.InsertNode(&ustr4);
    tree.InsertNode(&ustr5);
    tree.BFS();

}