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

class Hospital; // УҽԺ��
class Library; // ͼ�����
class Gate; // ѧУУ����
class Laboratory; // ʵ��¥��
class Dormitory; // ѧ��������
class TeachingBuilding; // ��ѧ¥
class Playground; // �ٳ�
class Gymnasium; // ������
class DiningHall; // ʳ��
class Lake; // δ����

class RequireMasks; // Ҫ��������� ��ţ�0
class MeasureTemperatureAndScanCode; // ����ɨ�� ��ţ�1
class ShutDownBuilding; // �رս��� ��ţ�2
class AddBaffle; // ���õ��� ��ţ�3
class StopDineIn; // ֹͣ��ʳ ��ţ�4
class TemporaryLockdown; // ��ʱ��� ��ţ�5
class ShutDownDormitory; // ������� ��ţ�6


class CoupleByLake; // �¼������ߵ�С����
class ReadByLake; // �¼���δ�����ߺö���

class SchoolWidePCRTest; // ѧУ���ߣ�ȫ�������
class ReseachOnVirus; // ѧУ���ߣ�����ר���
class SuspendCampusTour; // ѧУ���ߣ���ͣУ԰�ι�
class PostponeStartOfSchool; // ѧУ���ߣ��Ƴٿ�ѧ
class ProvideDesktopVideo; // ѧУ���ߣ��ṩ������Ƶ�������
class StartOnlineCourse; // ѧУ���ߣ���ʼ����
class StartPF; // ѧУ���ߣ���ʼPF��

class Game {//����Ϸ ��
private:
    int year;//��
    int month;//��
    int day;//��
    int hour;//Сʱ
    double overall_satisfaction;//�������
    int activity_point;//�ж���
    int sum_healthy_person;//�ܽ�������
    int sum_ill_person;//�ܸ�Ⱦ����
    int sum_quarantine_person;//�ܸ�������
    int sum_building;//�ܽ�����
    Policy* all_policy[1000];//�洢���е�����
    Person* all_person[40000];//�洢���е���
    Building* all_building[100];//�洢���еĽ���
public:
    Game();
    void gameRun();
    bool gameWin();
    bool gameLose();
    bool gameEnd();
    void startSpread(); // ��ʼ����
    void nextDay(); // �����һ��
    void updateTrack();
};
class Person {//�� ��
    friend class Game;
private:
    int health_state;//����״̬ 0 for ���� 1 for ��Ⱦ 2 for ���߸���
    bool is_now_infected;
    double satisfaction;//���������
    int track[24];//�ж��켣��24Сʱ���ڵĽ������
public:
    Person();
    void updateSatisfaction(double delta);
};
class Building
{
private:
    int level; // �����ﵱǰ�ĵȼ�

    Policy* available_policy[10]; // ���õ����ߵı��
    int total_number_of_available; // �������ߵ�����
    int policy_state[10]; // 0��ʾ�����ã�1��ʾ���ã�2��ʾ�Ѱ䲼
    int kind; // �����������
    int idx; // ������ı��
    double change_in_infection_likelihood; // ��Ⱦ���ʵĸı������ܵ���ǰ������ȼ������ߺ�������Ӱ��
    int action_point_per_day;

    bool is_shut_down; // �Ƿ񱻹ر�
    int total_people; // ��ǰ�������ڵ���Ա����
    int total_infectd_people; // ��ǰ�����������б���Ⱦ������
    int upgrade_cost[3];
    Person* people_in_building[40000]; // ��ǰ�������������˵�ָ��
public:
    friend class Game;
    friend class Policy;
    Building(int _kind, int _idx);
    void upgrade(); // ��������
    void enactPolcy(int policy_idx); // �䲼��available_policy�б��Ϊpolicy_idx������
    int produceActionPoint(); // ÿ�����ʱ�����ж���
    void updateChange(double delta); // �ı䴫Ⱦ���ʵĸı���
    void updateActionPoint(int delta); // �ı�ÿ��������ж���
    void shutDown(); // �رյ�ǰ����
    bool isShutDown(); // ��ǰ�����Ƿ񱻹ر�
    virtual void setInitialPolicy(); // ���ó�ʼ����
    void addPolicy(Policy* p); // ����������б��м�������
    void updatePolicyState(); // ��ʼ�����е����߿���״̬
    void peopleArrived(int delta, Person* p); // ��delta���˵ִ��˵�ǰ�Ľ�����
    void infectedPeopleArrived(int delta); // ��delta����Ⱦ�ߵִ��˵�ǰ�Ľ���
    int getLevel(); // �õ���ǰ������ĵȼ�
    virtual void addBaffle();
    int getInfectedNumber(); // ���ص�ǰ�������ڱ���Ⱦ��������
    int getTotalNumber(); // ���ص�ǰ�������ڵ�������
    virtual void checkAffairHappen(); // �жϵ�ǰ�Ľ������Ƿ���������¼�����
    void updateSatisfaction(double delta); // �İ浱ǰ�������������˵������
    bool isDormitoryShutDown(); // �������������Ļ��Ƿ񱻹ر���
};

class DiningHall : public Building
{
private:
    bool added_baffle; // �Ƿ����赲��
public:
    DiningHall(int _idx, int _kind);
    void setInitialPolicy(); // ����ʳ�õĳ�ʼ����
    void addBaffle();
};
class TeachingBuilding : public Building
{
public:
    TeachingBuilding(int _idx, int _kind);
    void setInitialPolicy(); // ���ý�ѧ¥�ĳ�ʼ����
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
    int idx; // ���ߵı��
    int cost;
    Building* building_attached; // �Ǹ������ĸ�building�ϵ�
public:
    friend class Game;
    Policy(int _idx, int _cost, Building* _b);
    static Policy* all_policy[50];
    // ÿ�����������ߵ�ָ�벻��ָ��ͬһ���ط�
    virtual void takeEffect(); // ������Ч
    virtual void printPolicy(); // ��ӡ����
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
    int idx; // �¼��ı��
    double probability_of_occurrence; // ��ǰ�¼������ĸ���
    Building* happened_place; // �����ص�

public:
    Affair(int _idx, double _p, Building* _b);
    virtual void takeEffect(); // ����¼�����������
    virtual void printAffair(); // ��ӡ��ǰ���¼�
    virtual void happen(); // ����¼��Ƿ�����
    double getProb(); // ���ظ���
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


