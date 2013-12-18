/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
/*
#include <cstdio>
#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "pnmfile.h"
#include "segment-image.h"

#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <stdio.h>

#include <sys/stat.h>

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

bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
  if (argc != 6) {
    fprintf(stderr, "usage: %s sigma k min input(ppm) output(ppm)\n", argv[0]);
    return 1;
  }
    
    float sigma = 0.5;
    float k = 100;
    int min_size = 50;

    struct dirent *de=NULL;
    DIR *d=NULL;
    
    //std::string dirName = "/Users/carlos/Google Drive/Havop/Clientes/CICS/EYEFIT/Ferrugem da Soja/200X/Avançado/";
    
    //std::string dirName = "/Users/carlos/Documents/PUC/Monografia/BSR/BSDS500/data/images/test/";
    
    std::string dirName = "/Users/carlos/Documents/PUC/Monografia/grafo/";
    
    
    d= opendir(dirName.c_str());
    if(d == NULL)
    {
        perror("Couldn't open directory");
        return(2);
    }
    
    // Loop while not NULL
    unsigned char isFile =0x8;
    
    while(de = readdir(d)){
        
                
        
        std::string fileNameOri = dirName + de->d_name;
        
        std::string fileName = de->d_name;
        int lastindex = fileName.find_last_of(".");
        fileName = fileName.substr(0, lastindex);

        
        if ( de->d_type == isFile )
        {
            ///Users/carlos/Google Drive/Havop/Clientes/CICS/EYEFIT/
            std::string ppmFileName ="/Users/carlos/Documents/PUC/Monografia/benchmark/ppm/";
            ppmFileName +=  fileName ;
            ppmFileName += ".ppm";
            
            std::string ppmResultFileName ="/Users/carlos/Documents/PUC/Monografia/benchmark/ppm/";
            ppmResultFileName +=  fileName ;
            ppmResultFileName += ".processed.ppm";

            
            std::string resultFileName = "/Users/carlos/Documents/PUC/Monografia/benchmark/result/";
            resultFileName += fileName ;
            resultFileName += "_10_5_segm.bmp";
            
            std::string resultFileNamea = "/Users/carlos/Documents/PUC/Monografia/benchmark/result/";
            resultFileNamea +=  fileName ;
            resultFileNamea += "_10_5.bmp";
            
            std::string resultFileNameJPG = "/Users/carlos/Documents/PUC/Monografia/benchmark/result/";
            resultFileNameJPG +=  fileName ;
            resultFileNameJPG += "_10_5.jpg";
            
            if(!fileExists(ppmFileName)){
                printf("\n== Converting to ppm.. %s",de->d_name);
                convertimage( fileNameOri , ppmFileName);
            }
            
            printf("-- %s\n",fileName.c_str());
        
            printf("loading input image.\n");
            image<rgb> *input = loadPPM(ppmFileName.c_str());
        
            printf("processing\n");
            int num_ccs;
            image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs);
            savePPM(seg,ppmResultFileName.c_str());
        
            convertimage( ppmResultFileName , resultFileName);
            convertimage( fileNameOri , resultFileNamea);
            convertimage( ppmResultFileName , resultFileNameJPG);
            
            printf("got %d components\n", num_ccs);
            printf("done! uff...thats hard work.\n");
            
        }
    }
        
    
  closedir(d);

  return 0;
}

*/

