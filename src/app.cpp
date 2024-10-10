#include <iostream>
#include "agent.hpp"
#include "mmanager.hpp"
#include "stochastic_decorator.hpp"
#include "simple_decorator.hpp"
#include <vector>
#include <string>
#include "tinyxml2.h"
#include "special_agent.hpp"

class InitManager
{
public:
	//기본 생성자
	InitManager(){}

	//경로를 인자로 받는 생성자
	InitManager(std::string path){

		tinyxml2::XMLDocument doc;

		doc.LoadFile(path.c_str());

		//<scenario> 태그의 <AgentList> 하위 엘리먼트를 가져오기
		tinyxml2::XMLElement* agListElem = doc.FirstChildElement( "scenario" )->FirstChildElement( "AgentList" );
		
		double x, y, heading, speed, drange;

		for(tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
			agElem != NULL; agElem = agElem->NextSiblingElement())
		{

			// 각 에이전트의 x, y 좌표, heading, speed, drange 속성 읽어오기
			agElem->QueryDoubleAttribute("x", &x);
			agElem->QueryDoubleAttribute("y", &y);
			agElem->QueryDoubleAttribute("heading", &heading);
			agElem->QueryDoubleAttribute("speed", &speed);
			agElem->QueryDoubleAttribute("drange", &drange);

			CAgent* ag = new CSpecialAgent(x, y, heading, speed, drange);
			m_agent_list.push_back(ag);
		}
	}

private:
	// 에이전트 객체들을 저장하는 벡터
	std::vector<CAgent*> m_agent_list;

public:
	// 에이전트 리스트 반환
	std::vector<CAgent*>& get_agent_list() { return m_agent_list; }
	
};

int main(int argc, char** argv)
{// Clinet Code  

    /* Simulation Engine */
    CManeuverManager mmanager;
	InitManager init_manager("test.xml");
    /* Simulation Model Registration */
	

	/*
		클라이언트 코드에서 클래스 타입을 몰라도 동작하는 이유
		다형성 - 동일한 인터페이스를 사용하여 서로 다른 클래스의 객체들을 처리
		CAgent* 포인터를 통해 CAgent를 상속 받은 자식 클래스를 다룰 수 있음
	*/
	for(std::vector<CAgent*>::iterator it = init_manager.get_agent_list().begin();
		it != init_manager.get_agent_list().end(); ++it)
		{
			mmanager.register_publisher(*it);
		}

	//	CAgent* p_ag = ???.create_agent();
	//	m_agnet_list.push_back(p_ag);
    // CAgent* p_agent1 = new CAgent(10,  0, 0, 1, 10);
    // p_agent1 = new StochasticDecorator(p_agent1);
    // p_agent1 = new SimpleDecorator(p_agent1);
    
    // mmanager.register_publisher(p_agent1);

    // CAgent* p_agent2 = new CAgent(10,  30, 180, 1, 10);
    // mmanager.register_publisher(p_agent2);
    
    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;

    for(sim_time = 0; sim_time < 30; sim_time += time_step)
    {   
        //p_agent1->maneuver(time_step);
        //p_agent2->maneuver(time_step);

        //p_agent1->detect(p_agent2);
        //p_agent2->detect(p_agent1);

        mmanager.svc(time_step);

        // std::cout << "----" << std::endl;
        // std::cout << "Time: " << sim_time << ", " <<*p_agent1 << std::endl;
        // std::cout << "Time: " << sim_time << ", " <<*p_agent2 << std::endl;

		std::cout << "-----" << std::endl;
		for(std::vector<CAgent*>::iterator it = init_manager.get_agent_list().begin();
			it != init_manager.get_agent_list().end(); ++it)
			{
				std::cout << "Time: " << sim_time << ", " << *(*it) << std::endl;
			}

    }
    return 0;
}
