### 默认迷宫

请在对应文件夹打开终端，输入 ```./main.exe```,会使用内置的默认20*20的迷宫展示DFS算法

### 导入迷宫

请在对应文件夹打开终端，输入```./main.exe -c <filepath>```,会获取对应文件内的以0,1标记空地、墙壁的迷宫，出入口默认在(1,0)和(n-1,n)

### 随机迷宫
请在对应文件夹打开终端，输入```./main.exe -g <mazeSize>```,会展示迷宫生成过程，将生成的迷宫保存在同文件夹下的config_gen文件中

## Warning
随机迷宫通过完全随机算法产生，在运气较差的情况下可能会遇到访问空堆栈的问题，并且很多情况下迷宫并不完美，不推荐使用