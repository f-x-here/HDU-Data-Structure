#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <deque>
#include <iomanip>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

#pragma region enums

enum class ElevatorStates
{
	Up,
	Down,
	StayClose,
	StayOpen,
};

enum class GuestState
{
	WaitForElevator,
	InElevator,
	LeaveFloor,
	LeaveElevator,
	ToElevator,
};

#pragma endregion

#pragma region defines

#define messageStart 31
#define guestWalkTime 30
#define elevatorTickTime 10
#define guestGenerateTime 100
#undef SendMessage

class Guest;
class GuestManager;
class Elevator;
class Floor;
class FloorManager;
class MessageManager;
class RulesManager;

class Guest
{
	friend GuestManager;
	friend MessageManager;

private:
	string name;
	double weight;
	int curFloor;
	int targetFloor;
	int createTime;
	int waitTime;
	int inElevatorTime;
	int outElevatorTime;
	int leaveTime;
	int maxWaitTime;
	int enterTime;
	int flag1;
	int flag2;
	GuestState curState;

public:
	void Awake(string _name, double _weight, int _curFloor, int _targetFloor, int _maxWaitTime);
	void UpdateLogic();
	void UpdateAnim();
	bool operator==(const Guest &rhs)
	{
		return rhs.name == name;
	}
	void ShowInfo();
};

class Elevator
{
	friend MessageManager;

private:
	int waitTime;
	deque<int> targetFloor;
	double curWeight;
	double maxWeight;
	int maxSize;
	int curSize;
	int curPosition;
	bool isWaiting;

public:
	int curFloor;
	ElevatorStates curState;
	void Start();
	void UpdateAnim();
	void UpdateLogic();
};

class MessageManager
{
private:
	deque<string> messages;
	deque<string> requests;
	string message;
	int maxSize;

public:
	void Start();

	void GuestGenerate(Guest &requester);
	void ElevatorMoving();
	void ElevatorArrived();
	void ElevatorWaiting();
	bool GuestToElevator(Guest &requester);
	void GuestLeaveElevator(Guest &requester);
	void GuestLeaveFloor(Guest &requester);
	void UpdateMessageList(string message);
	void UpdateAnim();

	bool NewRequest(string request);
	void GetNewRequest();
};

class GuestManager
{
private:
	vector<Guest> guestList;

public:
	void GenerateGuest();
	void UpdateLogic();
	void UpdateAnim();
	void Destroy(Guest guest);
	void UpdateGuestLitsAnim();
};

class Floor
{
	friend FloorManager;
	friend MessageManager;
	friend Elevator;

private:
	bool UpRequest;
	bool DownRequest;

public:
	void Start();
	void UpdateAnim(int FloorID);
};

class FloorManager
{
	friend MessageManager;
	friend Floor;
	friend Elevator;

private:
	vector<Floor> floors;

public:
	void Start();
	void SendMessage(int floorID, string message);
	void SendMessage(int floorID, ElevatorStates state);
};

class RulesManager
{
	friend MessageManager;

private:
	bool ruleFinished[16];
	int numOfFinishedRules;

public:
	void CheckRules(int list);
	bool Check() { return numOfFinishedRules == 15; }
	void Start();
};

Elevator elevator;
GuestManager guestManager;
MessageManager messageManager;
FloorManager floorManager;
RulesManager rulesManager;

#pragma endregion

#pragma region Utils

void gotoXY(int x, int y)
{
	COORD coord;
	coord.X = y;
	coord.Y = x;
	// 获取控制台的句柄
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
}

// 隐藏光标
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleCursorInfo(hConsoleOutput, &cursorInfo))
	{
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
	}
	else
	{
		std::cerr << "无法获取光标信息。" << std::endl;
	}
}

// 显示光标
void ShowCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleCursorInfo(hConsoleOutput, &cursorInfo))
	{
		cursorInfo.bVisible = TRUE;
		SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
	}
	else
	{
		std::cerr << "无法获取光标信息。" << std::endl;
	}
}

bool ASCE(int a, int b)
{
	return a < b;
}

bool DESC(int a, int b)
{
	return a > b;
}

#pragma endregion

#pragma region Timer
int Timer;
int isBusy;

void BusyFor(int time)
{
	isBusy = time;
}

void Tick()
{
	Sleep(1);
	Timer = (Timer + 1) % 10000000;
	isBusy--;
}

#pragma endregion

#pragma region Elevator

void Elevator::Start()
{
	curFloor = 1;
	curPosition = 20;
	maxSize = 5;
	maxWeight = 300.0;
	curState = ElevatorStates::StayClose;
	isWaiting = false;
}

void Elevator::UpdateAnim()
{

	switch (static_cast<int>(curState))
	{
	case 0:
	case 1:
	case 2:
	{
		gotoXY(curPosition, 0);
		cout << "                           ";
		gotoXY(curPosition + 1, 0);
		cout << "***************************";
		gotoXY(curPosition + 2, 0);
		cout << "*" << curSize << "/" << maxSize << "                      *";
		gotoXY(curPosition + 3, 0);
		cout << "*" << setw(3) << curWeight << "kg/" << setw(3) << maxWeight << "kg              *";
		gotoXY(curPosition + 4, 0);
		cout << "*                         **";
		gotoXY(curPosition + 5, 0);
		cout << "*                         **";
		gotoXY(curPosition + 6, 0);
		cout << "***************************";
		gotoXY(curPosition + 7, 0);
		cout << "                           ";
	}
	break;
	case 3:
	{
		gotoXY(curPosition, 0);
		cout << "                           ";
		gotoXY(curPosition + 1, 0);
		cout << "***************************";
		gotoXY(curPosition + 2, 0);
		cout << "*" << curSize << "/" << maxSize << "                      *";
		gotoXY(curPosition + 3, 0);
		cout << "*" << setw(3) << curWeight << "kg/" << setw(3) << maxWeight << "kg              *";
		gotoXY(curPosition + 4, 0);
		cout << "*                           ";
		gotoXY(curPosition + 5, 0);
		cout << "*                           ";
		gotoXY(curPosition + 6, 0);
		cout << "***************************";
		gotoXY(curPosition + 7, 0);
		cout << "                           ";
	}
	break;
	}
}

void Elevator::UpdateLogic()
{
	bool hasWaited = false;
	switch (static_cast<int>(curState))
	{
	case 0: // up
		curPosition = curPosition - 1;
		break;
	case 1: // down
		curPosition = curPosition + 1;
	case 2: // close
		waitTime = 0;
		break;
	case 3: // open
		waitTime++;
		break;
	}
	if (curPosition % 5 == 0)
	{
		curFloor = 5 - curPosition / 5;
		if ((!targetFloor.empty()) && (curFloor == targetFloor.front()))
		{
			curState = ElevatorStates::StayClose;
		}
	}
	if (curState == ElevatorStates::StayOpen && waitTime >= 3 && isBusy < 0)
	{
		curState = ElevatorStates::StayClose;
		rulesManager.CheckRules(13);
		hasWaited = true;
	}
	if (!targetFloor.empty())
	{
		if (curState != ElevatorStates::StayOpen)
		{
			if (curFloor < targetFloor.front())
			{
				curState = ElevatorStates::Up;
				rulesManager.CheckRules(2);
				if (hasWaited)
				{
					hasWaited = false;
					floorManager.SendMessage(curFloor, curState);
				}
				if (isWaiting)
				{
					messageManager.ElevatorMoving();
				}
				isWaiting = false;
			}
			else if (curFloor > targetFloor.front())
			{
				curState = ElevatorStates::Down;
				rulesManager.CheckRules(3);
				if (hasWaited)
				{
					hasWaited = false;
					floorManager.SendMessage(curFloor, curState);
				}
				if (isWaiting)
					messageManager.ElevatorMoving();
				isWaiting = false;
			}
			else if (curFloor == targetFloor.front())
			{
				curState = ElevatorStates::StayOpen;
				rulesManager.CheckRules(1);
				while (!targetFloor.empty() && targetFloor.front() == curFloor)
				{
					targetFloor.pop_front();
				}
				if (curSize == maxSize) // 满载下客位置停靠
				{
					rulesManager.CheckRules(15);
				}
				messageManager.ElevatorArrived();
			}
		}
		else if (curState == ElevatorStates::StayOpen)
		{
			if (!isWaiting)
			{
				messageManager.ElevatorWaiting();
				isWaiting = true;
			}
		}
	}
	else if (isBusy < 0)
	{
		curState = ElevatorStates::StayClose;
		messageManager.GetNewRequest();
	}
	if(curState == ElevatorStates::Up && floorManager.floors[curFloor].UpRequest==true)
	{
		rulesManager.CheckRules(7);
	}
	if(curState == ElevatorStates::Down && floorManager.floors[curFloor].DownRequest==true)
	{
		rulesManager.CheckRules(11);
	}
}

#pragma endregion

#pragma region Guests

void Guest::Awake(string _name, double _weight, int _curFloor, int _targetFloor, int _maxWaitTime)
{
	name = _name;
	weight = _weight;
	curFloor = _curFloor;
	targetFloor = _targetFloor;
	createTime = Timer;
	waitTime = 0;
	inElevatorTime = -1;
	outElevatorTime = -1;
	leaveTime = -1;
	maxWaitTime = _maxWaitTime;
	curState = GuestState::WaitForElevator;
	enterTime = -1;
	flag1 = 0;
	flag2 = 0;
}

void Guest::UpdateLogic()
{
	switch (static_cast<int>(curState))
	{
	case 0: // WaitForElevator
		if (elevator.curFloor == curFloor && elevator.curState == ElevatorStates::StayOpen && isBusy < 0)
		{
			BusyFor(guestWalkTime);
			curState = GuestState::ToElevator;
		}
		else
		{
			waitTime = Timer - createTime;
			if (waitTime > maxWaitTime && leaveTime == -1)
			{
				leaveTime = Timer;
				curState = GuestState::LeaveFloor;
			}
		}
		break;
	case 1: // InElevator
		if (elevator.curFloor == targetFloor && elevator.curState == ElevatorStates::StayOpen && isBusy < 0)
		{
			curFloor = elevator.curFloor;
			curState = GuestState::LeaveElevator;
		}
		break;
	case 2: // LeaveFloor
		if (outElevatorTime == -1 && leaveTime == -1)
		{
			leaveTime = Timer;
		}
		else if (leaveTime == -1)
		{
			leaveTime = Timer + guestWalkTime;
			messageManager.GuestLeaveFloor(*this);
		}
		else if (Timer - leaveTime >= 1000)
		{
			guestManager.Destroy(*this);
		}
		break;
	case 3: // LeaveElevator
		if (isBusy < 0 && inElevatorTime != -1)
		{
			BusyFor(guestWalkTime);
			outElevatorTime = Timer + guestWalkTime;
			messageManager.GuestLeaveElevator(*this);
			curState = GuestState::LeaveFloor;
		}
		break;
	case 4: // ToElevator
		if (elevator.curFloor == curFloor && elevator.curState == ElevatorStates::StayOpen)
		{
			if (messageManager.GuestToElevator(*this))
			{
				inElevatorTime = Timer;
				curState = GuestState::InElevator;
			}
			else
			{
				curState = GuestState::WaitForElevator;
			}
		}
		break;
	}
}

void Guest::UpdateAnim()
{
	static int offset;
	if (enterTime == -1 && ((curState == GuestState::InElevator && flag1 == 0) || (curState == GuestState::LeaveFloor && flag2 == 0)))
	{
		enterTime = Timer;
		if (waitTime > maxWaitTime)
		{
			offset = 12;
		}
		else
		{
			offset = 0;
		}
	}

	if (curState == GuestState::InElevator && enterTime != -1)
	{
		if (Timer - enterTime == 0)
		{
			gotoXY((6 - curFloor) * 5, 31);
			cout << "人";
		}
		else if (Timer - enterTime == guestWalkTime / 3)
		{
			gotoXY((6 - curFloor) * 5, 31);
			cout << "  ";
			gotoXY((6 - curFloor) * 5, 25);
			cout << "人";
		}
		else if (Timer - enterTime == ((guestWalkTime / 3) * 2))
		{
			gotoXY((6 - curFloor) * 5, 25);
			cout << "  ";
			gotoXY((6 - curFloor) * 5, 19);
			cout << "人";
		}
		else if (Timer - enterTime == guestWalkTime)
		{
			gotoXY((6 - curFloor) * 5, 19);
			cout << "  ";
			enterTime = -1;
			flag1 = 1;
		}
	}
	else if (curState == GuestState::LeaveFloor && enterTime != -1 && flag2 == 0)
	{
		if (Timer - enterTime == 0)
		{
			gotoXY((6 - curFloor) * 5, 19 + offset);
			cout << "人";
		}
		else if (Timer - enterTime == guestWalkTime / 3)
		{
			gotoXY((6 - curFloor) * 5, 19 + offset);
			cout << "  ";
			gotoXY((6 - curFloor) * 5, 25 + offset);
			cout << "人";
		}
		else if (Timer - enterTime == ((guestWalkTime / 3) * 2))
		{
			gotoXY((6 - curFloor) * 5, 25 + offset);
			cout << "  ";
			gotoXY((6 - curFloor) * 5, 31 + offset);
			cout << "人";
		}
		else if (Timer - enterTime == guestWalkTime)
		{
			gotoXY((6 - curFloor) * 5, 31 + offset);
			cout << "  ";
			enterTime = -1;
			flag2 = 1;
		}
	}
}

void Guest::ShowInfo()
{
	string state;
	switch (static_cast<int>(curState))
	{
	case 0:
		state = "WaitForElevator";
		break;
	case 1:
		state = "InElevator";
		break;
	case 2:
		state = "LeaveFloor";
		break;
	case 3:
		state = "LeaveElevator";
		break;
	case 4:
		state = "ToElevator";
		break;
	}
	cout << std::left << setw(6) << name
		 << std::left << setw(6) << weight
		 << std::left << setw(10) << curFloor
		 << std::left << setw(10) << targetFloor
		 << std::left << setw(10) << createTime
		 << std::left << setw(10) << waitTime
		 << std::left << setw(10) << inElevatorTime
		 << std::left << setw(10) << outElevatorTime
		 << std::left << setw(10) << leaveTime
		 << std::left << setw(15) << state;
}

#pragma endregion

#pragma region GuestManager

void GuestManager::GenerateGuest()
{
	srand(static_cast<unsigned int>(time(0)));
	Guest newGuest;
	static int name = 0;
	int curFloor = 1 + rand() % 5;
	int targetFloor = 1 + rand() % 5;
	while (targetFloor == curFloor)
	{
		targetFloor = 1 + rand() % 5;
	}
	newGuest.Awake(to_string(name++), 50.0 + rand() % 20, curFloor, targetFloor, 1000 + rand() % 1000);
	string request = to_string(curFloor) + " " + to_string(targetFloor);
	messageManager.NewRequest(request);
	floorManager.SendMessage(curFloor, request);
	messageManager.GuestGenerate(newGuest);
	guestList.push_back(newGuest);
}

void GuestManager::UpdateLogic()
{
	for (auto &g : guestList)
	{
		g.UpdateLogic();
	}
}

void GuestManager::UpdateAnim()
{
	for (auto &g : guestList)
	{
		g.UpdateAnim();
	}
}

void GuestManager::Destroy(Guest guest)
{
	guestList.erase(find(guestList.begin(), guestList.end(), guest));
}

void GuestManager::UpdateGuestLitsAnim()
{
	int maxSize = 22;
	string Log;
	Guest cur;
	string nothing = " ";
	for (int i = 0; i < maxSize; i++)
	{
		gotoXY(6 + i, 87);
		if (i < guestList.size())
		{
			cur = guestList[i];
			cur.ShowInfo();
		}
		else
		{
			cout << setw(100) << nothing;
		}
	}
}

#pragma endregion

#pragma region MessageManager

void MessageManager::Start()
{
	maxSize = 29;
}

void MessageManager::GuestGenerate(Guest &requester)
{
	message = to_string(Timer) + " " + requester.name + " " + "generated!"; // 我TM来啦！
	UpdateMessageList(message);
}

void MessageManager::ElevatorMoving()
{
	message = to_string(Timer) + " elevator moving."; // 在动
	UpdateMessageList(message);
}

void MessageManager::ElevatorArrived()
{
	message = to_string(Timer) + " elevator arrived."; // 到了
	UpdateMessageList(message);
}

void MessageManager::ElevatorWaiting()
{
	message = to_string(Timer) + " elevator waiting."; // 快点下去/上来
	UpdateMessageList(message);
}

bool MessageManager::GuestToElevator(Guest &requester)
{
	bool getin = false;
	if (elevator.curWeight + requester.weight > elevator.maxWeight)
	{
		message = to_string(Timer) + " " + requester.name + " " + "try get into elevator but overweight"; // 咕噜咕噜滚下电梯
		rulesManager.CheckRules(12);
	}
	else if (elevator.curSize + 1 > elevator.maxSize)
	{
		message = to_string(Timer) + " " + requester.name + " " + "try get into elevator but oversize"; // 进不去，怎么想都进不去
		rulesManager.CheckRules(12);
	}
	else
	{
		elevator.curWeight += requester.weight;
		elevator.curSize++;
		message = to_string(Timer) + " " + requester.name + " " + "get into elevator"; // 进去了
		getin = true;
	}
	UpdateMessageList(message);
	elevator.UpdateAnim();
	return getin;
}

void MessageManager::GuestLeaveElevator(Guest &requester)
{
	elevator.curWeight -= requester.weight;
	elevator.curSize--;
	message = to_string(Timer) + " " + requester.name + " " + "leave elevator"; // 走了喵
	UpdateMessageList(message);
}

void MessageManager::GuestLeaveFloor(Guest &requester)
{
	if (requester.outElevatorTime != -1)
	{
		message = to_string(Timer) + " " + requester.name + " " + "arrived target floor";
	}
	else
	{
		message = to_string(Timer) + " " + requester.name + " " + "can not wait any longer";
	}
	UpdateMessageList(message);
}

void MessageManager::UpdateMessageList(string message)
{
	if (messages.size() + 1 > maxSize)
	{
		messages.pop_back();
	}
	messages.push_front(message);
	UpdateAnim();
}

void MessageManager::UpdateAnim()
{
	for (int i = 0; i < messages.size(); i++)
	{
		gotoXY(messageStart + i, 87);
		cout << std::left << setw(100) << messages[i];
	}
}

bool MessageManager::NewRequest(string request)
{
	bool beAccept = false;
	int targetFloor1, targetFloor2;
	sscanf(request.c_str(), "%d %d", &targetFloor1, &targetFloor2);
	if (elevator.targetFloor.empty())
	{
		elevator.targetFloor.push_front(targetFloor1);
		elevator.targetFloor.push_back(targetFloor2);
		beAccept = true;
	}
	else if (elevator.curSize == elevator.maxSize)
	{
		rulesManager.CheckRules(14); // 满载不处理请求，也不在非下客位置停靠
	}
	else
	{
		switch (static_cast<int>(elevator.curState))
		{
		case 0: // Up
			if (targetFloor1 < targetFloor2)
			{
				if (targetFloor1 > elevator.curFloor)
				{
					rulesManager.CheckRules(4); // 上行上层有请求
					elevator.targetFloor.push_back(targetFloor2);
					elevator.targetFloor.push_front(targetFloor1);
					sort(elevator.targetFloor.begin(), elevator.targetFloor.end(), ASCE);
					beAccept = true;
				}
				else if (targetFloor1 == elevator.curFloor)
				{
					rulesManager.CheckRules(7); // 上行本层有请求
				}
				else if (targetFloor1 < elevator.curFloor)
				{
					rulesManager.CheckRules(5); // 上行下层有请求
				}
			}
			else if (targetFloor1 > targetFloor2)
			{
				rulesManager.CheckRules(6); // 上行有下行请求
			}
			break;
		case 1: // Down
			if (targetFloor2 < targetFloor1)
			{
				if ((targetFloor1 < elevator.curFloor))
				{
					rulesManager.CheckRules(8); // 下行下层有请求
					elevator.targetFloor.push_back(targetFloor1);
					elevator.targetFloor.push_back(targetFloor2);
					sort(elevator.targetFloor.begin(), elevator.targetFloor.end(), DESC);
					beAccept = true;
				}
				else if ((targetFloor1 == elevator.curFloor))
				{
					rulesManager.CheckRules(11); // 下行本层有请求
				}
				else if ((targetFloor1 > elevator.curFloor))
				{
					rulesManager.CheckRules(9); // 下行上层有请求
				}
			}
			else
			{
				rulesManager.CheckRules(10); // 下行上行请求
			}
			break;
		default:
			if (elevator.curFloor < elevator.targetFloor.front())
			{
				if (targetFloor1 < targetFloor2 && targetFloor1 >= elevator.curFloor)
				{
					elevator.targetFloor.push_back(targetFloor2);
					elevator.targetFloor.push_front(targetFloor1);
					sort(elevator.targetFloor.begin(), elevator.targetFloor.end(), ASCE);
					beAccept = true;
				}
			}
			else if (elevator.curFloor > elevator.targetFloor.front())
			{
				if (targetFloor2 < targetFloor1 && targetFloor1 <= elevator.curFloor)
				{
					elevator.targetFloor.push_back(targetFloor1);
					elevator.targetFloor.push_back(targetFloor2);
					sort(elevator.targetFloor.begin(), elevator.targetFloor.end(), DESC);
					beAccept = true;
				}
			}
			break;
		}
	}
	if (!beAccept)
		requests.push_back(request);
	return beAccept;
}

void MessageManager::GetNewRequest()
{
	if (requests.empty())
		return;
	int targetFloor1, targetFloor2;
	int size = requests.size();
	for (int i = 0; i < size; i++)
	{
		message = requests.front();
		requests.pop_front();
		NewRequest(message);
	}
}

#pragma endregion

#pragma region Floor

void Floor::Start()
{
	UpRequest = false;
	DownRequest = false;
}

void Floor::UpdateAnim(int floorID)
{
	gotoXY(5 * (6 - floorID) - 3, 29);
	if (UpRequest)
	{
		cout << "▲";
	}
	else
	{
		cout << "△";
	}

	gotoXY(5 * (6 - floorID) - 2, 29);
	if (DownRequest)
	{
		cout << "▼";
	}
	else
	{
		cout << "▽";
	}
}

#pragma endregion

#pragma region FloorManager

void FloorManager::Start()
{
	for (int i = 0; i <= 5; i++)
	{
		Floor newFloor;
		floors.push_back(newFloor);
	}
	for (int i = 1; i <= 5; i++)
	{
		floors[i].Start();
		floors[i].UpdateAnim(i);
	}
}

void FloorManager::SendMessage(int floorID, string message)
{
	int targetFloor1, targetFloor2;
	sscanf(message.c_str(), "%d %d", &targetFloor1, &targetFloor2);
	if (targetFloor1 > targetFloor2)
	{
		floors[floorID].DownRequest = true;
	}
	else
	{
		floors[floorID].UpRequest = true;
	}
	floors[floorID].UpdateAnim(floorID);
}

void FloorManager::SendMessage(int floorID, ElevatorStates state)
{
	if (state == ElevatorStates::Up)
	{
		floors[floorID].UpRequest = false;
	}
	else
	{
		floors[floorID].DownRequest = false;
	}
	floors[floorID].UpdateAnim(floorID);
}

#pragma endregion

#pragma region RulesMnager

void RulesManager::CheckRules(int list)
{
	if (!ruleFinished[list])
	{
		gotoXY(list * 2 + 29, 2);
		cout << "√";
		ruleFinished[list] = true;
		numOfFinishedRules++;
		gotoXY(list * 2 + 29, 75);
		cout << Timer;
	}
}

void RulesManager::Start()
{
	numOfFinishedRules = 0;
	memset(ruleFinished, 0, sizeof(bool));
}

#pragma endregion

#pragma region Monobehavior

bool Start()
{
	HideCursor();
	system("cls");
	ifstream infile("./UI", std::ios::in);
	string line;
	if (infile.is_open())
	{
		while (getline(infile, line))
		{
			cout << line << endl;
		}
	}
	else
	{
		return false;
	}
	floorManager.Start();
	elevator.Start();
	messageManager.Start();
	rulesManager.Start();
	Timer = 0;
	return true;
}

bool Update()
{

	if (Timer % elevatorTickTime == 0)
	{
		elevator.UpdateLogic();
		elevator.UpdateAnim();
	}
	if (Timer % guestGenerateTime == 0)
	{
		guestManager.GenerateGuest();
	}
	guestManager.UpdateLogic();
	guestManager.UpdateAnim();
	guestManager.UpdateGuestLitsAnim();
	Tick();
	if (!rulesManager.Check())
		return true;
	else
		return false;
}

void Exit()
{
	ShowCursor();
}

#pragma endregion

#pragma region main
int main()
{
	if (Start())
	{
		while (Update());
	}
	Exit();
}
#pragma endregion