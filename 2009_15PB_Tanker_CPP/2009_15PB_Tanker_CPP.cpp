// 2009_15PB_Tanker_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 本项目开源，见开源地址 https://github.com/mhw666/2009_15PB_Tanker_CPP

#include "CCtrl.h"
int main()
{
    const char* buf = "初始化游戏中...请稍后...";
    SetConsoleTitleA(buf);
    std::cout << buf << endl;


    CCtrl GAME({ 120,34 });
    GAME.InitCMD();
}