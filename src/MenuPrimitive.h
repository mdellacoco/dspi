/*
 * MenuPrimitive.h
 *
 *  Created on: May 31, 2016
 *      Author: Margarita E. Della Sera
 *      The Leaf Class implemntation of the Menu Composite Design Pattern
 */

#ifndef MENUPRIMITIVE_H_
#define MENUPRIMITIVE_H_

#include <string>
#include "MenuComponent.h"
#include "spi.h"


class MenuPrimitive: public MenuComponent {
public:
	MenuPrimitive(string name, int hierarchy, int methodId,
            string description,
            spi* spiDev);
	void Add(MenuComponent *d){}
	void Remove(MenuComponent *d){}
	void Display();
	virtual ~MenuPrimitive();
private:
	MenuPrimitive(); // not allowed
	void Run();
	void Calibrate();

	enum{v_dc_offset_cal = 1};
	int    methodId_;    //its the calibration type
	string description_;
	spi*   spiDev_;

};

#endif /* MENUPRIMITIVE_H_ */
