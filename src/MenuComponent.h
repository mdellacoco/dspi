/*
 * MenuComponent.h
 *
 *  Created on: May 31, 2016
 *      Author: Margarita E. Della Sera
 *      Usage of Composite Design Pattern for menu implemantation.
 */

#ifndef MENUCOMPONENT_H_
#define MENUCOMPONENT_H_

#include <string>
#include <vector>
#include "spi.h"


using namespace std;

class MenuComponent {
public:
	MenuComponent(string name, int hiearchy);
	virtual void Add(MenuComponent *d) = 0;
	virtual void Remove(MenuComponent *d) = 0;
	virtual void Display() = 0;
	virtual ~MenuComponent();

protected:
	string name_;
	int hiearchy_;

private:
	MenuComponent(){}; //not allowed
};

//} /* namespace std */

#endif /* MENUCOMPONENT_H_ */
