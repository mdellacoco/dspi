/*
 * MenuComposition.cpp
 *
 *  Created on: May 31, 2016
 *      Author: victoria
 */

#include "MenuComposition.h"

MenuComposition::MenuComposition(string name, int hierarchy) : MenuComponent(name, hierarchy) {
	// TODO Auto-generated constructor stub

}

MenuComposition::~MenuComposition() {
	// TODO Auto-generated destructor stub
}

void MenuComposition::Add(MenuComponent *d) {
	elements_.push_back(d);
}

void MenuComposition::Remove(MenuComponent *d){

	vector<MenuComponent*>::iterator it = elements_.begin();

	while(it != elements_.end()) {

		if(*it == d) {
			delete d;
			elements_.erase(it);
			break;
		}
		++it;
	}
}

void MenuComposition::Display(){

	vector<MenuComponent*>::iterator it = elements_.begin();
//here I want to display the calibration about to be performed.
    cout << name_ << endl;
	while(it != elements_.end()) {

		(*it)->Display();
		++it;
		}
}

