#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <set>
#include <map>
#include <iomanip>
using namespace std;

map<string, set<string>> school;
map<string, set<string>> major;
map<string, vector<string>> cls;

enum class RC
{
    SUCCESS,
    FAILED,
    FILE_NOT_FOUND,
    OBJECT_NOT_FOUND,
    PATH_NOT_EXISTS,
    UNIQUE_OBJECT_ALREADY_EXISTS,
    SYNTAX_ERROR,
    EXIT
};

string RCCaster(RC recalltype)
{
    switch (recalltype)
    {
    case RC::SUCCESS:
        return "SUCCESS";
        break;
    case RC::FAILED:
        return "FAILED";
        break;
    case RC::FILE_NOT_FOUND:
        return "FILE_NOT_FOUND";
        break;
    case RC::OBJECT_NOT_FOUND:
        return "OBJECT_NOT_FOUND";
        break;
    case RC::PATH_NOT_EXISTS:
        return "PATH_NOT_EXISTS";
        break;
    case RC::UNIQUE_OBJECT_ALREADY_EXISTS:
        return "UNIQUE_OBJECT_ALREADY_EXISTS";
        break;
    case RC::SYNTAX_ERROR:
        return "SYNTAX_ERROR";
        break;
    case RC::EXIT:
        return "EXIT_SUCCESS";
        break;
    }
    return "";
}

RC ShowTreeAll()
{
    string prefix0, prefix1, prefix2;
    for (auto it = school.begin(); it != school.end(); it++)
    {
        cout << "|---" << it->first << endl;
        if (next(it) != school.end())
        {
            prefix0 = "|       ";
        }
        else
        {
            prefix0 = "        ";
        }
        for (auto majors = it->second.begin(); majors != it->second.end(); majors++)
        {
            cout << prefix0 << "|---" << *majors << endl;
            if (next(majors) != it->second.end())
            {
                prefix1 = "|       ";
            }
            else
            {
                prefix1 = "        ";
            }
            for (auto clss = major[*majors].begin(); clss != major[*majors].end(); clss++)
            {
                cout << prefix0 << prefix1 << "|---" << *clss << endl;
                if (next(clss) != major[*majors].end())
                {
                    prefix2 = "|       ";
                }
                else
                {
                    prefix2 = "        ";
                }
                for (auto stu = cls[*clss].begin(); stu != cls[*clss].end(); stu++)
                {
                    cout << prefix0 << prefix1 << prefix2 << "|---" << *stu << endl;
                }
            }
        }
    }
    return RC::SUCCESS;
}

RC SelectSchool(string name)
{
    string prefix0, prefix1, prefix2;
    bool check = false;
    for (auto it = school.begin(); it != school.end(); it++)
    {
        if (it->first != name && name != "*")
        {
            continue;
        }
        else
        {
            check = true;
        }
        cout << "|---" << it->first << endl;
        if (next(it) != school.end() && name == "*")
        {
            prefix0 = "|       ";
        }
        else
        {
            prefix0 = "        ";
        }
        for (auto majors = it->second.begin(); majors != it->second.end(); majors++)
        {
            cout << prefix0 << "|---" << *majors << endl;
            if (next(majors) != it->second.end())
            {
                prefix1 = "|       ";
            }
            else
            {
                prefix1 = "        ";
            }
            for (auto clss = major[*majors].begin(); clss != major[*majors].end(); clss++)
            {
                cout << prefix0 << prefix1 << "|---" << *clss << endl;
                if (next(clss) != major[*majors].end())
                {
                    prefix2 = "|       ";
                }
                else
                {
                    prefix2 = "        ";
                }
                for (auto stu = cls[*clss].begin(); stu != cls[*clss].end(); stu++)
                {
                    cout << prefix0 << prefix1 << prefix2 << "|---" << *stu << endl;
                }
            }
        }
        if (check && name != "*")
            break;
    }
    if (!check)
        return RC::OBJECT_NOT_FOUND;
    return RC::SUCCESS;
}

RC SelectMajor(string name)
{
    bool check = false;
    string prefix0, prefix1;
    for (auto it = major.begin(); it != major.end(); it++)
    {
        if (it->first != name && name != "*")
            continue;
        check = true;
        if (next(it) != major.end() && name == "*")
        {
            prefix0 = "|       ";
        }
        else
        {
            prefix0 = "        ";
        }
        cout << "|---" << it->first << endl;
        for (auto iit = it->second.begin(); iit != it->second.end(); iit++)
        {
            cout << prefix0 << "|---" << *iit << endl;
            if (next(iit) != it->second.end())
            {
                prefix1 = "|       ";
            }
            else
            {
                prefix1 = "        ";
            }
            for (auto stu : cls[*iit])
            {
                cout << prefix0 << prefix1 << "|---" << stu << endl;
            }
        }
        if (check && name != "*")
            break;
    }
    if (check)
        return RC::SUCCESS;
    return RC::OBJECT_NOT_FOUND;
}

RC SelectClass(string name)
{
    bool check = false;
    string prefix0;
    for (auto it = cls.begin(); it != cls.end(); it++)
    {
        if (it->first != name && name != "*")
            continue;
        check = true;
        if (next(it) != cls.end() && name == "*")
        {
            prefix0 = "|       ";
        }
        else
        {
            prefix0 = "        ";
        }
        cout << "|---" << it->first << endl;
        for (auto stu : it->second)
        {
            cout << prefix0 << "|---" << stu << endl;
        }
        if (check && name != "*")
            break;
    }
    if (check)
        return RC::SUCCESS;
    return RC::OBJECT_NOT_FOUND;
}

RC Load(string path)
{
    string stu, sschool, smajor, scls, sname;
    ifstream in(path.c_str());
    if (!in.is_open())
    {
        return RC::FILE_NOT_FOUND;
    }
    while (getline(in, stu))
    {
        istringstream iss(stu);
        iss >> sschool >> smajor >> scls >> sname;
        school[sschool].insert(smajor);
        major[smajor].insert(scls);
        cls[scls].push_back(sname);
        // cout << sschool << " " << smajor << " " << scls << " " << sname << endl;
    }
    return RC::SUCCESS;
}

RC Save(string path)
{
    if(path=="")
    return RC::PATH_NOT_EXISTS;
    fstream fout(path, ios::out);
    for (auto &it : school)
    {
        for (auto &iit : it.second)
        {
            for (auto &iiit : major[iit])
            {
                for (auto &iiiit : cls[iiit])
                {
                    fout << it.first << " " << iit << " " << iiit << " " << iiiit << endl;
                }
            }
        }
    }
    return RC::SUCCESS;
}

RC Insert(vector<string> path, string value)
{
    switch (path.size())
    {
    case 0:
        if (school.count(value) == 0)
        {
            school[value] = {};
        }
        else
        {
            return RC::UNIQUE_OBJECT_ALREADY_EXISTS;
        }
        break;
    case 1:
        if (school[path[0]].insert(value).second)
        {
            major[value] = {};
        }
        else
        {
            return RC::UNIQUE_OBJECT_ALREADY_EXISTS;
        }
        break;
    case 2:
        school[path[0]].insert(path[1]);
        if (major[path[1]].insert(value).second)
        {
            cls[value] = {};
        }
        else
        {
            return RC::UNIQUE_OBJECT_ALREADY_EXISTS;
        }
        break;
    case 3:
        school[path[0]].insert(path[1]);
        major[path[1]].insert(path[2]);
        cls[path[2]].push_back(value);
        break;
    }
    return RC::SUCCESS;
}

RC Update(string type, string name, string value)
{
    if (type == "school")
    {
        if (school.find(name) == school.end())
        {
            return RC::OBJECT_NOT_FOUND;
        }
        school[value] = school[name];
        school.erase(name);
        return RC::SUCCESS;
    }
    else if (type == "major")
    {
        if (major.find(name) == major.end())
        {
            return RC::OBJECT_NOT_FOUND;
        }
        major[value] = major[name];
        major.erase(name);
        for (auto &it : school)
        {
            if (it.second.find(name) != it.second.end())
            {
                it.second.insert(value);
                it.second.erase(name);
                break;
            }
        }
        return RC::SUCCESS;
    }
    else if (type == "class")
    {
        if (cls.find(name) == cls.end())
        {
            return RC::OBJECT_NOT_FOUND;
        }
        cls[value] = cls[name];
        cls.erase(name);
        for (auto &it : major)
        {
            if (it.second.find(name) != it.second.end())
            {
                it.second.insert(value);
                it.second.erase(name);
                break;
            }
        }
        return RC::SUCCESS;
    }
    else if (type == "student")
    {
        for (auto &it : cls)
        {
            for (auto &iit : it.second)
            {
                if (iit == name)
                {
                    iit = value;
                    return RC::SUCCESS;
                }
            }
        }
        return RC::OBJECT_NOT_FOUND;
    }
    else
    {
        return RC::SYNTAX_ERROR;
    }
    return RC::SUCCESS;
}

RC Drop(string type, string name)
{
    if (type == "school")
    {
        if (school.count(name) == 0)
        {
            return RC::OBJECT_NOT_FOUND;
        }
        for (auto &it : school[name])
        {
            for (auto &iit : major[it])
            {
                cls[iit].clear();
            }
            major[it].clear();
        }
        school[name].clear();
        school.erase(name);
        return RC::SUCCESS;
    }
    else if (type == "major")
    {
        if (major.count(name) == 0)
        {
            return RC::OBJECT_NOT_FOUND;
        }
        for (auto &iit : major[name])
        {
            cls[iit].clear();
        }
        major[name].clear();
        major.erase(name);
        for (auto &it : school)
        {
            if (it.second.find(name) != it.second.end())
            {
                it.second.erase(name);
                break;
            }
        }
        return RC::SUCCESS;
    }
    else if (type == "class")
    {
        if (cls.count(name) == 0)
        {
            return RC::OBJECT_NOT_FOUND;
        }
        cls[name].clear();
        cls.erase(name);
        for (auto &it : major)
        {
            if (it.second.find(name) != it.second.end())
            {
                it.second.erase(name);
                break;
            }
        }
        return RC::SUCCESS;
    }
    else if (type == "student")
    {
        for (auto &it : cls)
        {
            for (auto iit = it.second.begin(); iit != it.second.end(); iit++)
            {
                if (*iit == name)
                {
                    it.second.erase(iit);
                    return RC::SUCCESS;
                }
            }
        }
        return RC::OBJECT_NOT_FOUND;
    }
    return RC::FAILED;
}

RC Cancel(int &history)
{
    history--;
    school.clear();
    major.clear();
    cls.clear();
    return Load("history/" + to_string(history));
}

RC Help()
{
    system("cls");
    cout << left << setw(60) << "--------------------StudentsManageSystem--------------------" << endl
         << left << setw(60) << "插入: insert <DataType> <DataPath>" << endl
         << left << setw(60) << "删除: drop <DataTypy> <DataName>" << endl
         << left << setw(60) << "更新: update <DataType> <OldName> (to) <NewName>" << endl
         << left << setw(60) << "查询: select <DataType> <DataName(* for all)>" << endl
         << left << setw(60) << "撤销: cancel" << endl
         << left << setw(60) << "DataType:school(学院)、major(专业)、class(班级)、student(学生姓名)" << endl
         << left << setw(60) << "保留字请用全部小写输入" << endl
         << left << setw(60) << "按任意键返回" << endl;
    system("pause");
    system("cls");
    return RC::SUCCESS;
}

RC parser(string expr)
{
    static int history = 0;
    bool hasChanged = false;
    RC rc;
    string opmod;
    istringstream iss(expr);
    iss >> opmod;
    if (opmod == "load")
    {
        string opvalue;
        iss >> opvalue >> opvalue;
        rc = Load(opvalue); // load From <path> / load <path>
        hasChanged = true;
    }
    else if (opmod == "update") // update <targettype> <targetname> to <targetvalue> /update <targettype> <targetname> <targetvalue>
    {
        string targettype, targetname, targetvalue;
        iss >> targettype >> targetname >> targetvalue >> targetvalue;
        rc = Update(targettype, targetname, targetvalue);
        hasChanged = true;
    }
    else if (opmod == "insert")
    {
        string mod;
        vector<string> path;
        string value;
        iss >> mod;
        if (mod == "school")
        {
            iss >> value;
        }
        else if (mod == "major")
        {
            iss >> value;
            path.push_back(value);
            iss >> value;
        }
        else if (mod == "class")
        {
            iss >> value;
            path.push_back(value);
            iss >> value;
            path.push_back(value);
            iss >> value;
        }
        else if (mod == "student")
        {
            iss >> value;
            path.push_back(value);
            iss >> value;
            path.push_back(value);
            iss >> value;
            path.push_back(value);
            iss >> value;
        }
        else
        {
            return RC::SYNTAX_ERROR;
        }
        rc = Insert(path, value);
        hasChanged = true;
    }
    else if (opmod == "drop")
    {
        string type, name;
        iss >> type >> name;
        rc = Drop(type, name);
        hasChanged = true;
    }
    else if (opmod == "save")
    {
        string path;
        iss >> path;
        rc = Save(path);
    }
    else if (opmod == "exit")
    {
        rc = RC::EXIT;
    }
    else if (opmod == "show") // show TODO:add different object detect and output
    {
        cout << endl
             << endl;
        cout << "-----------------------------------------" << endl;
        rc = ShowTreeAll();
        cout << "-----------------------------------------" << endl;
        cout << endl
             << endl;
    }
    else if (opmod == "cancel")
    {
        return Cancel(history);
    }
    else if (opmod == "select")
    {
        string mod, name;
        iss >> mod >> name;
        if (mod == "school")
        {
            rc = SelectSchool(name);
        }
        else if (mod == "major")
        {
            rc = SelectMajor(name);
        }
        else if (mod == "class")
        {
            rc = SelectClass(name);
        }
        else
        {
            rc = RC::SYNTAX_ERROR;
        }
    }
    else if (opmod == "help")
    {
        rc = Help();
    }
    else
    {
        rc = RC::SYNTAX_ERROR;
    }
    if (rc == RC::SUCCESS && hasChanged)
    {
        history++;
        Save("history/" + to_string(history));
    }
    return rc;
}

void Init()
{
    system("IF NOT EXIST history\\ (mkdir history)");
    fstream f("history/0", ios::out);
    f << "";
}

int main()
{
    string expr;
    cout << "StudentManageSystem=#";
    RC rc;
    Init();
    bool end = false;
    while (getline(cin, expr))
    {
        rc = parser(expr);
        if (rc == RC::EXIT)
        {
            end = true;
        }
        else if (rc != RC::SUCCESS)
        {
            cout << endl
                 << endl
                 << "Error Occured!" << endl
                 << endl;
        }
        cout << "StudentManageSystem-=" << RCCaster(rc) << endl;
        if (end)
            break;
        cout << "StudentManageSystem=#";
    }
}