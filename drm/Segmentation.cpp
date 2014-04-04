//
//  Segmentation.cpp
//  drm
//
//  Created by Carlos Neto on 10/10/13.
//  Copyright (c) 2013 Carlos Neto. All rights reserved.
//

#include "Segmentation.h"

#define DEBUG true
#define BORDER false

// random color
rgb _random_rgb(){
    rgb c;
    
    c.r = (uchar)random();
    c.g = (uchar)random();
    c.b = (uchar)random();
    
    return c;
}

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

void convertimage(std::string file1 , std::string file2) {
    
    char buff[1000];
    sprintf(buff, "/opt/local/bin/convert '%s' '%s'", file1.c_str(),file2.c_str());
    
    printf(exec(buff).c_str());
    
    
}

Segmentation::Segmentation(void)
{
	m_org = NULL;
	w = 1;
	max_inter = NULL;
	delta1 = 1;
	delta2 = 0.2;
}

Segmentation::Segmentation(image<rgb> * im)
{
	m_org = NULL;
	m_im = im;
	w = 1;
	max_inter = NULL;
    
	col = im->height();
	row = im->width();
	delta1 = 1;
	delta2 = 0.2;
    
	im_result = new image<rgb>(im->width(),im->height());
    
}

Segmentation::~Segmentation(void)
{
    
    
}

void Segmentation::LoadImages( image<rgb> *meanshift_image_i, image<rgb> *m_org_i)
{
    
	meanshift_image = meanshift_image_i;
    
	if (!meanshift_image) {
        printf("Can't open image meanshift");
	}
    
	// load the original image
    m_org = m_org_i;
    
	if (!m_org) {
		printf("Can't open image original");
    }
    
}

void Segmentation::BuildBlocks(void)
{
    //	if(m_org->m_color == GRAY)
    //        printf("gray image!");
    //	else
    //        printf("color image!");
    
	Stack1<CPoint> active;
	
	bool n = false;
	
    CPoint point;
	m_w = meanshift_image->width();
	m_h = meanshift_image->height();
    
	/*m_w = normalized_image->width();
     m_h = normalized_image->height();*/
    
	mask = (int **) new int *[m_h];
	label_image = (int **) new int *[m_h];
	for(int i=0;i<m_h;i++)
	{
        
		mask[i] = (int *) new int[m_w];
		label_image[i] = (int *) new int[m_w];
	}
    
	for(int i=0;i<m_h;i++)
		for(int j=0;j<m_w;j++)
		{
            
			mask[i][j] = 0;
			label_image[i][j] = 0;
		}
    
    /********************************build blocks************************************\
     \********************************************************************************/
    
	int size;
    size = m_org->height()*m_org->width();
	CPoint seed;
	
	while(size>0)
	{
		seed.x = -1; seed.y = -1;
		for(p.y=0;p.y<m_org->height();p.y++)  //’“µΩ“ª∏ˆŒ¥¿©≥‰µƒ÷÷◊”µ„
		{
			
			for(p.x=0;p.x<m_org->width();p.x++)
				if(mask[p.y][p.x] == 0)
				{
					seed.x = p.x; seed.y = p.y;
					blocks.append(new List<CPoint>);
					blocks.retrieveLast()
                    ->append(seed);
					label_image[seed.y][seed.x] = blocks.getLength();
					break;
				}
			if(seed.x!= -1) break;
		}
        
		active.push(seed);
		mask[seed.y][seed.x] = 1; size--;
		while(!active.isEmpty()) //’“µΩ÷÷◊”µ„À˘‘⁄µƒcomponent
        {
            //   printf("%d\n", size);
            p = active.pop();
            for(int i=1;i<5;i++)
            {
                q = p + shift[i];
                {if(q.x >= 0 && q.x < m_org->width() && q.y>= 0 && q.y < m_org->height())
                    if(mask[q.y][q.x]==0 )
                        if(meanshift_image->access[seed.y][seed.x].r==meanshift_image->access[q.y][q.x].r)
                            if(meanshift_image->access[seed.y][seed.x].g==meanshift_image->access[q.y][q.x].g)
                                if(meanshift_image->access[seed.y][seed.x].b==meanshift_image->access[q.y][q.x].b)
                                {
                                    active.push(q);
                                    mask[q.y][q.x] = 1;
                                    size--;
                                    blocks.retrieveLast()->append(q);
                                    label_image[q.y][q.x] = blocks.getLength();
                                }
                }
            }
        }
		seed.x = -1; seed.y = -1;
	}
    
    /********************************finish build blocks************************************\
     \***************************************************************************************/
	for(int i=1;i<=blocks.getLength();i++)
	{
		labels.append(i);
	}
	
    for(int i=1;i<=labels.getLength();i++)
    {
        
        allneibor.append(new List<int>);
        allneibor.retrieveLast()->append(labels.retrieve(i));
    }
    
    for(int i=1;i<m_h-1;i++)
        for(int j=1;j<m_w-1;j++)
        {
            n = false;
            p.x = j; p.y = i;
            for(int k=1;k<=4;k++)
            {
                q.x = (p + shift[k]).x;
                q.y = (p + shift[k]).y;
                if(label_image[i][j] != label_image[q.y][q.x])
                { n = true;}
                
                if(n == true)
                {
                    if(label_image[i][j]>label_image[q.y][q.x]||
                       label_image[i][j]<label_image[q.y][q.x])
                    {
                        bool add = false;
                        for(int k=1;k<=allneibor.getLength();k++)
                            if(label_image[i][j] == allneibor.retrieve(k)->retrieve(1))
                            {
                                add = true;
                                for(int l=2;l<=allneibor.retrieve(k)->getLength();l++)
                                    if(label_image[q.y][q.x] == allneibor.retrieve(k)->retrieve(l))
                                        add = false;
                                if(add == true)
                                    allneibor.retrieve(k)->append(label_image[q.y][q.x]);
                                k = allneibor.getLength() + 1;  // jump out of the loop
                            }
                    }
                    
                }
            }
        }
    
	for(int i=0;i<m_h;i++)
		delete [] mask[i];
	delete []mask;
    
	blockmask =  new int[blocks.getLength()+1];
	for(int i=1;i<=blocks.getLength();i++)
		blockmask[i] = 0;
    
	reg_ave_r = new float[blocks.getLength()];
	reg_ave_g = new float[blocks.getLength()];
	reg_ave_b = new float[blocks.getLength()];
	for(int i=0;i<blocks.getLength();i++) {reg_ave_r[i] = 0; reg_ave_g[i] = 0; reg_ave_b[i] = 0;}
    
	float ave;
	float ave1, ave2;
	for(int i=1;i<=blocks.getLength();i++)
	{
		ave = 0;  ave1 = 0; ave2 = 0;
		for(int j=1;j<=blocks.retrieve(i)->getLength();j++)
		{
			p = blocks.retrieve(i)->retrieve(j);
            ave = ave + m_org->access[p.y][p.x].r;
            ave1 = ave1 + m_org->access[p.y][p.x].g;
            ave2 = ave2 + m_org->access[p.y][p.x].b;
		}
		ave = ave/blocks.retrieve(i)->getLength();
        
        ave1 = ave1/blocks.retrieve(i)->getLength();
        ave2 = ave2/blocks.retrieve(i)->getLength();
        reg_ave_r[i-1] = ave;
        reg_ave_g[i-1] = ave1;
        reg_ave_b[i-1] = ave2;
		
	}

	   
}

List<image<rgb>*> Segmentation::RegionMerging(int &components)
{
	int curr_blocklength;
	bool combine = false;
	float *ave_block_r,*ave_block_g,*ave_block_b;
	int num;
	int tmp;
    
	int which_neibor1;
	int which_neibor2;
	List<List<int>*> iter_neibors;
	List<List<int>*> iter_neibors_next;
	List<List<int>*> iter_neibors_now;
	float ***graph_weight;
	int block_a, block_b;
	bool go_on;
	int round = 0;
    
    List<image<rgb>*> frames;
    
    rgb *colors = new rgb[500];
    
    for (int i = 0; i < 500; i++)
        colors[i] = _random_rgb();
    
    
	// ********************* initialization********************************************
	
    components = blocks.getLength();
    
	for(int i=1;i<=blocks.getLength();i++)
	{
		iter_blocks.append(new List<int>);
		iter_blocks.retrieve(i)->append(i);
		iter_blocks.retrieve(i)->append(i);
		iter_neibors.append(new List<int>);
		iter_neibors_next.append(new List<int>);
		iter_neibors_now.append(new List<int>);
		iter_neibors_consis.append(new List<int>);
	}
    
	
	for(int i=1;i<=allneibor.getLength();i++)
		for(int j=2;j<=allneibor.retrieve(i)->getLength();j++)
		{
			bool exist = false;
			tmp = allneibor.retrieve(i)->retrieve(j);
			for(int k=2;k<=allneibor.retrieve(tmp)->getLength();k++)
				if(allneibor.retrieve(tmp)->retrieve(k) == i)
					exist = true;
            
			if(exist == false)
			{
				allneibor.retrieve(tmp)->append(i);
				
			}
		}
	
    for(int i=1;i<=allneibor.getLength();i++){
		for(int j=1;j<=allneibor.retrieve(i)->getLength();j++)
        {
            iter_neibors.retrieve(i)->append(allneibor.retrieve(i)->retrieve(j));
            iter_neibors_next.retrieve(i)->append(allneibor.retrieve(i)->retrieve(j));
            iter_neibors_now.retrieve(i)->append(allneibor.retrieve(i)->retrieve(j));
        }
    }
    ave_block_r = new float[allneibor.getLength()];
    ave_block_g = new float[allneibor.getLength()];
    ave_block_b = new float[allneibor.getLength()];
    
    for(int j=0;j<allneibor.getLength();j++)
    { ave_block_r[j] = reg_ave_r[j]; ave_block_g[j] = reg_ave_g[j]; ave_block_b[j] = reg_ave_b[j];}
    
	// ********************* initialization********************************************
	curr_blocklength = iter_blocks.getLength();
	while(curr_blocklength>2&&curr_blocklength != round)
	{
		
		combine =  false;
        
		round = curr_blocklength;
        
		
		for(int i=1;i<=allneibor.getLength();i++)
		{
			while(!iter_neibors.retrieve(i)->isEmpty())
				iter_neibors.retrieve(i)->removeLast();
            
			while(!iter_neibors_now.retrieve(i)->isEmpty())
				iter_neibors_now.retrieve(i)->removeLast();
            
			while(!iter_neibors_now.retrieve(i)->isEmpty())
				iter_neibors_consis.retrieve(i)->removeLast();
		}
        
		for(int i=1;i<=iter_neibors_next.getLength();i++)
			for(int j=1;j<=iter_neibors_next.retrieve(i)->getLength();j++)
			{
				iter_neibors.retrieve(i)->append(
                                                 iter_neibors_next.retrieve(i)->retrieve(j));
			}
        
		
		for(int i=1;i<=iter_neibors_next.getLength();i++)
            for(int j=1;j<=iter_neibors_next.retrieve(i)->getLength();j++)
            {
                iter_neibors_now.retrieve(i)->append(
                                                     iter_neibors_next.retrieve(i)->retrieve(j));
            }
		
		graph_weight = (float ***) new float **[iter_neibors_now.getLength()+1];
		for(int i=1;i<=iter_neibors_now.getLength();i++)
		{
			tmp = iter_neibors_now.retrieve(i)->getLength();
			if(tmp > 1)
			{
				graph_weight[i] = (float **) new float *[tmp+1];
				for(int j=2;j<=tmp;j++)
				{
					graph_weight[i][j] = new float[1];
					graph_weight[i][j][0] = -1;
				}
			}
		}
        
        
		float value,tp;
		for(int i=1;i<=iter_blocks.getLength();i++)
		{
			num = 0;
			which_neibor1 = 0; which_neibor2 = 0;
			combine = false;
			block_a = 0; block_b = 0;
			if(iter_blocks.retrieve(i)->getLength()>1)
			{
				
				block_a= iter_blocks.retrieve(i)->retrieve(1);
                
				go_on = false;
				
				if(iter_neibors.retrieve(block_a)->getLength() ==1)
					continue;
                
				if(iter_neibors.retrieve(block_a)->getLength()!=
                   iter_neibors_now.retrieve(block_a)->getLength())
					continue;
				
				num = iter_neibors_now.retrieve(block_a)->retrieve(2);
                
				if(graph_weight[block_a][2][0] == -1)
				{
					value = CueEnhancement(block_a,num,1);
					graph_weight[block_a][2][0] = value;
					for(int k=2;k<=iter_neibors_now.retrieve(num)->getLength();k++)
						if(block_a == iter_neibors_now.retrieve(num)->retrieve(k))
                        {tmp = k;  k = iter_neibors_now.retrieve(num)->getLength()+1;}
					graph_weight[num][tmp][0] = value;
				}
				else
					value = graph_weight[block_a][2][0];
                
                block_b =  num;
                
                
                for(int j=3;j<=iter_neibors_now.retrieve(block_a)->getLength();j++)
                {
					
					
					num = iter_neibors_now.retrieve(block_a)->retrieve(j);
					if(graph_weight[block_a][j][0] == -1)
					{
						tp = CueEnhancement(block_a,num,1);
						graph_weight[block_a][j][0] = tp;
						for(int k=2;k<=iter_neibors_now.retrieve(num)->getLength();k++)
							if(block_a == iter_neibors_now.retrieve(num)->retrieve(k))
                            {tmp = k;  k = iter_neibors_now.retrieve(num)->getLength()+1;}
						graph_weight[num][tmp][0] = tp;
					}
					else
						tp = graph_weight[block_a][j][0];
                    
					if(value>tp)
					{
						value = tp;
						block_b = num;
					}
				}
				
				go_on = false;
				for(int k=1;k<=iter_neibors_consis.retrieve(block_a)->getLength();k++)
					if(iter_neibors_consis.retrieve(block_a)->retrieve(k) == true)
						go_on = true;
                
                if(go_on == true)
                {
                    
                    if(iter_neibors.retrieve(block_b)->getLength() >1)
                    {
                        if(iter_neibors.retrieve(block_b)->getLength()==
                           iter_neibors_now.retrieve(block_b)->getLength())
						{
							num = iter_neibors_now.retrieve(block_b)->retrieve(2);
                            
							if(graph_weight[block_b][2][0] ==-1)
							{
								value = CueEnhancement(block_b,num,2);
								graph_weight[block_b][2][0] = value;
								for(int k=2;k<=iter_neibors_now.retrieve(num)->getLength();k++)
									if(block_b == iter_neibors_now.retrieve(num)->retrieve(k))
                                    {tmp = k;  k = iter_neibors_now.retrieve(num)->getLength()+1;}
								graph_weight[num][tmp][0] = value;
							}
							else
								value = graph_weight[block_a][2][0] ;
							
							which_neibor2 = num;
							
                            
							for(int j=3;j<=iter_neibors_now.retrieve(block_b)->getLength();j++)
							{
								num = iter_neibors_now.retrieve(block_b)->retrieve(j);
                                
								if(graph_weight[block_b][j][0] == -1)
								{
									tp = CueEnhancement(block_b,num,2);
									graph_weight[block_b][j][0] = tp;
									for(int k=2;k<=iter_neibors_now.retrieve(num)->getLength();k++)
										if(block_b == iter_neibors_now.retrieve(num)->retrieve(k))
                                        {tmp = k; k = iter_neibors_now.retrieve(num)->getLength()+1;}
									graph_weight[num][tmp][0] = tp;
								}
								else
									tp = graph_weight[block_b][j][0];
                                
								if(value>tp)
								{
									value = tp;
									which_neibor2 = num;
								}
							}
                            
                            combine = false;
                            
                            for(int j=2;j<= iter_neibors_now.retrieve(block_b)->getLength();j++)
                                if(graph_weight[block_b][j][0] == value )
                                    if(iter_neibors_now.retrieve(block_b)->retrieve(j)
                                       == block_a)
									{combine = true;curr_blocklength--;}
                        }
                    }
                    else
                        combine = false;
                    
                    
                    if(combine == true)
                    {
                        components--;
                        
                        // update iter_neibors
                        while(iter_neibors.retrieve(block_b)->getLength()>1)
                            iter_neibors.retrieve(block_b)->removeLast();
                        while(iter_neibors.retrieve(block_a)->getLength()>1)
                            iter_neibors.retrieve(block_a)->removeLast();
                        
                        for(int a=1;a<=iter_neibors.getLength();a++)
                            for(int k=2;k<=iter_neibors.retrieve(a)->getLength();k++)
                                if(iter_neibors.retrieve(a)->retrieve(k) == block_b)
                                    iter_neibors.retrieve(a)->remove(k);
                        
                        
                        for(int a=1;a<=iter_neibors.getLength();a++)
                            for(int k=2;k<=iter_neibors.retrieve(a)->getLength();k++)
                                if(iter_neibors.retrieve(a)->retrieve(k) == block_a)
                                    iter_neibors.retrieve(a)->remove(k);
                        
                        
                        bool there;
                        for(int j=2;j<=iter_neibors_next.retrieve(block_a)->getLength();j++)
                            if(iter_neibors_next.retrieve(block_a)->retrieve(j)==block_b)
                            {iter_neibors_next.retrieve(block_a)->remove(j);}
                        
                        
						for(int j=2;j<=iter_neibors_next.retrieve(block_b)->getLength();j++)
						{
							tmp = iter_neibors_next.retrieve(block_b)->retrieve(j);
							there = false;
							if(tmp != block_a)
							{
								for(int k=2;k<=iter_neibors_next.retrieve(block_a)->getLength();k++)
									if(tmp == iter_neibors_next.retrieve(block_a)->retrieve(k))
										there =  true;
								if(there == false)
									iter_neibors_next.retrieve(block_a)->append(tmp);
							}
						}
                        
                        
						for(int j=1;j<=iter_neibors_next.getLength();j++)
                        {
                            for(int k=2;k<=iter_neibors_next.retrieve(j)->getLength();k++)
								if(iter_neibors_next.retrieve(j)->retrieve(k) == block_b)
                                {
                                    iter_neibors_next.retrieve(j)->remove(k);
                                }
                        }
						for(int j=2;j<=iter_neibors_next.retrieve(block_b)->getLength();j++)
                        {
                            tmp = iter_neibors_next.retrieve(block_b)->retrieve(j);
                            there = false;
                            if(tmp != block_a)
                            {
                                for(int k=2;k<=iter_neibors_next.retrieve(tmp)->getLength();k++)
                                    if(iter_neibors_next.retrieve(tmp)->retrieve(k)==block_a)
                                        there =true;
                                if(there==false)
                                    iter_neibors_next.retrieve(tmp)->append(block_a);
                            }
                        }
						
                        
						while(iter_neibors_next.retrieve(block_b)->getLength()>1)
							iter_neibors_next.retrieve(block_b)->removeLast();
                        
                        // update iter_blocks
                        
                        for(int j=2;j<=iter_blocks.retrieve(block_b)->getLength();j++)
                        {
                            tmp = iter_blocks.retrieve(block_b)->retrieve(j);
                            there = false;
                            for(int k=2;k<=iter_blocks.retrieve(block_a)->getLength();k++)
                                if(tmp == iter_blocks.retrieve(block_a)->retrieve(k))
                                    there = true;
                            if(there == false)
                                iter_blocks.retrieve(block_a)->append(tmp);
                        }
                        
                        while(iter_blocks.retrieve(block_b)->getLength()>1)
                            iter_blocks.retrieve(block_b)->removeLast();
                        
                        
                        if(DEBUG){
                            
                            int **map;
                            
                            map = (int **) new int*[m_org->height()];
                            for (int j=0; j<m_org->height(); j++) map[j] = (int *) new int[m_org->width()];
                            
                            for(int j=0;j<m_org->height();j++)
                                for(int k=0;k<m_org->width();k++)
                                    map[j][k] = 0;
                            
                            for(int j=1;j<=iter_blocks.getLength();j++)
                            {
                                if(iter_blocks.retrieve(j)->getLength()>1)
                                {
                                    for(int k=2;k<=iter_blocks.retrieve(j)->getLength();k++)
                                    {
                                        tmp = iter_blocks.retrieve(j)->retrieve(k);
                                        for(int l=1;l<=blocks.retrieve(tmp)->getLength();l++)
                                        {
                                            p = blocks.retrieve(tmp)->retrieve(l);
                                            map[p.y][p.x] = iter_blocks.retrieve(j)->retrieve(1);
                                            
                                            //pseudocolor
                                            if(block_a == j){
                                                imRef(im_result, p.x, p.y) = colors[j];
                                            }else{
                                                imRef(im_result,  p.x, p.y).r = (uchar) 255;
                                                imRef(im_result,  p.x, p.y).g = (uchar) 255;
                                                imRef(im_result,  p.x, p.y).b = (uchar) 255;
                                            }
                                        }
                                    }
                                }
                            }
                            
                            
                            for(int j=0; j<m_org->height();j++)
                                for(int k=0;k<m_org->width();k++)
                                {
                                    q.x = k; q.y = j;
                                    {
                                        for(int l=2;l<=3;l++)
                                        {
                                            p = q + shift[l];
                                            
                                            if(p.x>=0&&p.y>=0&&p.x<m_org->width()&&p.y<m_org->height())
                                            {
                                                if(map[p.y][p.x] != map[q.y][q.x])
                                                {
                                                    imRef(im_result, (q).x, (q).y).r = (uchar) 0;
                                                    imRef(im_result, (q).x, (q).y).g = (uchar) 0;
                                                    imRef(im_result, (q).x, (q).y).b = (uchar) 0;
                                                }
                                            }
                                        }
                                    }
                                }
                            
                            
                            for(int ii=0;ii<m_org->height();ii++) delete [] map[ii];
                            delete [] map;
                            
                            
                            frames.append(im_result->copy());
                            
                        }
                        
                        
                    }// combine is over
                }
                
                
            } // one iteration is over
            
		}
        
        
		for(int j=1;j<=iter_neibors_now.getLength();j++)
		{
			for(int k=2;k<=iter_neibors_now.retrieve(j)->getLength();k++)
				delete [] graph_weight[j][k];
			if(iter_neibors_now.retrieve(j)->getLength()>1)
				delete [] graph_weight[j];
		}
        
		delete [] graph_weight;
        
	}
    
    
    //main image ; aint debug
    int **map;
    
    map = (int **) new int*[m_org->height()];
    for (int j=0; j<m_org->height(); j++) map[j] = (int *) new int[m_org->width()];
    
    for(int j=0;j<m_org->height();j++)
        for(int k=0;k<m_org->width();k++)
            map[j][k] = 0;
    
    for(int j=1;j<=iter_blocks.getLength();j++)
    {
        if(iter_blocks.retrieve(j)->getLength()>1)
        {
            for(int k=2;k<=iter_blocks.retrieve(j)->getLength();k++)
            {
                tmp = iter_blocks.retrieve(j)->retrieve(k);
                for(int l=1;l<=blocks.retrieve(tmp)->getLength();l++)
                {
                    p = blocks.retrieve(tmp)->retrieve(l);
                    map[p.y][p.x] = iter_blocks.retrieve(j)->retrieve(1);
                    
                    //pseudocolor
                    imRef(im_result, p.x, p.y) = colors[j];
                }
            }
        }
    }
    
    
    for(int j=0;BORDER && j<m_org->height();j++)
        for(int k=0;k<m_org->width();k++)
        {
            q.x = k; q.y = j;
            {
                for(int l=2;l<=3;l++)
                {
                    p = q + shift[l];
                    
                    if(p.x>=0&&p.y>=0&&p.x<m_org->width()&&p.y<m_org->height())
                    {
                        if(map[p.y][p.x] != map[q.y][q.x])
                        {
                            imRef(im_result, (q).x, (q).y).r = (uchar) 0;
                            imRef(im_result, (q).x, (q).y).g = (uchar) 0;
                            imRef(im_result, (q).x, (q).y).b = (uchar) 0;
                        }
                    }
                }
            }
        }
    
    
    for(int ii=0;ii<m_org->height();ii++) delete [] map[ii];
    delete [] map;
    
    
    frames.append(im_result->copy());
    
    
    // clear
	tmp = iter_blocks.getLength();
    
    while(!iter_blocks.isEmpty())
	{
		while(!iter_blocks.retrieve(tmp)->isEmpty())
			iter_blocks.retrieve(tmp)->removeLast();
        
		while(!iter_neibors.retrieve(tmp)->isEmpty())
			iter_neibors.retrieve(tmp)->removeLast();
        
		while(!iter_neibors_next.retrieve(tmp)->isEmpty())
			iter_neibors_next.retrieve(tmp)->removeLast();
        
		while(!iter_neibors_consis.retrieve(tmp)->isEmpty())
			iter_neibors_consis.retrieve(tmp)->removeLast();
        
		
		iter_blocks.removeLast();
		iter_neibors.removeLast();
		iter_neibors_next.removeLast();
		iter_neibors_consis.removeLast();
        
		tmp--;
	}
    
	delete [] ave_block_r;
	delete [] ave_block_g;
	delete [] ave_block_b;
    
    return frames;
    
}





float Segmentation::CueEnhancement(int block, int neighbor, int sign)
{
	float delta = 0;
	int num;
	int num1, num2,num3;
    
	float pm,pp;
	CPoint pixel;
	float npixel;
	float ave_r1,ave_g1,ave_b1;
	float ave_r2,ave_g2,ave_b2;
	float ave_r3,ave_g3,ave_b3;
	float npixel1;
	float mean_r1,mean_g1,mean_b1;
	float mean_r2,mean_g2,mean_b2;
	int counter_n = 0;
	int counter = 0;
	int n0 = 1;   // upper bound on the number of tests
	int m1 = iter_blocks.retrieve(block)->getLength()*0.4 + 1;
	int m2 = iter_blocks.retrieve(neighbor)->getLength()*0.4 + 1;
	List<int> ran1;
	List<int> ran2;
	int *calculate;
	int tmp1;
    
	float min_dis = 1000;
	float tmp = 0;
	
	int size1 = iter_blocks.retrieve(block)->getLength();
	int size2 = iter_blocks.retrieve(neighbor)->getLength();
	delta = 0;
    
    
	while(counter_n<=n0)
	{
		//step2
        
		npixel = 0;
		ave_r1 = 0;ave_g1 = 0;ave_b1 = 0;
		ave_r2 = 0; ave_g2 = 0; ave_b2 = 0;
		ave_r3 = 0; ave_g3 = 0; ave_b3 = 0;
		
		calculate = new int[iter_blocks.retrieve(block)->getLength()+1];
        
		for(int i=1;i<=iter_blocks.retrieve(block)->getLength();i++)
			calculate[i]=iter_blocks.retrieve(block)->retrieve(i);
        
		for(int i=1;i<=m1;i++)
		{
			tmp1 = RandomNum(size1);
            
			num = calculate[tmp1];
			ran1.append(num);
            
			npixel = npixel + blocks.retrieve(num)->getLength();
			for(int j=1;j<=blocks.retrieve(num)->getLength();j++)
			{
				pixel = blocks.retrieve(num)->retrieve(j);
				ave_r1 = ave_r1 + m_org->access[pixel.y][pixel.x].r;
				ave_g1 = ave_g1 + m_org->access[pixel.y][pixel.x].g;
				ave_b1 = ave_b1 + m_org->access[pixel.y][pixel.x].b;
                
			}
		}
        
		ave_r3 = ave_r1;
		ave_g3 = ave_g1;
		ave_b3 = ave_b1;
        
		ave_r1 = ave_r1/npixel;ave_g1 = ave_g1/npixel;ave_b1 = ave_b1/npixel;
        
		delete []calculate;
        
		calculate = new int[iter_blocks.retrieve(neighbor)->getLength()+1];
		for(int i=1;i<=iter_blocks.retrieve(neighbor)->getLength();i++)
			calculate[i] = iter_blocks.retrieve(neighbor)->retrieve(i);
        
		npixel1 = 0 ;
		for(int i=1;i<=m2;i++)
		{
			tmp1 = RandomNum(size2);
			num = calculate[tmp1];
			ran2.append(num);
            
			npixel1 = npixel1 + blocks.retrieve(num)->getLength();
            
			for(int j=1;j<=blocks.retrieve(num)->getLength();j++)
			{
				pixel = blocks.retrieve(num)->retrieve(j);
				
				ave_r2 = ave_r2 + m_org->access[pixel.y][pixel.x].r;
				ave_g2 = ave_g2 + m_org->access[pixel.y][pixel.x].g;
				ave_b2 = ave_b2 + m_org->access[pixel.y][pixel.x].b;
			}
		}
        
		ave_r3 = ave_r3 + ave_r2;
		ave_g3 = ave_g3 + ave_g2;
		ave_b3 = ave_b3 + ave_b2;
        
		ave_r2 = ave_r2/npixel1;
		ave_g2 = ave_g2/npixel1;
		ave_b2 = ave_b2/npixel1;
        
		ave_r3 = ave_r3/(npixel1+npixel);
		ave_g3 = ave_g3/(npixel1+npixel);
		ave_b3 = ave_b3/(npixel1+npixel);
        
		delete []calculate;
        
        
		float expone = 0;
		pm = 0; pp	= 0 ;
        
		expone = 0.6*abs(ave_r1-ave_r3)+0.3*abs(ave_g1-ave_g3)+0.1*abs(ave_b1-ave_b3);
		pm = delta1*exp(-0.5*expone);
        
		expone = 0.6*abs(ave_r1-ave_r2)+0.3*abs(ave_g1-ave_g2)+0.1*abs(ave_b1-ave_b2);
		pp = 1 - exp(-delta2*expone);
		
		delta = delta + log(pm/pp);
        
		//step5: decide if the region can be distinguished
		float a,b;
		a = log((1-pmiss)/pfa); b = log(pmiss*(1-pfa));
		mean_r1 = 0; mean_g1 = 0; mean_b1 = 0;
		mean_r2 = 0; mean_g2 = 0; mean_b2 = 0;
		
        
		if(sign == 1)	// working on block_a
		{
			// consistent
			if(delta>=a || (counter_n==n0&&delta>0)) {
                
				while(!ran1.isEmpty()) ran1.removeLast();
				while(!ran2.isEmpty()) ran2.removeLast();
                
				iter_neibors_consis.retrieve(block)->append(true);
				iter_neibors_consis.retrieve(neighbor)->append(true);
                
                
				tmp = 0;counter = 0;
				for(int i=1;i<=iter_blocks.retrieve(block)->getLength();i++)
				{
					num1 = iter_blocks.retrieve(block)->retrieve(i);
                    
					for(int j=1;j<=allneibor.retrieve(num1)->getLength();j++)
					{
						num2 = allneibor.retrieve(num1)->retrieve(j);
						for(int k=1;k<=iter_blocks.retrieve(neighbor)->getLength();k++)
						{
							num3 = iter_blocks.retrieve(neighbor)->retrieve(k);
							if(num2 == num3)
							{
								tmp = abs(reg_ave_r[num1-1]-reg_ave_r[num3-1])+
                                abs(reg_ave_g[num1-1]-reg_ave_g[num3-1])+
                                abs(reg_ave_b[num1-1]-reg_ave_b[num3-1]);
                                
								if(min_dis>tmp)
									min_dis = tmp;
                                
                                k = iter_blocks.retrieve(neighbor)->getLength()+1;
							}
							
						}
					}
				}
                
				return(min_dis);
			}
            
			// inconsistent
			if(delta<=b || (counter_n==n0&&delta<0)) {
                
                
                
				while(!ran1.isEmpty()) ran1.removeLast();
				while(!ran2.isEmpty()) ran2.removeLast();
                
				iter_neibors_consis.retrieve(block)->append(false);
				iter_neibors_consis.retrieve(neighbor)->append(false);
                
				min_dis = 1000;
				tmp = 0;counter =0;
				for(int i=1;i<=iter_blocks.retrieve(block)->getLength();i++)
				{
					num1 = iter_blocks.retrieve(block)->retrieve(i);
                    
					for(int j=1;j<=allneibor.retrieve(num1)->getLength();j++)
					{
						num2 = allneibor.retrieve(num1)->retrieve(j);
						for(int k=1;k<=iter_blocks.retrieve(neighbor)->getLength();k++)
						{
							num3 = iter_blocks.retrieve(neighbor)->retrieve(k);
							if(num2 == num3)
							{
								tmp = abs(reg_ave_r[num1-1]-reg_ave_r[num3-1])+
								abs(reg_ave_g[num1-1]-reg_ave_g[num3-1])+
								abs(reg_ave_b[num1-1]-reg_ave_b[num3-1]);
                                
								if(min_dis>tmp)
									min_dis = tmp;
                                
								k = iter_blocks.retrieve(neighbor)->getLength()+1;
							}
							
						}
					}
				}
                
				return(min_dis);
			}
		}
        
		if(sign == 2)
		{
			if(delta>=a || (counter_n>=n0&&delta>0)) {
                
				while(!ran1.isEmpty()) ran1.removeLast();
				while(!ran2.isEmpty()) ran2.removeLast();
                
				iter_neibors_consis.retrieve(block)->append(true);
				iter_neibors_consis.retrieve(neighbor)->append(true);
				min_dis = 1000;
				tmp = 0;counter = 0;
				for(int i=1;i<=iter_blocks.retrieve(block)->getLength();i++)
				{
					num1 = iter_blocks.retrieve(block)->retrieve(i);
                    
					for(int j=1;j<=allneibor.retrieve(num1)->getLength();j++)
					{
						num2 = allneibor.retrieve(num1)->retrieve(j);
						for(int k=1;k<=iter_blocks.retrieve(neighbor)->getLength();k++)
						{
							num3 = iter_blocks.retrieve(neighbor)->retrieve(k);
							if(num2 == num3)
							{
                                
								tmp = abs(reg_ave_r[num1-1]-reg_ave_r[num3-1])+
                                abs(reg_ave_g[num1-1]-reg_ave_g[num3-1])+
                                abs(reg_ave_b[num1-1]-reg_ave_b[num3-1]);
                                
								if(min_dis>tmp)
									min_dis = tmp;
                                
								k = iter_blocks.retrieve(neighbor)->getLength()+1;
							}
							
						}
					}
				}
                
				return(min_dis);
			}
            
			if(delta<=b || (counter_n>=n0&&delta<0)) {
                
				while(!ran1.isEmpty()) ran1.removeLast();
				while(!ran2.isEmpty()) ran2.removeLast();
                
				iter_neibors_consis.retrieve(block)->append(false);
				iter_neibors_consis.retrieve(neighbor)->append(false);
                
				min_dis = 1000;
				tmp = 0;counter = 0;
				for(int i=1;i<=iter_blocks.retrieve(block)->getLength();i++)
				{
					num1 = iter_blocks.retrieve(block)->retrieve(i);
                    
					for(int j=1;j<=allneibor.retrieve(num1)->getLength();j++)
					{
						num2 = allneibor.retrieve(num1)->retrieve(j);
						for(int k=1;k<=iter_blocks.retrieve(neighbor)->getLength();k++)
						{
							num3 = iter_blocks.retrieve(neighbor)->retrieve(k);
							if(num2 == num3)
							{
								tmp = abs(reg_ave_r[num1-1]-reg_ave_r[num3-1])+
                                abs(reg_ave_g[num1-1]-reg_ave_g[num3-1])+
                                abs(reg_ave_b[num1-1]-reg_ave_b[num3-1]);
                                
								if(min_dis>tmp)
									min_dis = tmp;
                                
								k = iter_blocks.retrieve(neighbor)->getLength()+1;
							}
							
						}
					}
				}
				return(min_dis);
			}
        }
		counter_n = counter_n + 1;
	}
    
}



float Segmentation::Combo(int size1, int size2, int m)
{
	double t1=size1, t2=size2, t3=size1+size2;
    if(m>=t1) t1 = 1;
    else
        for(int i=t1-1;i>=t1-m;i++)
            t1 = t1 * i;
    
    if(m>=t2) t2 = 1;
    else
        for(int i=t2-1;i>=t2-m;i++)
            t2 = t2 * i;
    
    for(int i=t3-1;i>=t3-m;i++)
        t3 = t3 * i;
    
    return (t1+t2)/t3;
    
    
}

int Segmentation::RandomNum(int size)
{
	int num;
	num = rand() % size + 1;       // random number 1-size
	return num;
}


void Segmentation::DeletePreheaders(void)
{
    //clean contructor
    delete im_result;
    
    //cleaning building blocks
    
    for(int i=0;i<m_h;i++)
		delete [] label_image[i];
	delete []label_image;
    
    while(!blocks.isEmpty())
	{
        List<CPoint>* back = blocks.retrieveLast();
        
        back->clear();
        delete back;
        
        blocks.removeLast();
    }
    
    blocks.clear();
    
    while(!allneibor.isEmpty())
	{
        List<int>* back = allneibor.retrieveLast();
        
        back->clear();
        delete back;
        
        allneibor.removeLast();
    }

    allneibor.clear();
    allneibor = NULL;
    
    //end cleaning building blocks
    
    //cleaning region merging
    
    delete [] reg_ave_r;
    delete [] reg_ave_g;
    delete [] reg_ave_b;
    
    delete []blockmask;
    
    labels.clear();
    
    while(!neibor.isEmpty())
	{
        List<int>* back = neibor.retrieveLast();
        
        back->clear();
        delete back;
        
        neibor.removeLast();
    }
    
    p = NULL;
    q = NULL;
    s = NULL;
    m_point = NULL;
    p1 = NULL;
    p2 = NULL;
    
    while(!iter_blocks.isEmpty())
	{
        List<int>* back = iter_blocks.retrieveLast();
        
        back->clear();
        delete back;
        
        iter_blocks.removeLast();
    }
    
    iter_blocks.clear();
    
    while(!iter_neibors_consis.isEmpty())
	{
        List<int>* back = iter_neibors_consis.retrieveLast();
        
        back->clear();
        delete back;
        
        iter_neibors_consis.removeLast();
    }
    
    iter_neibors_consis.clear();
    
    
    //end region merging
    
}
