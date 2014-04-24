// drm.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>

#include "Segmentation.h"

//gb
#include "gb/misc.h"
#include "gb/pnmfile.h"
#include "gb/segment-image.h"

class Segmentation;

#include <iostream>

#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>

#define DEBUG false
#define TESTING true


std::string cexec(char* cmd) {
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

void cconvertimage(std::string file1 , std::string file2) {
    
    char buff[1000];
    sprintf(buff, "/opt/local/bin/convert -loop 1  -delay 20 '%s' '%s'", file1.c_str(),file2.c_str());
    
    printf(cexec(buff).c_str());
    
    
}


static bool _mkdir(const char *dir);
bool applyDRM( const char * filepath, const char * filename, float sigma, float k, int min_size, float delta1, float delta2, const char * output_path, bool usePseudoColor );

int main(int argc, const char * argv[])
{
    
    if(TESTING){
        
        
    }else if (argc < 6) {
        
        printf("            input --> input path of ppm video frames.\n");
        printf("           output --> output path of segmentation results.\n");
        printf("            sigma --> \n");
        printf("                k --> \n");
        printf("              min --> \n");
        printf("           delta1 --> \n");
        printf("           delta2 --> \n");
        
        return 1;
    }
    
    //inputs
    std::string ip("/Volumes/Macintosh HD 1/Users/carlos/Documents/PUC/Monografia/benchmark/input");
	std::string op("/Volumes/Macintosh HD 1/Users/carlos/Documents/PUC/Monografia/benchmark/output");
    
    const char * input_path = ip.c_str();
	const char * output_path = op.c_str();
    
    std::string d1("1.0");
	std::string d2("0.1");
    
    float sigma = 0.5;
    float k = 100;
    int min_size = 50;
    
	double delta1 = ::atof(d1.c_str());
	double delta2 = ::atof(d2.c_str());
    
    bool usePseudoColor = false;
    
    //vars
    DIR *pDir;
    struct dirent *pDirent;
    struct stat st;
    
    int num_frames, len;
    
    //dir initialization
    if (stat(output_path, &st) != 0) {
        // Directory does not exist
        if (mkdir(output_path, S_IRWXU) != 0) {
            fprintf(stderr,"Unable to create the output directories at %s",output_path);
        }
    }
    
    int i = 0;
    
    pDir = opendir(input_path);
    if (pDir != NULL) {
        while ((pDirent = readdir(pDir)) != NULL) {
            len = strlen(pDirent->d_name);
            
            if (len >= 4) {
                if (strcmp(".ppm", &(pDirent->d_name[len - 4])) == 0){
                    
                    
                    if(!applyDRM( input_path, pDirent->d_name , sigma, k, min_size, delta1, delta2, output_path, usePseudoColor )){
                        printf("<< exception!");
                        //return 1;
                    }
                    
                    i++;
                    
                }
            }
        }
    }
    
    
    return 1;
}

bool applyDRM( const char * input_path, const char * filename, float sigma, float k, int min_size, float delta1, float delta2, const char * output_path, bool usePseudoColor ){
    
    Segmentation* m_seg;
	
    char input_file[1024];
    snprintf(input_file,1023,"%s/%s",input_path,filename);
    
    int dot;
    
    //image initialization
    std::string m_imName(filename);
    
    image<rgb>* m_image = loadPPM(input_file);
    
    
    if ((dot = m_imName.find('.'))>0)
        m_imName = m_imName.substr(0,dot);
    
    if ( !m_image ) {
        
        printf("invalid input image %s !", filename );
        printf("\n");
        
        delete m_image;
        m_image = NULL;
        
        return false;
        
    }else{
        
        //applying gb
        printf(">>> segmenting %s\n", m_imName.c_str());
        
        int num_ccs;
        image<rgb>* m_image_seg = segment_image(m_image,
                                                sigma,
                                                k,
                                                min_size,
                                                &num_ccs,
                                                usePseudoColor);
        
        
        //saves the segmentation image
        struct stat st;
        char savepath_seg[1024];
        
        snprintf(savepath_seg, 1023, "%s/%s/", output_path,m_imName.c_str());
        
        if (stat(savepath_seg, &st) != 0) {
            // Directory does not exist
            if (_mkdir(savepath_seg) != 0) {
                fprintf(stderr,"Unable to create the segment output directories at %s",savepath_seg);
            }
        }
        
        snprintf(savepath_seg, 1023, "%s/%s/seg.ppm", output_path,m_imName.c_str());
        savePPM(m_image_seg,savepath_seg);
        
        if ( false ) {
            
            printf("invalid segmentation image!\n");
            return false;
            
        }else{
            
            printf("got %d components on gb\n", num_ccs);
            
            m_seg = (Segmentation *) new Segmentation(m_image);
            
            m_seg->pmiss = 0.03;
            m_seg->pfa = 0.03;
            
            //setting the deltas
            m_seg->delta1 = delta1;
            m_seg->delta2 = delta2;
            
            m_seg->LoadImages(m_image_seg, m_image );
            
            printf("building the blocks ...\n");
            m_seg->BuildBlocks();
            
            int num_comp;
            
            printf("starting the region merging...\n");
            //image<rgb> *im_result = m_seg->RegionMerging(num_comp);
            List<image<rgb>*> im_result = m_seg->RegionMerging(num_comp);
            
            printf("region merging done.\n");
            
            if(im_result.getLength() > 0){
                printf("#done! %d regions on drm\n",num_comp);
            }else{
                printf("fail!\n");
            }
            
            //dump result on respective path
            char savepath[1024];
            
            
            snprintf(savepath,1023,"%s/%s/%05d",output_path,m_imName.c_str(),num_comp);
            if (stat(savepath, &st) != 0) {
                // Directory does not exist
                if (_mkdir(savepath) != 0) {
                    fprintf(stderr,"Unable to create the output directories at %s",output_path);
                }
            }
            
            image<rgb>* current = im_result.retrieveLast();
            
            snprintf(savepath, 1023, "%s/%s/%05d/%05d.ppm", output_path,m_imName.c_str(), num_comp, 1);
            savePPM(current,savepath);
            
            char convertresult[1024];
            snprintf(convertresult, 1023, "%s.png", savepath);
            
            cconvertimage(savepath , convertresult);
            
            
            if(DEBUG){
                
                //frames
                snprintf(savepath, 1023, "%s/%s/%05d/%05d", output_path,m_imName.c_str(), num_comp, 1);
                if (stat(savepath, &st) != 0) {
                    // Directory does not exist
                    if (_mkdir(savepath) != 0) {
                        fprintf(stderr,"Unable to create the output directories at %s",output_path);
                    }
                }
                
                for(int i=1;i<=im_result.getLength();i++){
                    
                    current =  im_result.retrieve(i);
                    
                    // save the image
                    snprintf(savepath, 1023, "%s/%s/%05d/%05d/%05d.ppm", output_path,m_imName.c_str(), num_comp, 1, i);
                    savePPM(current,savepath);
                    
                    snprintf(convertresult, 1023, "%s.png", savepath);
                    
                    cconvertimage(savepath , convertresult);
                    remove(savepath);
                }
                
                char gifpath[1024];
                
                snprintf(savepath, 1023, "%s/%s/%05d/%05d/*.png", output_path,m_imName.c_str(), num_comp, 1);
                snprintf(gifpath, 1023, "%s/%s/%05d/%05d.gif", output_path,m_imName.c_str(), num_comp, 1);
                
                cconvertimage(savepath , gifpath);
                
            }
            
            while(!im_result.isEmpty())
            {
                image<rgb>* back = im_result.retrieveLast();
                
                delete back;
                
                im_result.removeLast();
            }
            
            im_result.clear();
            
            m_seg->DeletePreheaders();
            
            delete m_seg;
            
        }
        
        delete m_image_seg;
        m_image_seg = NULL;
        
        delete m_image;
        m_image = NULL;
        
    }
    
    return true;
}

static bool _mkdir(const char *dir) {
    
    char tmp[256];
    char *p = NULL;
    size_t len;
    
    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    return mkdir(tmp, S_IRWXU);
    
}

bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
