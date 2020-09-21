// 2009_15PB_Tanker_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "CCtrl.h"

int main()
{
    //std::cout << "Hello World!\n";

    char ccc = 0x00;
    CHARMAP map[2] = { 0 };
    int i = 0xFA;
    int nc = sizeof(ccc);
    int nm = sizeof(map);
    int ni = sizeof(i);
    map[1].WALL = 1;
    //std::cout << (char)map[0].TANKER<<"\n";
    map[0].TANKER = 15;
    //std::cout << (char)map[0].TANKER<<"\n";
    map[0].TANKER++;
    //std::cout << (char)map[0].TANKER<<"\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
