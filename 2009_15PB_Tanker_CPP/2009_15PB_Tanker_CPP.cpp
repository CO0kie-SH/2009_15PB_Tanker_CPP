// 2009_15PB_Tanker_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 本项目开源，见开源地址 https://github.com/mhw666/2009_15PB_Tanker_CPP

#include "CCtrl.h"

/*载入进度*/
void Init()
{
    const char* buf = "初始化游戏中...请稍后...";
    int X = 30;
    SetConsoleTitleA(buf);
    system("color 0A");
    system("mode con cols=30 lines=15");
    std::cout << buf << endl;
    cout << "\n\n加载进度：\n";
    for (size_t i = 0; i < X; i++)
    {
        cout << "■" << std::flush;
        Sleep(100);
    }
    cout << "\n加载完毕..." << std::flush;
    Sleep(1000);
}

/*主函数*/
int main()
{
    Init();

    CCtrl GAME({ 120,30 });
    GAME.InitCMD();
}