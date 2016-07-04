/*
 * MenuComposition.h
 *
 *  Created on: May 31, 2016
 *      Author: Margarita E. Della Sera
 *
 *      Composite Class to implement the Composite Design Pattern
 */

#ifndef MENUCOMPOSITION_H_
#define MENUCOMPOSITION_H_

#include <vector>
#include "MenuComponent.h"
#include "spi.h"
using namespace std;

class MenuComposition: public MenuComponent {
public:
	MenuComposition(string name, int hierarchy);
	void Add(MenuComponent *d);
	void Remove(MenuComponent *d);
	void Display();
	virtual ~MenuComposition();
private:
	MenuComposition(); //not allowed
	vector<MenuComponent*> elements_;
};

#endif /* MENUCOMPOSITION_H_ */
