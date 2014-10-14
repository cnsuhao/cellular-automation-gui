/************************************************
 *  
 *  cellular-automaton:Cell.h
 *
 *  Created on: Oct 13, 2014
 *  Author: Andy Huang
 *	Email: andyspider@126.com
 *
 *	All rights reserved!
 *	
 ************************************************/

#ifndef CELL_H_
#define CELL_H_
#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>

using namespace gamcs;

class CellMatrix;

/*
 *
 */
class Cell: public Avatar
{
	public:
		Cell(int id, CellMatrix *matrix);
		~Cell();

		int getColor() const;
		void updateColor();
		void setLearningMode(Agent::Mode mode);

	private:
		Agent::State perceiveState();
		void performAction(Agent::Action);
		OSpace availableActions(Agent::State);
		float originalPayoff(Agent::State);

		int id;
		int color;
		int rotate_color;
		CSOSAgent *myagent;
		CellMatrix *matrix;
		int numBlackNeigh;
		int numWhiteNeigh;
};

#endif /* CELL_H_ */
