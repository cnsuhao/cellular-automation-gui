/************************************************
 *  
 *  cellular-automaton:Cell.cpp
 *
 *  Created on: Oct 13, 2014
 *  Author: Andy Huang
 *	Email: andyspider@126.com
 *
 *	All rights reserved!
 *	
 ************************************************/

#include <vector>
#include <math.h>
#include <QtGlobal>
#include "Cell.h"
#include "CellMatrix.h"

Cell::Cell(int i, CellMatrix *mat) :
        id(i), color(qrand() % 2), rotate_color(0), myagent(NULL), matrix(mat), numBlackNeigh(
				0), numWhiteNeigh(0)
{
	myagent = new CSOSAgent(id, 0.9, 0.01);
	connectAgent(myagent);
}

Cell::~Cell()
{
	delete myagent;
}

Agent::State Cell::perceiveState()
{
	// get the cells color around
	numBlackNeigh = numWhiteNeigh = 0;
	std::vector<int> colors = matrix->getSurroundingColors(id);
	int st = 0;
	std::vector<int>::iterator cit;
	int i;
	for (cit = colors.begin(), i = 0; cit != colors.end(); ++cit, ++i)
	{
		st += (*cit) * pow(2, i);
		if ((*cit) == 0)
			numBlackNeigh++;
		else
			numWhiteNeigh++;
	}

	return st;
}

void Cell::performAction(Agent::Action act)
{
	// turn black or white
	if (act == 0)
		rotate_color = 0;
	else
		rotate_color = 1;
}

OSpace Cell::availableActions(Agent::State st)
{
	// black or white
	OSpace acts;
	acts.add(0);
	acts.add(1);

	return acts;
}

float Cell::originalPayoff(Agent::State st)
{
	return numWhiteNeigh - numBlackNeigh;		// prefer white
}

int Cell::getColor() const
{
	return color;
}

void Cell::updateColor()
{
	color = rotate_color;
}

void Cell::setLearningMode(Agent::Mode mode)
{
	myagent->setMode(mode);
}
