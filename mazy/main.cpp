/*
《动感迷宫》功能要求：
迷宫生成：迷宫为正方形；
      ① 内置：20×20的规模；
      ② 随机生成：
           要求：★迷宫规模可以由用户配置（15×15 ~ 50×50）；
                      ★生成的迷宫必须是有解的；
                      ★入口位置坐标（1, 0）、出口位置坐标（n-1, n）
      ③ 文件导入：用户配置TXT文件，导入生成迷宫（选做）；
迷宫路径求解：
      ① 采用回溯算法求解一条从入口到出口的通路；
      ② 探索生成多条通路：找出所有通路，计算每条路径长度，并选出最短路径（选做）；
迷宫显示：
      ① 显示符号：墙■；空□；入口位置/出口位置〓；当前位置㊣；不可通位置☒；当前路径→↓←↑；
      ② 动态显示迷宫路径探索过程：每探索一步，刷新一次迷宫显示；
           注意：★当前路径的箭头方向问题；
                 ★屏幕刷新时的闪屏问题；
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <cassert>
#include "mystack.h"
#include <windows.h>
#include <cstdlib>
#include <ctime>

using std::atoi;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;

int vis[55][55];
const int dxy[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // →↓←↑
enum class location : int
{
    SPACE,
    WALL,
    ENTER,
    EXIT,
    PATH
};

vector<vector<location>> map;

void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = y;
    coord.Y = x;
    // 获取控制台的句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, coord);
}

void HideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo); // 获取当前光标信息
    cursorInfo.bVisible = FALSE;                 // 设置光标不可见
    SetConsoleCursorInfo(hConsole, &cursorInfo); // 应用设置
}

void ShowCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo); // 获取当前光标信息
    cursorInfo.bVisible = TRUE;                  // 设置光标可见
    SetConsoleCursorInfo(hConsole, &cursorInfo); // 应用设置
}

struct Pos
{
    int x, y;
};

bool judge(Pos t)
{
    if (t.x < 0 || t.x >= map.size() || t.y < 0 || t.y >= map.size())
    {
        return false;
    }
    if (vis[t.x][t.y] != 0 || map[t.x][t.y] == location::WALL)
    {
        return false;
    }
    return true;
}

void dfs(int x, int y)
{
    Pos cur, end;
    cur.x = x, cur.y = y;
    end.x = map.size() - 2, end.y = map.size() - 1;
    mystack<Pos> path;
    int flag;
    path.push(cur);
    memset(vis, 0, sizeof(vis));
    while (cur.x != end.x || cur.y != end.y)
    {
        gotoXY(cur.x, cur.y * 2);
        cout << "㊣";
        Sleep(250);
        flag = 0;
        for (int i = 0; i < 4; i++)
        {
            Pos next;
            next.x = cur.x + dxy[i][0];
            next.y = cur.y + dxy[i][1];
            if (judge(next))
            {
                flag = i + 1;
                path.push(next);
                vis[next.x][next.y] = 1;
                break;
            }
        }
        gotoXY(cur.x, cur.y * 2);
        switch (flag)
        {
        case 0:
            path.pop();
            cout << "☒ ";
            break;
        case 1:
            cout << "→ ";
            break;
        case 2:
            cout << "↓ ";
            break;
        case 3:
            cout << "← ";
            break;
        case 4:
            cout << "↑ ";
            break;
        }
        cur = path.top();
    }
    gotoXY(end.x, end.y * 2);
    cout << "√";
    gotoXY(end.x + 2, 0);
}

void ShowMap(bool genMod)
{
    for (auto& line : map)
    {
        for (auto& loc : line)
        {
            assert(line.size() == line.size());
            switch (loc)
            {
            case location::WALL:
                cout << "■ ";
                break;
            case location::SPACE:
                if (genMod)
                {
                    cout << "■ ";
                    break;
                }
                cout << "□ ";
                break;
            case location::ENTER:
                cout << "= ";
                break;
            case location::EXIT:
                cout << "= ";
                break;
            defalut:
                cout << "o.O";
            }
        }
        cout << endl;
    }
}

void generateMaze(int n, string& filepath)
{

    system("cls");
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < n; i++)
    {
        vector<location> line;
        for (int j = 0; j < n; j++)
        {
            line.push_back(location::SPACE);
        }
        map.push_back(line);
        map[i][0] = location::WALL;
        map[i][n - 1] = location::WALL;
    }
    for (int i = 0; i < n; i++)
    {
        map[0][i] = location::WALL;
        map[n - 1][i] = location::WALL;
    }
    srand(int(time(0)));
    map[1][0] = location::ENTER;
    map[map.size() - 2][map.size() - 1] = location::EXIT;
    ShowMap(1);
    Pos cur, end;
    end.x = map.size() - 2, end.y = map.size() - 2;
    cur.x = 1, cur.y = 0;
    int flag;
    flag = 0;
    int offset;
    int step = 0;
    mystack<Pos> path;
    path.push(cur);
    while ((cur.x != end.x || cur.y != end.y) && step <= n * n / 4)
    {
        gotoXY(cur.x, cur.y * 2);
        cout << "□ ";
        Sleep(25);
        flag = 0;
        offset = rand();
        for (int i = 0; i < 4; i++)
        {
            Pos next;
            next.x = cur.x + dxy[(i + offset) % 4][0];
            next.y = cur.y + dxy[(i + offset) % 4][1];
            step++;
            if (judge(next))
            {
                flag = 1;
                path.push(next);
                vis[next.x][next.y] = 1;
                break;
            }
        }
        gotoXY(cur.x, cur.y * 2);
        switch (flag)
        {
        case 0:
            path.pop();
            break;
        case 1:
            cout << "□ ";
            break;
        }
        cur = path.top();
    }
    if (step <= n * n / 2)
    {
        mystack<Pos> path;
        cur.x = n / 2;
        cur.y = n / 2;
        path.push(cur);
        int findRoad = 0;
        while (step <= n * n / 2 || findRoad == 0)
        {
            gotoXY(cur.x, cur.y * 2);
            cout << "□ ";
            if (cur.x == end.x && cur.x == end.y)
                break;
            if (vis[cur.x][cur.y] == 1)
            {
                findRoad = 1;
            }
            Sleep(25);
            flag = 0;
            offset = rand() % 4;
            for (int i = 0; i < 4; i++)
            {
                Pos next;
                next.x = cur.x + dxy[(i + offset) % 4][0];
                next.y = cur.y + dxy[(i + offset) % 4][1];
                step++;
                if (judge(next))
                {
                    flag = 1;
                    path.push(next);
                    vis[next.x][next.y] = 2;
                    break;
                }
            }
            gotoXY(cur.x, cur.y * 2);
            switch (flag)
            {
            case 0:
                path.pop();
                break;
            case 1:
                cout << "□ ";
                break;
            }
            cur = path.top();
        }
    }
    if (vis[end.x][end.y] == 0)
    {
        mystack<Pos> path;
        cur.x = end.x;
        cur.y = end.y;
        vis[cur.x][cur.y] = 3;
        path.push(cur);
        while (1)
        {
            gotoXY(cur.x, cur.y * 2);
            cout << "□ ";
            Sleep(25);
            flag = 0;
            offset = rand() % 4;
            for (int i = 0; i < 4; i++)
            {
                Pos next;
                next.x = cur.x + dxy[(i + offset) % 4][0];
                next.y = cur.y + dxy[(i + offset) % 4][1];
                if (vis[next.x][next.y] == 1 || vis[next.x][next.y] == 2)
                    goto finish;
                if (judge(next))
                {
                    flag = 1;
                    path.push(next);
                    vis[next.x][next.y] = 3;
                    break;
                }
            }
            gotoXY(cur.x, cur.y * 2);
            switch (flag)
            {
            case 0:
                path.pop();
                break;
            case 1:
                cout << "□ ";
                break;
            }
            cur = path.top();
        }
    }
finish:
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (vis[i][j])
            {
                map[i][j] = location::SPACE;
            }
            else
            {
                map[i][j] = location::WALL;
            }
        }
    }
    map[map.size() - 2][map.size() - 1] = location::EXIT;
    map[1][0] = location::ENTER;
    filepath = "./config_gen";
    freopen(filepath.c_str(), "w", stdout);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (map[i][j] == location::WALL)
            {
                cout << "1 ";
            }
            else
            {
                cout << "0 ";
            }
        }
        cout << endl;
    }
    system("cls");
    freopen("CON", "w", stdout);
}

int main(int argc, char* argv[])
{
    string filepath = "./config";

    HideCursor();

    if (argc > 1)
    {
        if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--config") == 0)
        {
            if (argc > 2)
            {
                filepath = argv[2];
            }

            else
            {
                std::cerr << "Error: No file path provided after -c or --config" << endl;
                return 1;
            }
        }
        else if (strcmp(argv[1], "-g") == 0)
        {
            if (argc > 2)
            {
                generateMaze(atoi(argv[2]), filepath);
            }
            else
            {
                generateMaze(20, filepath);
            }
        }
    }
    freopen(filepath.c_str(), "r", stdin);
    string line;
    while (!map.empty())
    {
        map.pop_back();
    }
    while (getline(cin,line))
    {
        vector<location> line_t;
        for (auto& c : line)
        {
            if (c == '0' || c == '1')
            {
                line_t.push_back(static_cast<location>(c - '0'));
            }
        }
        if(!line_t.empty())
        map.push_back(line_t);
    }
    gotoXY(0, 1);
    map[1][0] = location::ENTER;
    map[map.size() - 2][map.size() - 1] = location::EXIT;
    ShowMap(0);
    dfs(1, 0);
    ShowCursor();
    return 0;
}