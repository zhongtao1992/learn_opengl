#include "exercises\exercises.h"
#include "tutorials\tutorials.h"

int tutorial(){
	// shader interpolation
	return tutorial5();

	// shader uniform
	return tutorial4();

	// draw rectangle
	return tutorial3();

	// draw triangle
	return tutorial2();

	// hello window
	return tutorial1();
}

int exercise(){
	// output the vertex shader value to the fragment shader
	return exercise5();
	// move triangle by using uniform value
	return exercise4();
	// triangle is upside down
	return exercise3();
	// draw two triangle with two VAOs VBOs and two shader program
	return exercise2();
	// draw two triangle
	return exercise1();
}

int main(){
	
	//return tutorial();

	return exercise();
}