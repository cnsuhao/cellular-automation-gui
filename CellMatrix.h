/************************************************
 *  
 *  cellular-automaton:CellMatrix.h
 *
 *  Created on: Oct 13, 2014
 *  Author: Andy Huang
 *	Email: andyspider@126.com
 *
 *	All rights reserved!
 *	
 ************************************************/

#ifndef CELLMATRIX_H_
#define CELLMATRIX_H_
#include <vector>

class Cell;

/*
 *
 */
class CellMatrix
{
	public:
		CellMatrix(int w, int h);
		~CellMatrix();

		void step();
        void step(int id);
		std::vector<int> getSurroundingColors(int id);
        int getCellColor(int id);
        void setCellColor(int id, int c);
		void setCellsExploreMode(bool val);
		void printCells();
        void updateColors();

	private:
		int width;
		int height;
		std::vector<Cell *> cells;

		std::vector<int> getNeighbours(int id);
		bool idExist(int id);
		bool sameLine(int id1, int id2);
};

#endif /* CELLMATRIX_H_ */
