/*
 * MenuPrimitive.cpp
 *
 *  Created on: May 31, 2016
 *      Author: victoria
 */

#include "MenuPrimitive.h"



MenuPrimitive::MenuPrimitive(string name, int hierarchy, int methodId,
		                     string description,
		                     spi* spiDev)
                         :MenuComponent(name, hierarchy),
                          methodId_(methodId),
                          description_(description),
                          spiDev_(spiDev){
	// TODO Auto-generated constructor stub

}

MenuPrimitive::~MenuPrimitive() {
	// TODO Auto-generated destructor stub
}

void MenuPrimitive::Display(){

	printf("%s %i\n", name_.c_str(), hiearchy_);

	if(methodId_ == 0)
	   Run();
	else
		Calibrate();

}

void MenuPrimitive::Calibrate(){
   cout << description_.c_str() << endl;

   spiDev_->Calibrate(methodId_);
}

void MenuPrimitive::Run(){
	 cout << description_.c_str() << endl;
	 spiDev_->Run();

}

