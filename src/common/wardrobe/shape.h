/*
 * shape.h
 *
 *  Created on: Dec 31, 2013
 *      Author: victoria
 */

#ifndef SHAPE_H_
#define SHAPE_H_
class shape {
public:
	virtual ~shape(){}
	virtual void draw() = 0;
};

typedef shape* create_t();
typedef void destroy_t(shape*);


#endif /* SHAPE_H_ */
