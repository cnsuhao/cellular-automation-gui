/************************************************
 *  
 *  cellular-automaton:CellMatrix.cpp
 *
 *  Created on: Oct 13, 2014
 *  Author: Andy Huang
 *	Email: andyspider@126.com
 *
 *	All rights reserved!
 *	
 ************************************************/

#include "CellMatrix.h"
#include "Cell.h"

CellMatrix::CellMatrix(int w, int h) :
		width(w), height(h)
{
	int i;
	for (i = 0; i < width * height; i++)
	{
		Cell *cell = new Cell(i, this);
		cells.push_back(cell);
	}
}

CellMatrix::~CellMatrix()
{
	int i;
	for (i = 0; i < width * height; i++)
	{
		delete cells[i];
	}
}

void CellMatrix::step()
{
	std::vector<Cell*>::iterator cit;
	for (cit = cells.begin(); cit != cells.end(); ++cit)
	{
		(*cit)->step();
	}

	// update colors
	for (cit = cells.begin(); cit != cells.end(); ++cit)
	{
		(*cit)->updateColor();
	}
}

void CellMatrix::printCells()
{
	int i;
	for (i = 0; i < width * height; i++)
	{
		if (i % width == 0)
			printf("\n");
		printf("%d ", cells[i]->getColor());
	}
	printf("\n");
}

void CellMatrix::setCellsExploreMode(bool val)
{
	std::vector<Cell*>::iterator cit;
	if (val)	// set to explore mode
	{
		for (cit = cells.begin(); cit != cells.end(); ++cit)
		{
			(*cit)->setLearningMode(Agent::EXPLORE);
		}
	}
	else	// set to online mode
	{
		for (cit = cells.begin(); cit != cells.end(); ++cit)
		{
			(*cit)->setLearningMode(Agent::ONLINE);
		}
	}
}

std::vector<int> CellMatrix::getSurroundingColors(int id)
{
	std::vector<int> colors;
	std::vector<int> neighs = getNeighbours(id);
	std::vector<int>::iterator nit;
	for (nit = neighs.begin(); nit != neighs.end(); ++nit)
	{
		if (*nit == -1)
			colors.push_back(0);		// black if no cell
		else
			colors.push_back(cells[*nit]->getColor());
	}

	return colors;
}

int CellMatrix::getCellColor(int id)
{
    return cells[id]->getColor();
}

std::vector<int> CellMatrix::getNeighbours(int id)
{
	std::vector<int> neighs;
	int nid, upCenter, downCenter;

	// up part
	upCenter = id - width;
	if (idExist(upCenter))
	{
		nid = upCenter - 1;
		if (idExist(nid) && sameLine(nid, upCenter))
			neighs.push_back(nid);
		else
			neighs.push_back(-1);
		neighs.push_back(upCenter);
		nid = upCenter + 1;
		if (idExist(nid) && sameLine(nid, upCenter))
			neighs.push_back(nid);
		else
			neighs.push_back(-1);
	}
	else
	{
		neighs.push_back(-1);
		neighs.push_back(-1);
		neighs.push_back(-1);
	}

	// middle
	nid = id - 1;
	if (idExist(nid) && sameLine(nid, id))
		neighs.push_back(nid);
	else
		neighs.push_back(-1);

	nid = id + 1;
	if (idExist(nid) && sameLine(nid, id))
		neighs.push_back(nid);
	else
		neighs.push_back(-1);

	// down
	downCenter = id + width;
	if (idExist(downCenter))
	{
		nid = downCenter - 1;
		if (idExist(nid) && sameLine(nid, downCenter))
			neighs.push_back(nid);
		else
			neighs.push_back(-1);
		neighs.push_back(downCenter);
		nid = downCenter + 1;
		if (idExist(nid) && sameLine(nid, downCenter))
			neighs.push_back(nid);
		else
			neighs.push_back(-1);
	}
	else
	{
		neighs.push_back(-1);
		neighs.push_back(-1);
		neighs.push_back(-1);
	}

//	// test
//	printf("------------------- id: %d neighs\n", id);
//	std::vector<int>::iterator nit;
//	for (nit = neighs.begin(); nit != neighs.end(); ++nit)
//		printf("%d ", *nit);
//	printf("\n");

	return neighs;
}

bool CellMatrix::idExist(int id)
{
	return (id >= 0 && id < width * height);
}

bool CellMatrix::sameLine(int id1, int id2)
{
	return (id1 / width == id2 / width);
}
