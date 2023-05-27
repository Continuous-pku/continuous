#define _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<iostream>
#include<cstring>
#include<iomanip>
#include<cmath>
#include<list>
#include<random>
#include"class.h"
using namespace std;

random_device seed;//硬件生成随机数种子
ranlux48 engine(seed());//用种子生成随机数引擎


//Game 类 | 主游戏 类
Game::Game() {
	time_t now_time;
	time(&now_time);//获取从1900年1月1日0分0秒到现在经过的秒数
	tm* p = localtime(&now_time);//将秒数转换为本地时间，年从1900算起，需要+1900，月为0~11，所以要+1
	year = p->tm_year + 1900;//初始化年为现实当前年份
	month = p->tm_mon + 1;//初始化月为现实当前月
	day = p->tm_mday;//初始化日为现实当前日
	hour = 0;//初始化小时为0时
	overall_satisfaction = 90;//初始化满意度为0
	activity_point = 0;//初始化行动力为0
	sum_healthy_person = 40000;
	sum_ill_person = sum_quarantine_person = 0;
	sum_building = 71;
	//初始化all_building
	/*
	kind:
	0 学生宿舍
	1 食堂
	2 教学楼
	3 图书馆
	4 校医院
	5 操场
	6 体育馆
	7 未名湖
	8 实验楼
	9 学校校门
	code:
	0~44    45个宿舍(0~44)
	45~50  食堂
	45 农园
	46 艺园
	47 家园
	48 勺园
	49 学一
	50 学五
	51~57  教学楼
	51 一教
	52 二教
	53 三教
	54 四教
	55 理教
	56 文史
	57 地学
	58 图书馆
	59 校医院
	60 五四操场
	61 五四体育馆
	62 邱徳拔体育馆
	63 未名湖
	64 理科实验楼
	65 东门
	66 南门
	67 西门
	68 西南门
	69 东南门
	70 东北门
	*/
	for (int i = 0; i < sum_building; i++) {
		if (i <= 44) {
			all_building[i] = new Dormitory(0, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i >= 45 && i <= 50) {
			all_building[i] = new DiningHall(1, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i >= 51 && i <= 57) { 
			all_building[i] = new TeachingBuilding(2, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i == 58) { 
			all_building[i] = new Library(3, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i == 59) { 
			all_building[i] = new Hospital(4, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i == 60) { 
			all_building[i] = new Playground(5, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i >= 61 && i <= 62) { 
			all_building[i] = new Gymnasium(6, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i == 63) { 
			all_building[i] = new Lake(7, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i == 64) { 
			all_building[i] = new Laboratory(8, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
		if (i >= 65 && i <= 70) { 
			all_building[i] = new Gate(9, i);
			all_building[i]->setInitialPolicy(); // 设置初始政策
		}
	}
	//初始化all_person
	for (int i = 0; i < 40000; i++) {
		all_person[i] = new Person();
		double P;
		//0~6点，100%在宿舍睡觉
		uniform_int_distribution<> int_distrib(0, 44);
		int temp_dormitory = int_distrib(engine);
		for (int j = 0; j <= 6; j++) {
			all_person[i]->track[j] = temp_dormitory;
		}
		//7点，50%去吃早饭，50%宿舍摆烂
		uniform_int_distribution<> int_distrib1(45, 50);
		int temp_morning_dininghall = int_distrib1(engine);
		uniform_real_distribution<> real_distrib(0, 1);
		P = real_distrib(engine);
		if (P >= 0.5) { all_person[i]->track[7] = temp_morning_dininghall; }
		else { all_person[i]->track[7] = temp_dormitory; }
		//8~9点，40%教学楼上课，30%宿舍，20%图书馆，10%未名湖
		uniform_int_distribution<> int_distrib2(51, 57);
		int temp_teachingbuilding1 = int_distrib2(engine);
		uniform_real_distribution<> real_distrib1(0, 1);
		P = real_distrib1(engine);
		for (int j = 8; j <= 9; j++) {
			if (0 <= P && P < 0.4) { all_person[i]->track[j] = temp_teachingbuilding1; }
			else if (0.4 <= P && P < 0.7) { all_person[i]->track[j] = temp_dormitory; }
			else if (0.7 <= P && P < 0.9) { all_person[i]->track[j] = 58; }//图书馆
			else if (0.9 <= P && P <= 1.0) { all_person[i]->track[j] = 63; }//未名湖
		}
		//10~11点，40%教学楼上课，20%宿舍，15%图书馆，10%食堂，10%未名湖，5%体育馆
		uniform_int_distribution<> int_distrib3(51, 57);
		int temp_teachingbuilding2 = int_distrib3(engine);
		uniform_int_distribution<> int_distrib4(45, 50);
		int temp_lunchbuilding1 = int_distrib4(engine);
		uniform_int_distribution<> int_distrib5(61, 62);
		int temp_gymnasium1 = int_distrib5(engine);
		uniform_real_distribution<> real_distrib2(0, 1);
		P = real_distrib2(engine);
		for (int j = 10; j <= 11; j++) {
			if (0 <= P && P < 0.4) { all_person[i]->track[j] = temp_teachingbuilding2; }
			else if (0.4 <= P && P < 0.6) { all_person[i]->track[j] = temp_dormitory; }
			else if (0.6 <= P && P < 0.75) { all_person[i]->track[j] = 58; }//图书馆
			else if (0.75 <= P && P < 0.85) { all_person[i]->track[j] = temp_lunchbuilding1; }
			else if (0.85 <= P && P < 0.95) { all_person[i]->track[j] = 63; }//未名湖
			else if (0.95 <= P && P <= 1.0) { all_person[i]->track[j] = temp_gymnasium1; }
		}
		//12点，40%食堂午饭，15%教学楼，20%宿舍，10%图书馆，12%未名湖，3%体育馆
		uniform_int_distribution<> int_distrib6(45, 50);
		int temp_lunchbuilding2 = int_distrib6(engine);
		uniform_int_distribution<> int_distrib7(51, 57);
		int temp_teachingbuilding3 = int_distrib7(engine);
		uniform_int_distribution<> int_distrib8(61, 62);
		int temp_gymnasium2 = int_distrib8(engine);
		uniform_real_distribution<> real_distrib3(0, 1);
		P = real_distrib3(engine);
		if (0 <= P && P < 0.4) { all_person[i]->track[12] = temp_lunchbuilding2; }
		else if (0.4 <= P && P < 0.55) { all_person[i]->track[12] = temp_teachingbuilding3; }
		else if (0.55 <= P && P < 0.75) { all_person[i]->track[12] = temp_dormitory; }
		else if (0.75 <= P && P < 0.85) { all_person[i]->track[12] = 58; }//图书馆
		else if (0.85 <= P && P < 0.97) { all_person[i]->track[12] = 63; }//未名湖
		else if (0.97 <= P && P <= 1.0) { all_person[i]->track[12] = temp_gymnasium2; }
		//13~14点，40%教学楼，20%图书馆，20%宿舍，10%食堂，5%未名湖，3%体育馆，2%操场
		uniform_int_distribution<> int_distrib9(51, 57);
		int temp_teachingbuilding4 = int_distrib9(engine);
		uniform_int_distribution<> int_distrib10(45, 50);
		int temp_lunchbuilding3 = int_distrib10(engine);
		uniform_int_distribution<> int_distrib11(61, 62);
		int temp_gymnasium3 = int_distrib11(engine);
		uniform_real_distribution<> real_distrib4(0, 1);
		P = real_distrib4(engine);
		for (int j = 13; j <= 14; j++) {
			if (0 <= P && P < 0.4) { all_person[i]->track[j] = temp_teachingbuilding4; }
			else if (0.4 <= P && P < 0.6) { all_person[i]->track[j] = 58; }//图书馆
			else if (0.6 <= P && P < 0.8) { all_person[i]->track[j] = temp_dormitory; }
			else if (0.8 <= P && P < 0.9) { all_person[i]->track[j] = temp_lunchbuilding3; }
			else if (0.9 <= P && P < 0.95) { all_person[i]->track[j] = 63; }//未名湖
			else if (0.95 <= P && P < 0.98) { all_person[i]->track[j] = temp_gymnasium3; }
			else if (0.98 <= P && P <= 1.0) { all_person[i]->track[j] = 60; }//操场
		}
		//15~16点，40%教学楼，20%图书馆，20%宿舍，10%食堂，5%未名湖，3%体育馆，2%操场
		uniform_int_distribution<> int_distrib12(51, 57);
		int temp_teachingbuilding5 = int_distrib12(engine);
		uniform_int_distribution<> int_distrib13(45, 50);
		int temp_lunchbuilding4 = int_distrib13(engine);
		uniform_int_distribution<> int_distrib14(61, 62);
		int temp_gymnasium4 = int_distrib14(engine);
		uniform_real_distribution<> real_distrib5(0, 1);
		P = real_distrib5(engine);
		for (int j = 15; j <= 16; j++) {
			if (0 <= P && P < 0.4) { all_person[i]->track[j] = temp_teachingbuilding5; }
			else if (0.4 <= P && P < 0.6) { all_person[i]->track[j] = 58; }//图书馆
			else if (0.6 <= P && P < 0.8) { all_person[i]->track[j] = temp_dormitory; }
			else if (0.8 <= P && P < 0.9) { all_person[i]->track[j] = temp_lunchbuilding4; }
			else if (0.9 <= P && P < 0.95) { all_person[i]->track[j] = 63; }//未名湖
			else if (0.95 <= P && P < 0.98) { all_person[i]->track[j] = temp_gymnasium4; }
			else if (0.98 <= P && P <= 1.0) { all_person[i]->track[j] = 60; }//操场
		}
		//17点，45%食堂晚饭，15%教学楼，15%图书馆，15%宿舍，5%操场，3%未名湖，2%体育馆
		uniform_int_distribution<> int_distrib15(51, 57);
		int temp_teachingbuilding6 = int_distrib15(engine);
		uniform_int_distribution<> int_distrib16(45, 50);
		int temp_supperbuilding1 = int_distrib16(engine);
		uniform_int_distribution<> int_distrib17(61, 62);
		int temp_gymnasium5 = int_distrib17(engine);
		uniform_real_distribution<> real_distrib6(0, 1);
		P = real_distrib6(engine);
		if (0 <= P && P < 0.45) { all_person[i]->track[17] = temp_supperbuilding1; }
		else if (0.45 <= P && P < 0.6) { all_person[i]->track[17] = temp_teachingbuilding6; }
		else if (0.6 <= P && P < 0.75) { all_person[i]->track[17] = 58; }//图书馆
		else if (0.75 <= P && P < 0.9) { all_person[i]->track[17] = temp_dormitory; }
		else if (0.9 <= P && P < 0.95) { all_person[i]->track[17] = 60; }//操场
		else if (0.95 <= P && P < 0.98) { all_person[i]->track[17] = 63; }//未名湖
		else if (0.98 <= P && P <= 1.0) { all_person[i]->track[17] = temp_gymnasium5; }
		//18点，30%食堂晚饭，20%教学楼，20%图书馆，15%宿舍，5%操场，5%未名湖，5%体育馆
		uniform_int_distribution<> int_distrib18(51, 57);
		int temp_teachingbuilding7 = int_distrib18(engine);
		uniform_int_distribution<> int_distrib19(45, 50);
		int temp_supperbuilding2 = int_distrib19(engine);
		uniform_int_distribution<> int_distrib20(61, 62);
		int temp_gymnasium6 = int_distrib20(engine);
		uniform_real_distribution<> real_distrib7(0, 1);
		P = real_distrib7(engine);
		if (0 <= P && P < 0.3) { all_person[i]->track[18] = temp_supperbuilding2; }
		else if (0.3 <= P && P < 0.5) { all_person[i]->track[18] = temp_teachingbuilding7; }
		else if (0.5 <= P && P < 0.7) { all_person[i]->track[18] = 58; }//图书馆
		else if (0.7 <= P && P < 0.85) { all_person[i]->track[18] = temp_dormitory; }
		else if (0.85 <= P && P < 0.9) { all_person[i]->track[18] = 60; }//操场
		else if (0.9 <= P && P < 0.95) { all_person[i]->track[18] = 63; }//未名湖
		else if (0.95 <= P && P <= 1.0) { all_person[i]->track[18] = temp_gymnasium6; }
		//19~20点，40%教学楼，20%图书馆，20%宿舍，6%操场，7%未名湖，7%体育馆
		uniform_int_distribution<> int_distrib21(51, 57);
		int temp_teachingbuilding8 = int_distrib21(engine);
		uniform_int_distribution<> int_distrib22(61, 62);
		int temp_gymnasium7 = int_distrib22(engine);
		uniform_real_distribution<> real_distrib8(0, 1);
		P = real_distrib8(engine);
		for (int j = 19; j <= 20; j++) {
			if (0 <= P && P < 0.4) { all_person[i]->track[j] = temp_teachingbuilding8; }
			else if (0.4 <= P && P < 0.6) { all_person[i]->track[j] = 58; }//图书馆
			else if (0.6 <= P && P < 0.8) { all_person[i]->track[j] = temp_dormitory; }
			else if (0.8 <= P && P < 0.86) { all_person[i]->track[j] = 60; }//操场
			else if (0.86 <= P && P < 0.93) { all_person[i]->track[j] = 63; }//未名湖
			else if (0.93 <= P && P <= 1.0) { all_person[i]->track[j] = temp_gymnasium7; }
		}
		//21点，20%教学楼，40%宿舍，8%操场，8%未名湖，9%体育馆，15%图书馆
		uniform_int_distribution<> int_distrib23(51, 57);
		int temp_teachingbuilding9 = int_distrib23(engine);
		uniform_int_distribution<> int_distrib24(61, 62);
		int temp_gymnasium8 = int_distrib24(engine);
		uniform_real_distribution<> real_distrib9(0, 1);
		P = real_distrib9(engine);
		if (0 <= P && P < 0.2) { all_person[i]->track[21] = temp_teachingbuilding9; }
		else if (0.2 <= P && P < 0.6) { all_person[i]->track[21] = temp_dormitory; }
		else if (0.6 <= P && P < 0.68) { all_person[i]->track[21] = 60; }//操场
		else if (0.68 <= P && P < 0.76) { all_person[i]->track[21] = 63; }//未名湖
		else if (0.76 <= P && P < 0.85) { all_person[i]->track[21] = temp_gymnasium8; }
		else if (0.85 <= P && P <= 1.0) { all_person[i]->track[21] = 58; }//图书馆
		//22点，10%未名湖，7%体育馆，50%宿舍，15%图书馆，10%教学楼，8%操场
		uniform_int_distribution<> int_distrib25(51, 57);
		int temp_teachingbuilding10 = int_distrib25(engine);
		uniform_int_distribution<> int_distrib26(61, 62);
		int temp_gymnasium9 = int_distrib26(engine);
		uniform_real_distribution<> real_distrib10(0, 1);
		P = real_distrib10(engine);
		if (0 <= P && P < 0.1) { all_person[i]->track[22] = 63; }//未名湖
		else if (0.1 <= P && P < 0.17) { all_person[i]->track[22] = temp_gymnasium9; }
		else if (0.17 <= P && P < 0.67) { all_person[i]->track[22] = temp_dormitory; }
		else if (0.67 <= P && P < 0.82) { all_person[i]->track[22] = 58; }//图书馆
		else if (0.82 <= P && P < 0.92) { all_person[i]->track[22] = temp_teachingbuilding10; }
		else if (0.92 <= P && P <= 1.0) { all_person[i]->track[22] = 60; }//操场
		//23点，95%宿舍，5%未名湖
		uniform_real_distribution<> real_distrib11(0, 1);
		P = real_distrib11(engine);
		if (0 <= P && P < 0.95) { all_person[i]->track[23] = temp_dormitory; }
		else if (0.95 <= P && P <= 1.0) { all_person[i]->track[23] = 63; }//未名湖
	}
}
void Game::gameRun() {
	//首先进行基础数据的初始化
	activity_point = 0;//初始化行动力
	overall_satisfaction = 90;//初始化总满意度
	
	//初始化all_policy
	uniform_int_distribution<> fucking(0, 40000);
	int intitial_infected_idx = fucking(engine);
	all_person[intitial_infected_idx]->health_state = 1;
	sum_healthy_person--;
	sum_ill_person++;
	while (!gameEnd()) {
		printf("%d年%d月%d日\n", year, month, day);
		cout << "总健康人数：" << sum_healthy_person << ' ' << "总感染人数：" << sum_ill_person << ' ' << "总隔离人数：" << sum_quarantine_person << endl;
		printf("当前学生满意度:%.2lf\n", overall_satisfaction);
		printf("当前的总行动力：%d\n", activity_point);
		system("pause");

		cout << "政策情况如下：" << endl;
		for (int building_index = 0; building_index < sum_building; building_index++) {
			cout << "building_index：" << building_index << endl;
			printf("当前building的level是：%d\n", all_building[building_index]->getLevel());
			for (int policy_index = 0; policy_index < all_building[building_index]->total_number_of_available; policy_index++) {
				cout << "policy_index：" << setw(3) << policy_index << ' ';
				switch (all_building[building_index]->policy_state[policy_index]) {
				case 0:
					cout << "不可用：";
					break;
				case 1:
					cout << "  可用：";
					break;
				case 2:
					cout << "已颁布：";
					break;
				}
				// 这里写政策情况的输出
				all_building[building_index]->available_policy[policy_index]->printPolicy();
			}
			// 开始发生随机事件
			all_building[building_index]->checkAffairHappen();
		}
		// 这个地方在qt中使用点击的方式吧
		printf("请输入1表示要开始选择建筑进行升级！\n");
		int opt;
		cin >> opt;
		if (opt == 1) {
			printf("建筑的编号范围是0~70，请输入一个编号选择哦！输入不在这个范围内视为您不想升级哦！\n");
			int temp_idx;
			while (cin >> temp_idx) {
				if (temp_idx < 0 || temp_idx > 70)
					break;
				if (activity_point < all_building[temp_idx]->upgrade_cost[all_building[temp_idx]->level]) {
					printf("您有的行动力无法升级当前建筑哦\n");
				}
				else if (all_building[temp_idx]->level == 3) {
					printf("当前的建筑已经满级啦\n");
				}
				else {
					activity_point -= all_building[temp_idx]->upgrade_cost[all_building[temp_idx]->level];
					all_building[temp_idx]->upgrade();
				}
			}
			printf("建筑升级环节已经结束了！\n");
		}
		// 下面写选择建筑政策进行升级的东西
		// 建筑选用政策后有可能会导致建筑被关闭，要更新新的轨迹欸
		bool is_building_shut_down = false;
		printf("请输入1进入政策升级界面\n");
		cin >> opt;
		if (opt == 1) {
			printf("请先输入建筑物的编号，选择一个建筑，建筑的编号为0-70\n");
			int building_idx;
			while (cin >> building_idx) {
				if (building_idx < 0 || building_idx > 70)
					break;
				printf("下面是这个建筑的政策情况，请选择一个政策实行：\n");
				for (int policy_idx = 0; policy_idx < all_building[building_idx]->total_number_of_available; policy_idx++) {
					cout << "policy_index：" << setw(3) << policy_idx << ' ';
					switch (all_building[building_idx]->policy_state[policy_idx]) {
					case 0:
						cout << "不可用：";
						break;
					case 1:
						cout << "  可用：";
						break;
					case 2:
						cout << "已颁布：";
						break;
					}
					all_building[building_idx]->available_policy[policy_idx]->printPolicy();
				}
				int policy_idx;
				printf("请您输入您需要选用的政策编号，不在正常范围内视为退出咯\n");
				while (cin >> policy_idx) {
					if (policy_idx >= all_building[building_idx]->total_number_of_available || policy_idx < 0) {
						break;
					}
					else if (all_building[building_idx]->policy_state[policy_idx] == 0) {
						printf("当前政策还不可用哦\n");
					}
					else if (all_building[building_idx]->policy_state[policy_idx] == 2) {
						printf("当前政策已经颁布啦\n");
					}
					else {
						if (activity_point < all_building[building_idx]->available_policy[policy_idx]->cost) {
							printf("您当前的点数不够辣\n");
						}
						else {
							activity_point -= all_building[building_idx]->available_policy[policy_idx]->cost;
							all_building[building_idx]->enactPolcy(policy_idx);
							if (all_building[building_idx]->isShutDown())
								is_building_shut_down = true;
						}
					}
				}
			}
		}
		printf("选择颁布政策的环节已经结束啦\n");

		if (is_building_shut_down)
			updateTrack(); // 如果有建筑被关闭，选择更新学生的轨迹，让所有学生无法再进入这个建筑了，宿舍要反过来不能再出去了

		startSpread();
		cout << "总健康人数：" << sum_healthy_person << ' ' << "总感染人数：" << sum_ill_person << ' ' << "总隔离人数：" << sum_quarantine_person << endl;
		int temp_satisfaction = 0;
		int temp_total_person = 0;
		for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
			// 不知道已经被隔离的满意度是否要被计算？
			temp_satisfaction += all_person[i]->satisfaction;
			temp_total_person++;
		}
		overall_satisfaction = temp_satisfaction * 1.0 / temp_total_person;
		for (int i = 0; i < sum_building; i++) {
			activity_point += all_building[i]->produceActionPoint();
		}
		nextDay();
	}
}
bool Game::gameEnd() {
	if (gameWin())
		cout << "win!" << endl;
	if (gameLose())
		cout << "lose!" << endl;
	return gameWin() || gameLose();
}
bool Game::gameWin() {
	//若总感染人数归零，且总健康人数不为零，则游戏胜利
	bool ret = true;
	for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
		if (all_person[i]->health_state == 1) { ret = false; }
	}
	if (sum_healthy_person == 0) { ret = false; }
	return ret;
}
bool Game::gameLose() {
	//若总健康人数归零，则游戏失败
	bool ret = true;
	for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
		if (all_person[i]->health_state == 0) { ret = false; }
	}
	return ret;
}
void Game::startSpread() {
	// 开始进行当天的传播
	for (int h = 0; h < 24; h++) {
		int new_infected = 0; // 新增感染者的数量
		// 首先枚举当前的时间点
		for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
			// 再枚举所有还可以移动的学生
			if (all_person[i]->health_state == 2)
				continue;
			// 被拉走隔离了不能再去建筑了
			// 但是如果这个学生要去的建筑被关闭了怎么办？这个待会再想一下！

			all_building[all_person[i]->track[h]]->peopleArrived(1, all_person[i]); // 一个学生到达了这建筑
			if (all_person[i]->health_state == 1)
				all_building[all_person[i]->track[h]]->infectedPeopleArrived(1); // 一个被感染的学生抵达了当前的建筑物
		}
		for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
			if (all_person[i]->health_state == 2 || all_person[i]->health_state == 1)
				continue;
			// 看看这个学生在当前的时间和建筑中是否会被感染
			Building* this_building = all_building[all_person[i]->track[h]];
			double p = this_building->change_in_infection_likelihood;
			int tot = this_building->getInfectedNumber();
			if (tot == 0)
				p = 0; // 当前建筑物里面没有感染者，感染概率为0
			else if (1 <= tot && tot <= 50)
				p += 0.0001; // 当前建筑物里面有1-50个感染者，感染概率+0.0001
			else if (51 <= tot && tot <= 100)
				p += 0.0002; // 当前建筑物里面有51-100个感染者，感染概率+0.0002
			else if (101 <= tot && tot <= 200)
				p += 0.0004; // 当前建筑物里面有100-200个感染者，感染概率+0.0004
			else if (201 <= tot && tot <= 300)
				p += 0.0005; // 当前建筑物里面有200-300个感染者，感染概率+0.0005
			else
				p += 0.0007; // 当前建筑物里面有300个以上的感染者，感染概率+0.0007
			uniform_real_distribution<> real_distub(0, 1);
			double rand_p = real_distub(engine);
			if (rand_p <= p) {
				all_person[i]->is_now_infected = true; // 他被感染了
				sum_ill_person++;
				sum_healthy_person--;
				new_infected++;
			}
		}
		printf("in %d o'clock, %d people was infected.\n", h, new_infected);
		for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
			// 再枚举所有还可以移动的学生
			if (all_person[i]->health_state == 2)
				continue;
			// 被拉走隔离了不能再去建筑了
			all_building[all_person[i]->track[h]]->peopleArrived(-1, all_person[i]); // 一个学生离开了这建筑
			if (all_person[i]->health_state == 1)
				all_building[all_person[i]->track[h]]->infectedPeopleArrived(-1); // 一个被感染的学生离开了当前的建筑物
			if (all_person[i]->is_now_infected) {
				all_person[i]->health_state = 1; // 他已经被感染
				all_person[i]->is_now_infected = false;
			}
		}
	}

}
void Game::nextDay() {
	// 把当前的天数往后翻一天
	int this_month_days = 30;
	if (month == 4 || month == 6 || month == 9 || month == 1)
		this_month_days = 30;
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		this_month_days = 31;
	else {
		// 当前是二月
		if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
			this_month_days = 29;
		else
			this_month_days = 28;
	}

	day++;
	if (day >= this_month_days) {
		day = 1;
		month++;
		if (month >= 12) {
			month = 1;
			year++;
		}
	}
}
void Game::updateTrack() {
	for (int i = 0; i < sum_healthy_person + sum_ill_person + sum_quarantine_person; i++) {
		int before_idx = all_person[i]->track[0]; // 先前在的建筑物的编号
		for (int h = 0; h < 24; h++) {
			if (all_building[all_person[i]->track[h]]->isDormitoryShutDown()) {
				// 把这一天所有的行程都改为在宿舍，而且退出循环
				int idx = all_person[i]->track[h];
				for (int h_hat = 0; h_hat < 24; h_hat++) {
					all_person[i]->track[h_hat] = idx;
				}
				break;
			}
			else if (all_building[all_person[i]->track[h]]->isShutDown()) {
				all_person[i]->track[h] = before_idx; // 这个人要去的建筑物被关闭了，那么他先呆在原来的建筑物当中
			}
			else {
				// 如果要去的建筑物没有被关闭
				before_idx = all_person[i]->track[h];
			}
		}
	}
}
//Person 类 | 人 类
Person::Person() {
	satisfaction = 90;
	health_state = 0;
	is_now_infected = false;
	memset(track, -1, sizeof(track));
}
void Person::updateSatisfaction(double delta) {
	satisfaction += delta;
}
Building::Building(int _kind, int _idx) {
	total_number_of_available = 0;
	memset(policy_state, 0, sizeof(policy_state)); // 一开始全都是不可用的政策
	level = 1; // 初始所有的建筑都是1级
	kind = _kind;
	idx = _idx;
	// 白板定为对传染概率无影响
	change_in_infection_likelihood = 0;
	// 一开始每个建筑产生2点
	action_point_per_day = 2;
	
	// 升级花费为10,20
	upgrade_cost[1] = 10;
	upgrade_cost[2] = 20;

	// 是否被关闭
	is_shut_down = false;
	// 一开始建筑物里面没有人
	total_people = 0;
	// 一开始建筑物里面没有感染者
	total_infectd_people = 0;
}
void Building::upgrade() {
	// 建筑升级
	level++;
	// 把所有可用的政策重新赋值
	for (int i = 0; i < total_number_of_available; i++) {
		if (policy_state[i] == 0 && available_policy[i]->available())
			policy_state[i] = 1;
	}
	updateActionPoint(1); // 升级了之后行动力也要加
	updateChange(-0.00001);
	printf("当前的建筑被升级了！从%d->%d\n", level-1, level);
}
void Building::enactPolcy(int policy_idx) {
	// 这里主游戏还要减掉政策的花费
	policy_state[policy_idx] = 2;
	available_policy[policy_idx]->takeEffect();
	printf("当前的建筑颁布了法令%d\n", policy_idx);
}
int Building::produceActionPoint() {
	printf("当前建筑产生了%d行动力\n", action_point_per_day);
	return action_point_per_day;
}
void Building::updateChange(double delta) {
	change_in_infection_likelihood += delta;
}
void Building::updateActionPoint(int delta) {
	action_point_per_day += delta;
}
void Building::shutDown() {
	try {
		if (is_shut_down == true)
			throw -1;
	}
	catch (int e) {
		printf("当前的建筑已经被关闭了两次\n");
	}
	is_shut_down = true;
	printf("当前的建筑已经被关闭了\n");
}
bool Building::isShutDown() {
	return is_shut_down;
}
void Building::addPolicy(Policy* p) {
	available_policy[total_number_of_available] = p;
	total_number_of_available++;
}
void Building::updatePolicyState() {
	// 初始所有的都不可用
	memset(policy_state, 0, sizeof(policy_state));
	// 遍历所有的政策更改当前的状态
	for (int i = 0; i < total_number_of_available; i++) {
		if (policy_state[i] == 0 && available_policy[i]->available())
			policy_state[i] = 1;
	}
}
// 在主游戏中调用这些东西
void Building::setInitialPolicy() {
	// 这是白板政策
	addPolicy(new RequireMasks(0, 10, this)); // 佩戴口罩的政策
	addPolicy(new MeasureTemperatureAndScanCode(1, 20, this)); // 测温扫码的政策
	addPolicy(new ShutDownBuilding(2, 30, this)); // 关闭建筑的政策
	updatePolicyState();
}
void Building::peopleArrived(int delta, Person* p) {
	if (delta >= 1) {
		people_in_building[total_people] = p;
	}
	else
		people_in_building[total_people] = NULL;
	total_people += delta;
}
int Building::getLevel() { return level; }
int Building::getInfectedNumber() { return total_infectd_people; }
int Building::getTotalNumber() { return total_people; }
void Building::infectedPeopleArrived(int delta) {
	total_infectd_people += delta;
}
void Building::addBaffle() { }
void Building::checkAffairHappen() { }
void Building::updateSatisfaction(double delta) {
	for (int i = 0; i < total_people; i++) {
		people_in_building[i]->updateSatisfaction(delta);
	}
}
bool Building::isDormitoryShutDown() {
	if (kind == 0 && is_shut_down)
		return true;
	return false;
}

void DiningHall::setInitialPolicy() {
	// 设置食堂的初始政策
	addPolicy(new AddBaffle(3, 20, this)); // 增设挡板的政策
	addPolicy(new RequireMasks(0, 20, this)); // 佩戴口罩的政策
	addPolicy(new MeasureTemperatureAndScanCode(1, 30, this)); // 测温扫码的政策
	addPolicy(new StopDineIn(4, 40, this)); // 停止堂食的政策
	updatePolicyState();
}
DiningHall::DiningHall(int _idx, int _kind) : Building(_idx, _kind) {
	added_baffle = false; // 初始未增设挡板
}
void DiningHall::addBaffle() {
	added_baffle = true;
	printf("食堂加挡板了！\n");
}

TeachingBuilding::TeachingBuilding(int _idx, int _kind) : Building(_idx, _kind) {
}
void TeachingBuilding::setInitialPolicy() {
	addPolicy(new RequireMasks(0, 20, this)); // 佩戴口罩的政策
	addPolicy(new MeasureTemperatureAndScanCode(1, 30, this)); // 测温扫码的政策
	addPolicy(new ShutDownBuilding(2, 40, this)); // 关闭建筑的政策
	updatePolicyState();
}


Lake::Lake(int _idx, int _kind) : Building(_idx, _kind) {
	possible_affairs[0] = new CoupleByLake(0, 0.5, this);
	possible_affairs[1] = new ReadByLake(1, 0.5, this);
	number_of_possible_affairs = 2;
}
void Lake::checkAffairHappen() {
	for (int i = 0; i < number_of_possible_affairs; i++) {
		possible_affairs[i]->happen();
	}
}

Gymnasium::Gymnasium(int _idx, int _kind) : Building(_idx, _kind) {
}
Playground::Playground(int _idx, int _kind) : Building(_idx, _kind) {
}
Library::Library(int _idx, int _kind) : Building(_idx, _kind) {
}
Gate::Gate(int _idx, int _kind) : Building(_idx, _kind) {
}
Laboratory::Laboratory(int _idx, int _kind) : Building(_idx, _kind) {
}
Hospital::Hospital(int _idx, int _kind) : Building(_idx, _kind) {
}
Dormitory::Dormitory(int _idx, int _kind) : Building(_idx, _kind) {
}
void Dormitory::setInitialPolicy() {
	addPolicy(new RequireMasks(0, 20, this)); // 佩戴口罩的政策
	addPolicy(new TemporaryLockdown(5, 30, this)); // 临时封控的政策
	addPolicy(new ShutDownDormitory(6, 40, this)); // 封闭寝室楼的政策
	updatePolicyState(); // 更新所有的政策状态
}


Building* Policy::BuildingAttached() {
	return building_attached;
}
Policy::Policy(int _idx, int _cost, Building* _b) {
	idx = _idx;
	cost = _cost;
	building_attached = _b;
}
void Policy::takeEffect() { }
void Policy::printPolicy() { }
bool Policy::available() { return true; }

RequireMasks::RequireMasks(int _idx, int _cost, Building* _b) : Policy(_idx, _cost, _b) { }
void RequireMasks::takeEffect() {
	BuildingAttached()->updateChange(-0.00001); // 取佩戴口罩带来的效应为
	BuildingAttached()->updateActionPoint(1); // 每天产生的行动力增加1
}
bool RequireMasks::available() {
	// 只要当前是一级的建筑，就可以采用这个政策
	if (BuildingAttached()->getLevel() >= 1)
		return true;
	return false;
}
void RequireMasks::printPolicy() {
	printf("当前是要求佩戴口罩的政策！\n");
}

MeasureTemperatureAndScanCode::MeasureTemperatureAndScanCode(int _idx, int _cost, Building* _b) : Policy(_idx, _cost, _b) { }
void MeasureTemperatureAndScanCode::takeEffect() {
	BuildingAttached()->updateChange(-0.00002); // 取测温扫码的效应为-10%
	BuildingAttached()->updateActionPoint(2); // 每天产生的行动力增加2
}
bool MeasureTemperatureAndScanCode::available() {
	// 当前是二级建筑，就可以采用这个政策
	if (BuildingAttached()->getLevel() >= 2)
		return true;
	return false;
}
void MeasureTemperatureAndScanCode::printPolicy() {
	printf("当前是测温扫码的政策！\n");
}

ShutDownBuilding::ShutDownBuilding(int _idx, int _cost, Building* _b) : Policy(_idx, _cost, _b) { }
void ShutDownBuilding::takeEffect() {
	BuildingAttached()->updateChange(-0.00003); // 取关闭建筑的效应为-100%
	BuildingAttached()->updateActionPoint(3); // 每天产生的行动力增加3
	BuildingAttached()->shutDown(); // 同时关闭当前的建筑
}
bool ShutDownBuilding::available() {
	// 这是一个三级的建筑政策
	if (BuildingAttached()->getLevel() >= 3)
		return true;
	return false;
}
void ShutDownBuilding::printPolicy() {
	printf("当前是关闭建筑的政策 \n");
}

AddBaffle::AddBaffle(int _idx, int _cost, Building* _b) : Policy(_idx, _cost, _b) { }
void AddBaffle::takeEffect() {
	BuildingAttached()->updateChange(-0.00001); // 增设隔板带来的效应为-5%
	BuildingAttached()->updateActionPoint(1); // 增加的行动点数为1
	BuildingAttached()->addBaffle(); // 增设挡板
}
bool AddBaffle::available() {
	// 这是一个一级政策
	if (BuildingAttached()->getLevel() >= 1)
		return true;
	return false;
}
void AddBaffle::printPolicy() {
	printf("当前是增设挡板的政策 \n");
}

StopDineIn::StopDineIn(int _idx, int _kind, Building* _b) : Policy(_idx, _kind, _b) { }
void StopDineIn::takeEffect() {
	BuildingAttached()->updateChange(-0.00003); // 取停止堂食的效应为-100%
	BuildingAttached()->updateActionPoint(3); // 每天产生的行动力增加3
	BuildingAttached()->shutDown();
}
bool StopDineIn::available() {
	if (BuildingAttached()->getLevel() >= 3)
		return true;
	return false;
}
void StopDineIn::printPolicy() {
	printf("当前是停止堂食的政策 \n");
}

TemporaryLockdown::TemporaryLockdown(int _idx, int _kind, Building* _b) : Policy(_idx, _kind, _b) { }
void TemporaryLockdown::takeEffect() {
	BuildingAttached()->updateChange(-0.00002); // 临时封控的效应为-20%
	BuildingAttached()->updateActionPoint(2); // 每天产生的行动力增量为2
}
bool TemporaryLockdown::available() {
	if (BuildingAttached()->getLevel() >= 2)
		return true;
	return false;
}
void TemporaryLockdown::printPolicy() {
	printf("当前是临时封控的政策 \n");
}

ShutDownDormitory::ShutDownDormitory(int _idx, int _kind, Building* _b) : Policy(_idx, _kind, _b) { }
void ShutDownDormitory::takeEffect() {
	BuildingAttached()->updateChange(0.00002); // 封锁宿舍楼带来的效应为+40%
	BuildingAttached()->updateActionPoint(-2); // 每天产生的行动力-2
	BuildingAttached()->shutDown();
}
bool ShutDownDormitory::available() {
	if (BuildingAttached()->getLevel() >= 3)
		return true;
	return false;
}
void ShutDownDormitory::printPolicy() {
	printf("当前是封闭寝室的政策 \n");
}




Affair::Affair(int _idx, double _p, Building* _b) {
	idx = _idx;
	probability_of_occurrence = _p;
	happened_place = _b;
}
double Affair::getProb() {
	return probability_of_occurrence;
}
Building* Affair::getPlace() {
	return happened_place;
}
void Affair::takeEffect() { }
void Affair::printAffair() { }
void Affair::happen() { }

CoupleByLake::CoupleByLake(int _idx, double _p, Building* _b) : Affair(_idx, _p, _b) { }
void CoupleByLake::happen() {
	uniform_real_distribution<> rdt(0, 1);
	double rand_p = rdt(engine);
	if (rand_p < getProb())
		takeEffect();
}
void CoupleByLake::takeEffect() {
	// 未名湖边的小情侣事件：满意度+2 
	// 传播速度+10%
	getPlace()->updateChange(0.00001);
	// ^ 这里要写满意度+2！
	// 当前建筑里的所有人的满意度+2
	getPlace()->updateSatisfaction(2);
	printAffair();
	system("pause");
}
void CoupleByLake::printAffair() {
	printf("当前是未名湖边的小情侣事件\n");
}

ReadByLake::ReadByLake(int _idx, double _p, Building* _b) : Affair(_idx, _p, _b) { }
void ReadByLake::happen() {
	uniform_real_distribution<> rdt(0, 1);
	double rand_p = rdt(engine);
	if (rand_p < getProb())
		takeEffect();
}
void ReadByLake::takeEffect() {
	// 未名湖畔好读书事件：传播速度+10%,满意度+2
	getPlace()->updateChange(0.00001);
	// ^这里要写满意度+2
	getPlace()->updateSatisfaction(2);
	printAffair();
	system("pause");
}
void ReadByLake::printAffair() {
	printf("当前是未名湖畔好读书事件\n");
}