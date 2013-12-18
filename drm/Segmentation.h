#pragma once
#include "List1.h"
#include "Stack1.h"
#include <math.h>

#include "CPoint.h"
#include <stdlib.h>

#include "pf/pfimconv.h"
#include "pf/pfpnmfile.h"

using namespace std;

static const CPoint shift[5]={CPoint(0,0),CPoint(-1,0),CPoint(1,0),CPoint(0,-1),CPoint(0,1)};

class Segmentation
{
public:
	Segmentation(void);
	Segmentation(image<rgb> * im); //Initializes a contour, links with an image
public:
	~Segmentation(void);

public:
	Stack1<CPoint> active;   // stored unextented points, used for finding independent components
	
	image<rgb> * m_im;
	float w;
	float w1;   
	 
	List<List<CPoint>*> blocks;   
	List<int> labels;
	List<List<int>*> neibor;    
	List<List<int>*> allneibor;  	 
	 
	double **m_diff;               
	List<int> **diff;	 
 
	CPoint p,q,s;
	CPoint m_point;
	//CRect m_rect;
	int **count;  
	CPoint p1,p2;
	 	 
	int **mask;  
	int row,col;	 
	float *max_inter;   // inner object average intensity 	 
	 
	int h[256];
	int num_node;
 	 
	float *reg_ave_r;
	float *reg_ave_b;
	float *reg_ave_g;
	int m_w,m_h;
  	
	image<rgb> * meanshift_image;
	image<rgb> * normalized_image;
	image<rgb> * m_org;
	std::string imagename;
 
	int ** label_image;
 	
	std::string m_segimage;
 
	int *blockmask;
	 
	List<List<int>*> iter_blocks;    
	List<List<int>*> iter_neibors_consis;

	float pmiss;
	float pfa;
	float delta1;
	float delta2;
	image<rgb> *im_result;
public:
    void LoadImages( image<rgb> *meanshift_image_i, image<rgb> *m_org_i);
public:
	void BuildBlocks(void);
public:
	List<image<rgb>*> RegionMerging(int &components);
public:
	float CueEnhancement(int block, int neighbor, int sign);
public:
	float Combo(int size1, int size2, int m);
public:
	int RandomNum(int size);
public:
	void DeletePreheaders(void);
};
