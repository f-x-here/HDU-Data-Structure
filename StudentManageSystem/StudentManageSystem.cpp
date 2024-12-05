#include <iostream>
#include <vector>
#include <map>
using namespace std;

struct Attribute{
    string name;
    string cls;
    string major;
    string school;
    Attribute(string _name,string _cls,string _major,string _school):name(_name),cls(_cls),major(_major),school(_school){}
};

map<string,Attribute>Students;

struct piece{
    string data;
    piece *next;
    vector<piece*>child;
};

void BuildTree()
{
    vector<vector<string>>exprs;
    for(auto it:Students)
    {
        vector<string>temp;
        temp.push_back(it.second.school);
        temp.push_back(it.second.major);
        temp.push_back(it.second.cls);
        temp.push_back(it.second.name);
        exprs.push_back(temp);
    }
    piece *head = new piece;
    piece* cur;
    cur = head;
    for(auto it:exprs)
    {
        
    }

}

void ShowTree()
{
    
}

int main()
{

}