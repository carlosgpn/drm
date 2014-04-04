#pragma once

#include <iostream>


using namespace std;

class CPoint {
private:
    
    static unsigned int debug;
    static unsigned int total;
    unsigned int id;
    static unsigned int current;
public:
    CPoint(double,double);
    CPoint();
    CPoint(double);
    ~CPoint();
    int inv();
    CPoint operator + (const CPoint&);
    CPoint operator - (const CPoint&);
    
    int x;
    int y;
    
    void SetX(double);
    void SetY(double);
    void Move (double, double);
    double GetX() const;
    double GetY() const;
    
    
    
    void Print();
    
    // Point Relations
    double d( CPoint);         // Distance
    double d(CPoint, CPoint);
    double d2( CPoint);        // Distance^2
    int nCheck( CPoint, CPoint);
    
    
    void SetDebug (unsigned int);
    unsigned int GetID ();
    unsigned int GetCurrent();
    unsigned int GetTotal();
    
    // îäèíàêîâû ëè òî÷êè ?
    
    // CPoint operator = (CPoint&);
    int operator == (CPoint);
    int operator!= (CPoint&);
    int operator== (double);
    int operator!= (double);
    int operator< (CPoint&);
    int operator> (CPoint&);
    int operator> (double);
    int operator< (double);
    int operator<= (double);
    int operator>= (double);
    int operator>= (CPoint&);
    int operator<= (CPoint&);
    friend std::ostream& operator<<( std::ostream& output,const CPoint& P);
    friend std::istream& operator>>( std::istream&, CPoint&);
    
};
