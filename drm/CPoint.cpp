#include "CPoint.h"
#include <iostream>
#include <math.h>

using namespace std;

unsigned int CPoint :: debug = 0;
unsigned int CPoint :: total = 0;
unsigned int CPoint :: current = 0;

CPoint::CPoint (double _x, double _y) : x (_x), y (_y) {
    if (debug) std::cout << "Point (" << x << ";" << y << ") created."<<endl;
	id = ++total;
	++current;
	
}

CPoint::CPoint () : x(0), y(0) {
	if (debug) cout << "Point (" << x << ";" << y << ") created."<<endl;
	id = ++total;
	++current;
	
}

CPoint::CPoint (double c) : x(c), y(c) {
	if (debug) cout << "Point (" << x << ";" << y << ") created."<<endl;
	id = ++total;
	++current;
	
}

CPoint::~CPoint () {
    if (debug) cout << "Point (" << x << ";" << y << ") deleted"<<endl;
	--current;
}

int CPoint :: inv () {
    return ( ((x>=-1000) && (x<=1000)) && ((y>=-1000) && (y<=1000)) );
}


void CPoint :: SetX(double _x) {
	x = _x;
}

void CPoint :: SetY(double _y) {
	y = _y;
}

void CPoint :: Move (double dx, double dy) {
	x += dx;
	y += dy;
}

double CPoint :: GetX() const{
	return x;
}

double CPoint :: GetY() const{
	return y;
}

double CPoint::d(CPoint P) {      // Euclidean distance
	double dx = P.GetX() - x;
	double dy = P.GetY() - y;
    
	return sqrt(dx*dx + dy*dy);
}

double CPoint::d(CPoint P1, CPoint P2) {      // Euclidean distance
	double dx = P1.GetX() - P2.GetX();
	double dy = P1.GetY() - P2.GetY();
    
	return sqrt(dx*dx + dy*dy);
}

double CPoint::d2(CPoint P) {     // squared distance (more efficient)
	double dx = P.GetX() - x;
	double dy = P.GetY() - y;
    
	return (dx*dx + dy*dy);
}

int CPoint :: nCheck(CPoint P1, CPoint P2) {
    // = 0 is on Line P1, P2
    // < 0 is rights from Line P1, P2
    // > 0 is lefts from Line P1, P2
    return (P2.x-P1.x)*(y-P1.y)-(P2.y-P1.y)*(x-P1.x);
}


void CPoint :: SetDebug (unsigned int d) {
    debug = d;
}

unsigned int CPoint ::  GetID () {
    return id;
}

unsigned int CPoint :: GetCurrent () {
    return current;
}

unsigned int CPoint :: GetTotal () {
    return total;
}

/*CPoint CPoint::operator=(CPoint& p) {
 
 x = p.x;
 y = p.y;
 
 return *this;
 }  */


void CPoint :: Print () {
	cout << "(" << x << ";" << y << ")";
}


CPoint CPoint::operator+ (const CPoint &p) {
	return CPoint (x + p.x, y + p. y);
}

CPoint CPoint::operator - (const CPoint &p){
	return CPoint (x - p.x, y - p. y);
}

int CPoint::operator == (CPoint p) {
	return (x == p.x) && (y == p.y);
}

int CPoint::operator != (double c) {
	return !(this->x == c && this->y == c);
}

int CPoint::operator != (CPoint &p) {
	return !(*this == p);
}

int CPoint::operator< (CPoint &p) {
	return ((x < p.x) || ((x == p.x) && (y<p.y)));
}

int CPoint::operator> (CPoint &p) {
	return ((x > p.x) || ((x == p.x) && (y > p.y)));
}

int CPoint :: operator== (double c) {
	return (x == c) && (y == c);
}

int CPoint :: operator> (double c) {
	return (x > c) && (y > c);
}

int CPoint :: operator< (double c) {
	return (x < c) && (y < c);
}

int CPoint :: operator<= (double c) {
	return ((x < c) || (x == c)) && ((y < c) || (y == c));
}

int CPoint :: operator>= (double c) {
	return ((x > c) || (x == c)) && ((y > c) || (y == c));
}

int CPoint :: operator<= (CPoint& p) {
	return ((x < p.x) || (x == p.x)) && ((y < p.x) || (y == p.x));
}

int CPoint :: operator>= (CPoint& p) {
	return ((x > p.x) || (x == p.x)) && ((y > p.x) || (y == p.x));
}

// Write output Point in format: "(%f, %f)"
ostream& operator<<( ostream& output,const CPoint& P) {
	
	output << "(" << P.x << ";" << P.y << ")";
    
	return output;
}

// Read input Point format: "(%f)", "(%f, %f)", or "(%f, %f, %f)"
istream& operator>>( istream& input, CPoint& P) {
	char c;
	input >> c;                // skip '('
	input >> P.x;
	input >> c;
    // skip ','
	input >> P.y;
	input >> c;
	
    return input;
	
}

