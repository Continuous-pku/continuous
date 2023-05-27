#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<iostream>
#include<cstring>
#include<iomanip>
#include<cmath>
#include<list>
#include <string>
#include <vector>
using namespace std;
class Game;
class Person;
class Policy;
class Building;
class Affair;

class Hospital; // 校医院类
class Library; // 图书馆类
class Gate; // 学校校门类
class Laboratory; // 实验楼类
class Dormitory; // 学生宿舍类
class TeachingBuilding; // 教学楼
class Playground; // 操场
class Gymnasium; // 体育馆
class DiningHall; // 食堂
class Lake; // 未名湖

class RequireMasks; // 要求佩戴口罩 编号：0
class MeasureTemperatureAndScanCode; // 测温扫码 编号：1
class ShutDownBuilding; // 关闭建筑 编号：2
class AddBaffle; // 设置挡板 编号：3
class StopDineIn; // 停止堂食 编号：4
class TemporaryLockdown; // 临时封控 编号：5
class ShutDownDormitory; // 封闭宿舍 编号：6


class CoupleByLake; // 事件：湖边的小情侣
class ReadByLake; // 事件：未名湖边好读书

class SchoolWidePCRTest; // 学校政策：全民核酸检测
class ReseachOnVirus; // 学校政策：科研专项攻关
class SuspendCampusTour; // 学校政策：暂停校园参观
class PostponeStartOfSchool; // 学校政策：推迟开学
class ProvideDesktopVideo; // 学校政策：提供桌面视频会议软件
class StartOnlineCourse; // 学校政策：开始网课
class StartPF; // 学校政策：开始PF制

class Game {//主游戏 类
private:
    int year;//年
    int month;//月
    int day;//日
    int hour;//小时
    double overall_satisfaction;//总满意度
    int activity_point;//行动力
    int sum_healthy_person;//总健康人数
    int sum_ill_person;//总感染人数
    int sum_quarantine_person;//总隔离人数
    int sum_building;//总建筑数
    Policy* all_policy[1000];//存储所有的政策
    Person* all_person[40000];//存储所有的人
    Building* all_building[100];//存储所有的建筑
public:
    Game();
    void gameRun();
    bool gameWin();
    bool gameLose();
    bool gameEnd();
    void startSpread(); // 开始传播
    void nextDay(); // 获得下一天
    void updateTrack();
};
class Person {//人 类
    friend class Game;
private:
    int health_state;//健康状态 0 for 健康 1 for 感染 2 for 拉走隔离
    bool is_now_infected;
    double satisfaction;//个人满意度
    int track[24];//行动轨迹，24小时所在的建筑编号
public:
    Person();
    void updateSatisfaction(double delta);
};
class Building
{
private:
    int level; // 建筑物当前的等级

    Policy* available_policy[10]; // 可用的政策的编号
    int total_number_of_available; // 可用政策的总数
    int policy_state[10]; // 0表示不可用，1表示可用，2表示已颁布
    int kind; // 建筑物的种类
    int idx; // 建筑物的编号
    double change_in_infection_likelihood; // 传染概率的改变量，受到当前建筑物等级、政策和人数的影响
    int action_point_per_day;

    bool is_shut_down; // 是否被关闭
    int total_people; // 当前建筑物内的人员总数
    int total_infectd_people; // 当前建筑物内所有被感染的人数
    int upgrade_cost[3];
    Person* people_in_building[40000]; // 当前在这个建筑里的人的指针
public:
    friend class Game;
    friend class Policy;
    Building(int _kind, int _idx);
    void upgrade(); // 升级建筑
    void enactPolcy(int policy_idx); // 颁布在available_policy中编号为policy_idx的政策
    int produceActionPoint(); // 每天结算时产生行动力
    void updateChange(double delta); // 改变传染概率的改变量
    void updateActionPoint(int delta); // 改变每天产生的行动力
    void shutDown(); // 关闭当前建筑
    bool isShutDown(); // 当前建筑是否被关闭
    virtual void setInitialPolicy(); // 设置初始政策
    void addPolicy(Policy* p); // 向可用政策列表中加入政策
    void updatePolicyState(); // 初始化所有的政策可用状态
    void peopleArrived(int delta, Person* p); // 有delta个人抵达了当前的建筑物
    void infectedPeopleArrived(int delta); // 有delta个感染者抵达了当前的建筑
    int getLevel(); // 得到当前建筑物的等级
    virtual void addBaffle();
    int getInfectedNumber(); // 返回当前建筑物内被感染的总人数
    int getTotalNumber(); // 返回当前建筑物内的总人数
    virtual void checkAffairHappen(); // 判断当前的建筑物是否会有特殊事件发生
    void updateSatisfaction(double delta); // 改版当前在这个建筑里的人的满意度
    bool isDormitoryShutDown(); // 这个建筑是宿舍的话是否被关闭了
};

class DiningHall : public Building
{
private:
    bool added_baffle; // 是否增设挡板
public:
    DiningHall(int _idx, int _kind);
    void setInitialPolicy(); // 设置食堂的初始政策
    void addBaffle();
};
class TeachingBuilding : public Building
{
public:
    TeachingBuilding(int _idx, int _kind);
    void setInitialPolicy(); // 设置教学楼的初始政策
};
class Lake : public Building
{
private:
    Affair* possible_affairs[3];
    int number_of_possible_affairs;
public:
    Lake(int _idx, int _kind);
    void checkAffairHappen();
};
class Gymnasium : public Building
{
public:
    Gymnasium(int _idx, int _kind);
};
class Playground : public Building
{
public:
    Playground(int _idx, int _kind);
};
class Library : public Building
{
public:
    Library(int _idx, int _kind);
};
class Gate : public Building
{
public:
    Gate(int _idx, int _kind);
};
class Laboratory : public Building
{
public:
    Laboratory(int _idx, int _kind);
};
class Hospital : public Building
{
public:
    Hospital(int _idx, int _kind);
};
class Dormitory : public Building
{
public:
    Dormitory(int _idx, int _kind);
    void setInitialPolicy();
};

class Policy
{
    friend class Game;
private:
    int idx; // 政策的编号
    int cost;
    Building* building_attached; // 是附着在哪个building上的
public:
    friend class Game;
    Policy(int _idx, int _cost, Building* _b);
    static Policy* all_policy[50];
    // 每个建筑的政策的指针不能指向同一个地方
    virtual void takeEffect(); // 政策生效
    virtual void printPolicy(); // 打印政策
    virtual bool available();
    Building* BuildingAttached();
};
class RequireMasks : public Policy
{
public:
    RequireMasks(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};
class MeasureTemperatureAndScanCode : public Policy
{
public:
    MeasureTemperatureAndScanCode(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};
class ShutDownBuilding : public Policy
{
public:
    ShutDownBuilding(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};
class AddBaffle : public Policy
{
public:
    AddBaffle(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};
class StopDineIn : public Policy
{
public:
    StopDineIn(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};
class TemporaryLockdown : public Policy
{
public:
    TemporaryLockdown(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};
class ShutDownDormitory : public Policy
{
public:
    ShutDownDormitory(int _idx, int _cost, Building* _b);
    void takeEffect();
    bool available();
    void printPolicy();
};

class Affair
{
private:
    int idx; // 事件的编号
    double probability_of_occurrence; // 当前事件发生的概率
    Building* happened_place; // 发生地点

public:
    Affair(int _idx, double _p, Building* _b);
    virtual void takeEffect(); // 这个事件发生了作用
    virtual void printAffair(); // 打印当前的事件
    virtual void happen(); // 这个事件是否发生了
    double getProb(); // 返回概率
    Building* getPlace();
};

class CoupleByLake : public Affair
{
public:
    CoupleByLake(int _idx, double _p, Building* _b);
    void happen();
    void takeEffect();
    void printAffair();
};
class ReadByLake : public Affair
{
public:
    ReadByLake(int _idx, double _p, Building* _b);
    void happen();
    void takeEffect();
    void printAffair();
};


