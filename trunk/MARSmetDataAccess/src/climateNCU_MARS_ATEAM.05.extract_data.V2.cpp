#include <iostream>
#ifndef __BCplusplus__
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#else
	#include <fstream.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <dir.h>
#endif
#define maxpars 5

#include <math.h>

/*
LAST UPDATE 04/2009 (V1)
LAST UPDATE 06/2009 (V2)
 
This file reads data values from the binary climate files that are generated for the NitroEurope-IProject
on the basis of the MARS and ATEMA/CRU datasets and cover EU27+3 for the period 1901-2000.
The parameter included are Tmin [degree Celsius], Tmax [degree Celsius], Pre [mm], Vapour pressure [hPa], global radiation [MJ/m2/day].

All files are organized in groups of 10 years and 1500 NCUs. The reason is that each individual file should be kept at a
reasonable size (prefereably less than 100 MB) and the number of files a user needs should be limited for both applications
for a particular region or time slice.

Four parameters are stored together (Tmin, Tmax, Pre, Vap)
One parameter is stored extra (Rad)

Each set of files (for one time slice) is accompanied by a "map" file that stores the first bit for each NCU.
The loops are for each NCU: Loop(years,loop(days,loop(parameters))).
In a header of each climate_map, the following information are stored:

     numdim          --> Number of dimensions (four in this case: NCU, Years, Days, Parameters)

  1. Dimension: NCUs: 
     the first dimension is used to split the data-files, therefore a "gap" between the 
     must be defined

     dim_beg[0]      --> Start Value (1)
     dim_end[0]      --> Last Value (4091)
     dim_stp[0]      --> Step (1)
     dim1_gap        --> The files are separated by the first dimension

 2. Dimension: Years
    Attention: there is no information on the map about differnt time slices; 
    this must be programmed

    dim_beg[1]      --> Start value (First year in the time slice)
    dim_end[1]      --> Last value (Last year in the time slice)
    dim_stp[1]      --> Step (1)

 3. Dimension: Days

    dim_beg[2]      --> Start value (1)
    dim_end[2]      --> Last value (365)
    dim_stp[2]      --> Step (1)
                    
 4. Dimension: Parameters
    Attention: the last dimension needs only the number of values  

    dim_end[3]       -->  Number of parameters


 To read an individual datum, it must be defined by
 - File structure (location of climate data and place to put results), for example
   -- 
 - Data files-trunc for both sets of files, for example
   -- MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN for the files containing the first four parameters
   -- radCRU_NCU.i686-pc-cygwin_g++3.4.4_LITTLE_ENDIAN for the files containing the global radiation
 - Info on time slicing: absolute start year (1901) and size of each slice (10 years)
 - Identification of the numbers to read: NCU, Year, Day, Parameter
   Attention!!
   For each information you must give start and end value
   The program will cut out the n-dimensional array
 - In addition, a file is required that links NCU-numbers (from 1 to 4091) with the NCU-IDs.
   In some cases, only the NCU-ID will be known and this enables the program to find their numbers.
   We include this file as climateNCU_MARS_ATEAM.04.readbin_ncus.txt (name must be provided to the program)
 This information must be stored in a file called "climateNCU_MARS_ATEAM.05.extract_data.txt"


EXAMPLE OF A VALID INPUT FILE:
=============================================================================
4_Lncu_year
4_Lncu_year_test
MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN
radCRU_NCU.i686-pc-cygwin_g++3.4.4_LITTLE_ENDIAN
climateNCU_MARS_ATEAM.04.readbin_ncus.txt
1901
10
2036,2037
1982,1985
237,240
1,4
=============================================================================

... ANNOTATED (NOT VALID - ONLY HERE FOR EXPLANATION!!!)
=============================================================================
marsateam                                                  --> if you start the program at e.g. /home/nitroeurope/, the files are archived at /home/nitroeurope/marsateam
marsateam_test                                             --> if you start the program at e.g. /home/nitroeurope/, the files are archived at /home/nitroeurope/marsateam_test
MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN    --> for example for MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN.1901-1910.00001_01500.bin
radCRU_NCU.i686-pc-cygwin_g++3.4.4_LITTLE_ENDIAN           --> for example for radCRU_NCU.i686-pc-cygwin_g++3.4.4_LITTLE_ENDIAN.1991-2000.00001_01500.bin
climateNCU_MARS_ATEAM.04.readbin_ncus.txt                  --> this file must exist at the same location as the program is run!!
                                                               it is need in the case an NCU is called with the ID, for example 
                                                               100249520 instead of the running number (in this case 1)

                                                           In the following the structure of the files is defined
1901                                                       --> first year considered
10                                                         --> number of years stored in one set of files

                                                           In the following the "window" for which the data should be extracted is defined
2036,2037                                                  --> NCUs (all between 2036 and 2037 incl.)
1982,1985                                                  --> years (all between 1982 and 1985 incl.)
237,240                                                    --> days (Julian days, all between 237 and 240 inc.)
1,5                                                        --> parameters (all between parameter #1 (Tmin) and #5 (Rad) incl.)
=============================================================================


Histoy of this file:
- first version april 2009
- second version june 2009, with extension to global-radiation files

*/

int getNCU_ID(int,char[100]);
int getNCU(int,char[100]);

int main()
{
	FILE *clim_map1, *clim_map2, *clim_bin1, *clim_bin2;
    FILE *clim_write;
	char Cclim_map1[200], Cclim_map2[200], Cclim_bin1[200], Cclim_bin2[200], Cclim_out[200];
	char Cclim_name1[200], Cclim_name2[200], clim_inpath[200], clim_outpath[200];
    char Cncu[100];
    
	int hsmu,hsmuID,rhsmuID;
	int yearstart, yearsfile;
	int read_start[maxpars], read_end[maxpars];
	int dim_beg[maxpars],dim_stp[maxpars],dim_end[maxpars];
    int idummy1,idummy2;
    float fdummy; 

    

	long int posmap1,posmap2,posbin1,posbin2,posclim;

    char pars[maxpars][5];
    sprintf(pars[0],"tmn");
    sprintf(pars[1],"tmx");
    sprintf(pars[2],"pre");
    sprintf(pars[3],"vap");
    sprintf(pars[4],"rad");
	char line[50000];

#ifdef __BCplusplus__
	chdir("..");
	chdir("..");
#endif
	//Read informaion from input-text file
    sprintf(Cclim_map1,"climateNCU_MARS_ATEAM.05.extract_data.V2n.txt");
    clim_map1=fopen(Cclim_map1,"r");  //rb
	if (clim_map1==NULL)
   {
		char buffer[200];
		getcwd(buffer, sizeof(buffer));
		cout << buffer << endl;
		cout << "Cannot open " << Cclim_map1 <<"\n";
      exit(0);
   }
	cout<<"clim_map1: "<<Cclim_map1<<"\n";
    fscanf(clim_map1,"%s",clim_inpath);
	cout<<"clim_inpath: "<<clim_inpath<<"\n";
    fscanf(clim_map1,"%s",clim_outpath);
	cout<<"clim_outpath: "<<clim_outpath<<"\n";
	fscanf(clim_map1,"%s",Cclim_name1);
	cout<<"Cclim_name1: "<<Cclim_name1<<"\n";
	fscanf(clim_map1,"%s",Cclim_name2);
	cout<<"Cclim_name2: "<<Cclim_name2<<"\n";
	fscanf(clim_map1,"%s",Cncu);
	cout<<"ncu: "<<Cncu<<"\n";
	fscanf(clim_map1,"%d",&yearstart);
	cout<<"year_range_start: "<<yearstart<<"\n";
	fscanf(clim_map1,"%d",&yearsfile);
	cout<<"year_range_end: "<<yearsfile<<"\n";
    fscanf(clim_map1,"%d,%d",&read_start[0],&read_end[0]);
    fscanf(clim_map1,"%d,%d",&read_start[1],&read_end[1]);
    fscanf(clim_map1,"%d,%d",&read_start[2],&read_end[2]);
    fscanf(clim_map1,"%d,%d",&read_start[3],&read_end[3]);
    //parameters count from 0 to maxpars-1
    read_start[3]=read_start[3]-1;
	read_end[3]=read_end[3]-1;

    //int i=0;cout<<"read_start["<<i<<"]: "<<read_start[i]<<", read_end["<<i<<"]: "<<read_end[i]<<"\n";
    //i=1;cout<<"read_start["<<i<<"]: "<<read_start[i]<<", read_end["<<i<<"]: "<<read_end[i]<<"\n";
    //i=2;cout<<"read_start["<<i<<"]: "<<read_start[i]<<", read_end["<<i<<"]: "<<read_end[i]<<"\n";
    //i=3;cout<<"read_start["<<i<<"]: "<<read_start[i]<<", read_end["<<i<<"]: "<<read_end[i]<<"\n";
    fclose(clim_map1);

    int numdim, dim1_gap,startpix,nnendpix;
    int ncomment,numyears, numdays, firstncunut,stepncunut,nncunut;

    //open map ... all maps are equally constructed, so it does not matter which one ...
    sprintf(Cclim_map1,"%s\%s.%d-%d.map",clim_inpath,Cclim_name1,yearstart,yearstart+yearsfile-1);
    cout <<  "map file 1: " << Cclim_map1 << endl;
    sprintf(Cclim_map2,"%s\%s.%d-%d.map",clim_inpath,Cclim_name2,yearstart,yearstart+yearsfile-1);
    cout <<  "map file 2: " << Cclim_map2 << endl;
    clim_map1=fopen(Cclim_map1,"rb");  //rb
    clim_map2=fopen(Cclim_map2,"rb");  //rb
    if (clim_map1==NULL){cout << "Cannot open " << Cclim_map1 <<"\n"; exit(0);}
    if (clim_map2==NULL && read_end[3]==5){cout << "Cannot open " << Cclim_map2 <<"\n"; exit(0);}
    fread(&numdim,4,1,clim_map1);
    fread(&dim_beg[0],4,1,clim_map1); // 1. Dimension: HSMUs
    fread(&dim_end[0],4,1,clim_map1);
    fread(&dim_stp[0],4,1,clim_map1);
    fread(&dim1_gap,4,1,clim_map1);     // The files are separated by the first dimension
    fread(&dim_beg[1],4,1,clim_map1);  // 2. Dimension: Years
    fread(&dim_end[1],4,1,clim_map1);
    fread(&dim_stp[1],4,1,clim_map1);
    fread(&dim_beg[2],4,1,clim_map1);  // 3. Dimension: Days
    fread(&dim_end[2],4,1,clim_map1);
    fread(&dim_stp[2],4,1,clim_map1);
    fread(&dim_end[3],4,1,clim_map1);  //  4. Dimension: Parameters

    numdays=(dim_end[2]-dim_beg[2]+1)/dim_stp[2];
    numyears=(dim_end[1]-dim_beg[1]+1)/dim_stp[1];


    sprintf(Cclim_out,"%s/%d-%d.%d-%d.%d-%d.%d-%d.txt",clim_outpath,read_start[0],read_end[0],read_start[1],read_end[1],read_start[2],read_end[2],read_start[3],read_end[3]);
    clim_write=fopen(Cclim_out,"w");  //rb
    if (clim_write==NULL){cout << "Cannot open " << Cclim_out <<"\n"; exit(0);}
    fprintf(clim_write,"hsmu,hsmuID,year,jday",pars[0],pars[1],pars[2],pars[3],pars[4]);
    for (int para=read_start[3];para<=read_end[3];para++)
    {
        fprintf(clim_write,",%s",pars[para]);
    }


    for (int ncu=read_start[0];ncu<=read_end[0];ncu+=dim_stp[0])
    {
        //If NCU-ID is given, convert to NCU-number, otherwise get the ID
        int hsmuID=ncu;
        int hsmu=ncu;
        if (hsmu>dim_end[0]) {hsmu=getNCU(hsmu,Cncu);}
        if (hsmuID==hsmu) {hsmuID=getNCU_ID(hsmu,Cncu);}

        //Check position of first bit of the NCU in the climate-map.
        //Attention: a few NCUs are missing (small islands, not included in the NEU-data, so they should not occur)
        //           however, the program will not give any numbers for these.
        posmap1=1000+12*(hsmu-1);
        fseek(clim_map1,posmap1,SEEK_SET);
        fseek(clim_map2,posmap1,SEEK_SET);
        //Read Start-Bit for the NCU and calculated bit for the actual value
        fread(&idummy1,4,1,clim_map1);
        fread(&posbin1,8,1,clim_map1);
        fread(&idummy1,4,1,clim_map2);
        fread(&posbin2,8,1,clim_map2);

        if(idummy1!=hsmu || hsmuID==999)
        {
            //cout<<"Attention: wrong NCU in climate-map. NCU="<<hsmu<<" and read: "<<idummy1<<"\n";
            fprintf(clim_write,"\n%d,%d --> no data available",hsmu,hsmuID);
        }
        else
        {
            for (int year=read_start[1];year<=read_end[1];year+=dim_stp[1])
            {
                //Open Data-File!

                //First determine the time-slice
                int filey1=yearstart;
                int filey2=filey1+yearsfile-1;
                while (filey1+yearsfile <= year)
                {
                    filey1+=yearsfile ;
                    filey2=filey1+yearsfile-1;
                }

                //File names have trailing zeros ... must be included here
                startpix=(hsmu-1)/dim1_gap;
                startpix=startpix*dim1_gap+1;
                nnendpix=startpix+dim1_gap-1;
                char zeroa[5], zeroe[5];
                if(startpix>=10000){sprintf(zeroa,"");};
                if(startpix<10000){sprintf(zeroa,"0");};
                if(startpix<1000){sprintf(zeroa,"00");};
                if(startpix<100){sprintf(zeroa,"000");};
                if(startpix<10){sprintf(zeroa,"0000");};
                if(nnendpix>=10000){sprintf(zeroe,"");};
                if(nnendpix<10000){sprintf(zeroe,"0");};
                if(nnendpix<1000){sprintf(zeroe,"00");};
                if(nnendpix<100){sprintf(zeroe,"000");};
                if(nnendpix<10){sprintf(zeroe,"0000");};
                sprintf(Cclim_bin1,"%s/%s.%d-%d.%s%d_%s%d.bin",clim_inpath,Cclim_name1,filey1,filey2,zeroa,startpix,zeroe,nnendpix);
                sprintf(Cclim_bin2,"%s/%s.%d-%d.%s%d_%s%d.bin",clim_inpath,Cclim_name2,filey1,filey2,zeroa,startpix,zeroe,nnendpix);
                clim_bin1=fopen(Cclim_bin1,"rb");
                clim_bin2=fopen(Cclim_bin2,"rb");
                if (clim_bin1==NULL)
                {
                	cout << "Cannot open " << Cclim_bin1 <<"\n";
                  exit(0);
                }
                if (clim_bin2==NULL && read_end[3]==4)
                {
                	cout << "Cannot open " << Cclim_bin2 <<"\n";
                  exit(0);
                }

                for (int day=read_start[2];day<=read_end[2];day+=dim_stp[2])
                {
                    fprintf(clim_write,"\n%d,%d,%d,%d",hsmu,hsmuID,year,day);
                    cout << hsmu << " " << year <<  " " << day << endl;
                    for (int para=read_start[3];para<=read_end[3];para++)
                    {

                        if(para<4)
                        {
                            posclim=posbin1+4*(

                                    +(hsmu-startpix)*(filey2-filey1+1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]*4  // full ncu's
                                    +(year-filey1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]*4                      // full years
                                    +(day-dim_beg[2])/dim_stp[2]*4                                                        // full days
                                    +para);
                            fseek(clim_bin1,posclim,SEEK_SET);
                            fread(&fdummy,4,1,clim_bin1);
                        }
                        if(para==4)
                        {
                            posclim=posbin2+4*(

                                    +(hsmu-startpix)*(filey2-filey1+1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]  // full ncu's
                                    +(year-filey1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]
                                    +(day-dim_beg[2])/dim_stp[2]

                                    );
                            fseek(clim_bin2,posclim,SEEK_SET);
                            fread(&fdummy,4,1,clim_bin2);
                        }
                        fprintf(clim_write,",%f",fdummy);
                    }
                }
                fclose(clim_bin1);
            }
        }
    }
    fclose(clim_map1);
 }

int getNCU_ID(int hsmu,char Cncu[100])
{
    FILE *ncu;
    //char Cncu[100];
    ncu=fopen(Cncu, "r");
    int hsmuID=999, idummy1, idummy2;
    if (ncu==NULL){
		char buffer[200];
		getcwd(buffer, sizeof(buffer));
		cout << buffer << endl;
		cout << "Cannot open " << Cncu <<"\n";
    	exit(0);
    }
    while ( fscanf(ncu, "%d %d", &idummy1,&idummy2) == 2 && hsmuID==999 )
    {
        if(idummy2==hsmu){hsmuID=idummy1;}
    }
    fclose(ncu);
    return hsmuID;
}
int getNCU(int hsmuID,char Cncu[100])
{
    FILE *ncu;
    //char Cncu[100];
    ncu=fopen(Cncu, "r");
    int hsmu=99999, idummy1, idummy2;
    if (ncu==NULL){
		char buffer[200];
		getcwd(buffer, sizeof(buffer));
		cout << buffer << endl;
		cout << "Cannot open " << Cncu <<"\n";
    	exit(0);
    }
    while ( fscanf(ncu, "%d %d", &idummy1,&idummy2) == 2 && hsmu==99999 )
    {
        if(idummy1==hsmuID){hsmu=idummy2;cout<<hsmu;}
        //cout<<idummy1<<","<<hsmuID<<"\n";
    }
    fclose(ncu);
    return hsmu;
}
