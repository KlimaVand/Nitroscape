#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
#define maxpars 5

/*
LAST UPDATE 04/2009 (V1)
LAST UPDATE 06/2009 (V2)
LAST UPDATE 07/2010 (V3)
 
This file reads data values from the binary climate files that are generated for the NitroEurope-IProject
on the basis of the MARS and ATEMA/CRU datasets and cover EU27+3 for the period 1901-2000 and of the REMO dataset (two scenarios)
for the period 2001-2100.
The parameter included are Tmin [degree Celsius], Tmax [degree Celsius], Pre [mm], Vapour pressure [hPa], global radiation [MJ/m2/day].
REMO data do not have Vapour pressure.

All files are organized in groups of years and NCUs as defined in the map-files. The reason is that each individual file should be kept at a
reasonable size (prefereably less than 100 MB) and the number of files a user needs should be limited for both applications
for a particular region or time slice. 

We consider each time slice as a single database (containing info on all spatial units and parameters), thus
the need to have one map-file per time-slice. These map-files are consequently (in principle) identical.
This program knows about this 'extra' and opens the correct time-slice & map-file from the info that is given.

This version reads daily data of the last version of the binary meteo-data file, which have been stored as 'short integer' (a 2 bytes each)
in order to save storage space. The conversion factor is given in the map-files. All parameters are stored together, which (for the marsateam-
dataset) is different to the first version, where Rad was stored separately from the other parameters (as it was ready at a later stage).


Each set of files (for one time slice) is accompanied by a "map" file that stores the first bit for each NCU.
The loops are for each NCU: Loop(years,loop(days,loop(parameters))).

Each map-file has a 'header' containing all the meta-information required:

---------------------->  CONTENT OF MAP-FILE HEADER <---------------------------------------

Byte   Length  Type          Name_in_program Content
0      8       Integer       startbyte       Byte where ncu-data start (end of meta-info)
4      4       Integer       numdim          Numnber of dimensions

then for each of the dimensions (without last dim=parameters)
       100     char[100]     dim_name[i]     Name of dimension
         4     Integer       dim_beg[i]      Start for dimension (first value)
         4     Integer       dim_end[i]      End for dimension (last value)
         4     Integer       dim_stp[i]      Step for dimension

last dimension=parameters
       100     char[100]                     PARAM
         4     Integer                       Number of parameters

then for each parameter (dim_end of last dimension)
       100     char[100]     Name of parameter
       100     char[100]     Unit of parameter.
         4     float         Conversion factor (to multiply the value with)


----------------------> CONTENT OF MAIN PART OF MAP-FILE <---------------------------------------
Then start of description of spatial units (first dimension)
at this point the pointer should be at position given in the first record

for each spatial unit
         8     Long int      Spatial-unit number ?? why long int??
         8     Long int      Position in bin-file
        20     char[20]      File-suffix indicating range of spatial-units
                             ?? suffix contains "_"
                             while files are named "-"


       *when no data are available in the bin-file (and no
        alternative pointer can be set) then the Position
        in the bin-file should be set to 3
        and no char[20] is written!
---------------------->  END OF MAP-FILE <---------------------------------------


---------------------->          IMPORTANT        <---------------------------------------
---------------------->  HOW TO USE THIS FILE !!! <---------------------------------------
---------------------->          IMPORTANT        <---------------------------------------

A) This program reads meteo-data from the binary files to illustrate the usage of the 
   data files. As an example, this program calculates monthly and annual averages 
   (or sums for precipitation). 

B) There are different data versions around (for NCUs). For the moment that are the following (08/2010)
   - marsateam data for HSMU (hsmu), values stored as single float
   - marsateam data for NCU (ncu), first version (values stored as single float). As these data
     were avaiable in two gos (first T, p, vapour pressure and later radiation) there are 
     two sets of data available (ds199)
   - marsateam data for NCU (ncushort), second version (values stored as short integer). In this version,
     the data are re-written to save storage space (and transfer times). All variables are
     now written into single files.
   - remo-scenarios for NCU (remo). These data are stored in the same format as the
     second version of the marsateam data for NCU.

   This program keeps the flexibility of reading whatever data need to be read. This comes at the 
   cost of a higher complexity of the program (many "if"s etc.) and partly obsolete pieces of code
   if you are using just the latest version. Sorry for that!

   ATTENTION!! the info above is only for "ncushort" and "remo", the information for "ncu"
   is found together with these data (ds199).

   The second drawback is that you need two text-files to steer the program:
   1) A file called "meteodataextraceannual.txt" which basically just informs the program
      which type of data need to be read and where additional info on what needs to be done
      is found. In addition, you can chose to calcualte annual or annual/monthly data or if you 
      want to extract a "window" of daily data too.
      You can also chose a level of command-line listing (for debugging). 
      An example of this file is shown here:

------example file: meteodataextraceannual.txt start------
marsateamextraceannualncushort.txt
ncushort
3
1

Line 1: name of file with input information
remoextraceannualncu.txt
Line 2: type of data (hsmu/ncu)
remo
Line 3: data to write:    1=only annual data
                          2=annual and monthly data
                          3=annual and monthly and daily data
Line 4: type of listing:  0=no listing
                          1=some listing
                          2=more listing
------example file: meteodataextraceannual.txt end-------

  2) The "main" info-file containing info on which exact value needs to be extracted.
     Its name is given in the first line of meteodataextraceannual.txt.
     Information are as in the example below:
     - structure of the folder system
     - database (as trunc of file names)
     - 'window' to examine. A window is a n-dimensional cut-out of the whole dataset, 
       defined by range of the dimensions. 
        In the present case, the dimensions are the following:

        1. Dimension: NCUs: 
        2. Dimension: Years
        3. Dimension: Days
        4. Dimension: Parameters
This
        Attention!! For each information you must give start and end value, separated by a comma
   The program will cut out the n-dimensional array

------example file: marsateamextraceannualncushort.txt start------
marsateam/NCU
output
output2/marsateam_ncu
ncumarsateam.ncus.txt
1901
10
1,40991
1991,2000
1,365
1,5


Line 1: input folder
Line 2: output folder (used in the example program)
Line 3: trunc of file name for climate data 
Line 4: file name assigning NCU-index to NCU-number
Line 5: first year considered in the climate data set
Line 6: number of years stored in one set of files
Line 7: NCUs (all between first and second number inclusive - max possible 40991)
Line 8: years (all between first and second number incl. - max possible 2000)
Line 9: days (Julian days, all between first and second number incl.)
Line 10:  parameters (all between parameter #1 (Tmin) and #5 (Rad) incl.
------example file: marsateamextraceannualncushort.txt start------

  3) In addition, a file is required that links NCU-numbers (from 1 to 4091) with the NCU-IDs.
     In some cases, only the NCU-ID will be known and this enables the program to find their numbers.


History of this file:
- first version april 2009
- second version june 2009, with extension to global-radiation files
- third version july 2010, extension to cover also remo-data and to short-integers transformed marsateam data for NCUs

*/

int getNCU_ID(int,char[100]);
int getNCU(int,char[100]);
int getMonth(int);

int main()
{
	//! clim_map1 is map file for the binary file clim_bin1. clim_bin1 is the binary data file.  clim_map and clim_bin2 are not used for REMO data
    FILE *clim_map1, *clim_map2, *clim_bin1, *clim_bin2;
    //! these files are used for storing the annual, monthly, and daily data respectively. The final file is used to store data that is considered in error
    FILE *clim_writeannual,*clim_writemonth,*clim_writeday,*clim_writenodata;
    //! these are used to store the names of the map and binary files
    char Cclim_map1[200], Cclim_map2[200], Cclim_bin1[200], Cclim_bin2[200];
    //! these are used to store the names of the output files
    char Cclim_writeannual[200],Cclim_writemonth[200],Cclim_writeday[200],Cclim_writenodata[200];
    //! the first two are used in the construction of the data filenames. The last two are the directories for input and output
    char Cclim_name1[200], Cclim_name2[200], clim_inpath[200], clim_outpath[200];
    //! Cncu is used to store the name of the file that contains data linking NCUs and NCUIs.
    char Cncu[100],char100[100], *buffer;
    //! clim_type stores information on climate file type (we are only using REMO).
    string clim_type,dim_name[5],par_name[10],par_unit[10],filesuffix;
    
    int hsmu,hsmuID,rhsmuID;
    int yearstart, yearsfile;
    int read_start[maxpars], read_end[maxpars];
    int dim_beg[maxpars],dim_stp[maxpars],dim_end[maxpars];
    int idummy1,idummy2,writing,listing;
    float fdummy; 
    //! this contains the values by which the binary data should be multiplied in order to convert it into scientific units
    float par_conv[maxpars]={1};
    float par_min[maxpars]={0};
    
    

    long int posmap1,posmap2,posbin1,posbin2,posclim,ldummy1;

    char line[50000];
    //! these two arrays contain the minimum and maximum values permissible for the variables
    float par_max[5];
    par_min[0]=230;
    par_max[0]=330;
    par_min[1]=230;
    par_max[1]=330;
    par_min[2]=0;
    par_max[2]=300;
    par_min[3]=0;
    par_max[3]=100;
    par_min[4]=0;
    par_max[4]=100;


    
    //First determine the input file to read
    //      and the type of data
    sprintf(Cclim_map1,"meteodataextraceannual.txt");
    clim_map1=fopen(Cclim_map1,"r");
    if (clim_map1==NULL){cout << "Cannot open clim_map1 " << Cclim_map1 <<"\n"; exit(0);}
    fscanf(clim_map1,"%s",Cclim_map1);
    fscanf(clim_map1,"%s",line);
    fscanf(clim_map1,"%d",&writing);
    fscanf(clim_map1,"%d",&listing);
    fclose(clim_map1);
    clim_type=line;
    //Read information from input-text file
    clim_map1=fopen(Cclim_map1,"r"); 
    if (clim_map1==NULL){cout << "Cannot open clim_map1 " << Cclim_map1 <<"\n"; exit(0);}
    else {if(listing>0){cout<<Cclim_map1<<" could be opened!\n";}}
    fscanf(clim_map1,"%s",clim_inpath);
    fscanf(clim_map1,"%s",clim_outpath);
    fscanf(clim_map1,"%s",Cclim_name1);
    if(clim_type=="ncu"){fscanf(clim_map1,"%s",Cclim_name2);}
    fscanf(clim_map1,"%s",Cncu);
    fscanf(clim_map1,"%d",&yearstart);
    fscanf(clim_map1,"%d",&yearsfile);

    fscanf(clim_map1,"%d,%d",&read_start[0],&read_end[0]);
    fscanf(clim_map1,"%d,%d",&read_start[1],&read_end[1]);
    fscanf(clim_map1,"%d,%d",&read_start[2],&read_end[2]);
    fscanf(clim_map1,"%d,%d",&read_start[3],&read_end[3]);
    fclose(clim_map1);
    if ((clim_type=="remo")&&(read_end[3]==5))
    {
    	cout << "Maximum of 4 variables in REMO files" << endl;
    	read_end[3]=4;
    }
    //! Assign names to the parameters
    char pars[maxpars][5];
    if (clim_type=="remo")
    {
    sprintf(pars[0],"tmn");
    sprintf(pars[1],"tmx");
    sprintf(pars[2],"pre");
    sprintf(pars[3],"rad");
    sprintf(pars[4],"none");
    }
    else
    {
        sprintf(pars[0],"tmn");//! minimum temperature
        sprintf(pars[1],"tmx");//! maximum temperature
        sprintf(pars[2],"pre");//! precipitation
        sprintf(pars[3],"vap");//! water vapour pressure
        sprintf(pars[4],"rad");//! solar radiation
    }
    if(listing>0){
    cout<<"clim_map1: "<<Cclim_map1<<"\n";
    cout<<"clim_inpath: "<<clim_inpath<<"\n";
    cout<<"clim_outpath: "<<clim_outpath<<"\n";
    cout<<"Cclim_name1: "<<Cclim_name1<<"\n";
    if(clim_type=="ncu"){cout<<"Cclim_name2: "<<Cclim_name2<<"\n";}
    cout<<"ncu: "<<Cncu<<"\n";
    cout<<"year_range_start: "<<yearstart<<"\n";
    cout<<"year_range_end: "<<yearsfile<<"\n";}

    //parameters count from 0 to maxpars-1
    int i=0;
    read_start[3]=read_start[3]-1;
    read_end[3]=read_end[3]-1;
    if(listing>0){for(i=0;i<=read_end[3];i++){cout<<"read_start["<<i<<"]: "<<read_start[i]<<", read_end["<<i<<"]: "<<read_end[i]<<"\n";}}
	
    int startbyte,numdim,dim1_gap,startpix,nnendpix;
    int ncomment,numyears,numdays,firstncunut,stepncunut,nncunut;

    long int posstart=0;
    if(clim_type=="ncu"){posstart=1000;}
    if(clim_type=="hsmu"){posstart=48;}
    if(listing>0){cout<<"clim_type="<<clim_type<<endl;cout<<"posstart="<<posstart<<endl;}

    //! the name of the map file is constructed from the name of the input directory, a root file name and the period of the data in years
    //open map ... all maps are equally constructed, so it does not matter which one ...
    if(clim_type=="ncu"){sprintf(Cclim_map1,"%s/%s.%d-%d.map",clim_inpath,Cclim_name1,yearstart,yearstart+yearsfile-1);}
    if(clim_type=="hsmu"){sprintf(Cclim_map1,"%s/%s.map",clim_inpath,Cclim_name1);}
    if(clim_type=="ncushort"){sprintf(Cclim_map1,"%s/%s.map",clim_inpath,Cclim_name1);}
    if(clim_type=="remo"){sprintf(Cclim_map1,"%s/%s.%d-%d.map",clim_inpath,Cclim_name1,yearstart,yearstart+yearsfile-1);}
    //if(clim_type=="remo"){sprintf(Cclim_map1,"%s/%s2021.map",clim_inpath,Cclim_name1);}

    
    clim_map1=fopen(Cclim_map1,"rb"); 
    if (clim_map1==NULL){cout << "Cannot open " << Cclim_map1 <<"\n"; exit(0);}
    else{if(listing>0)cout<<Cclim_map1<<" could be opened!" << endl;}
    //! From the map file, read the format information for the binary file
    if(clim_type=="ncu" || clim_type=="hsmu"){
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
    }else if(clim_type=="remo" || clim_type=="ncushort"){
        fread(&posstart,8,1,clim_map1);
        fread(&numdim,4,1,clim_map1);
        
        //if(clim_type=="remo") posstart=1268;
        //if(clim_type=="remo") numdim++;  

        if(listing>0)cout<<"startbyte="<<posstart<<endl;
        if(listing>0)cout<<"numdim="<<numdim<<endl;
        for (int i=0; i<numdim-1; i++){
          fread(char100,1,100,clim_map1);
          dim_name[i]=char100;
          fread(&dim_beg[i],4,1,clim_map1); 
          fread(&dim_end[i],4,1,clim_map1);
          fread(&dim_stp[i],4,1,clim_map1);
          if(listing>0)cout<<"dim_name["<<i<<"]="<<dim_name[i]<<endl;
          if(listing>0)cout<<"dim_beg["<<i<<"]="<<dim_beg[i]<<endl;
          if(listing>0)cout<<"dim_end["<<i<<"]="<<dim_end[i]<<endl;
          if(listing>0)cout<<"dim_stp["<<i<<"]="<<dim_stp[i]<<endl;
        }
        //Only need name/number of parameter-dimension
        int ii=numdim-1;
        fread(char100,1,100,clim_map1);
        dim_name[ii]=char100;
        fread(&dim_end[ii],4,1,clim_map1); 
        if(listing>0)cout<<"dim_name["<<ii<<"]="<<dim_name[ii]<<endl;
        if(listing>0)cout<<"dim_end["<<ii<<"]="<<dim_end[ii]<<endl;

        //! from the map file, read the name, unit and conversion factor for the meteorological variables
        for (int i=0; i<dim_end[3]; i++){
          fread(char100,1,100,clim_map1);
          par_name[i]=char100;
          if(listing>0)cout<<"par_name["<<i<<"]="<<par_name[i]<<endl;
          fread(char100,1,100,clim_map1);
          par_unit[i]=char100;
          if(listing>0)cout<<"par_unit["<<i<<"]="<<par_unit[i]<<endl;
          fread(&par_conv[i],4,1,clim_map1);
          if(listing>0)cout<<"par_conv["<<i<<"]="<<par_conv[i]<<endl;
        }
        // only for testing!!
        int teststart=4490;
        int testnumbe=1;
        posmap1=posstart+36*(teststart-1);
        if(listing>0)cout<<"posstar="<<posstart<<endl;
        if(listing>0)cout<<"posmap1="<<posmap1<<endl;
        fseek(clim_map1,posmap1,SEEK_SET);
        for (int i=teststart; i<=teststart+testnumbe; i++){
          fread(&posmap1,8,1,clim_map1);
          if(listing>0)cout<<"spatial unit: posmap1="<<posmap1<<endl;        
          fread(&posmap2,8,1,clim_map1);
          if(listing>0)cout<<"map-fiel position: posmap2="<<posmap2<<endl;        
          fread(char100,1,20,clim_map1);
          filesuffix=char100;
          if(listing>0)cout<<"filesuffix="<<filesuffix<<endl;
        }
    }
    fclose(clim_map1);

    //Open files to write daily/monthly/annual output
    numdays=(dim_end[2]-dim_beg[2]+1)/dim_stp[2];
    numyears=(dim_end[1]-dim_beg[1]+1)/dim_stp[1];


    sprintf(Cclim_writeannual,"%s/%s%d-%d.%d-%d.%d-%d.%d-%d_yr.csv",clim_outpath,clim_type.c_str(),read_start[0],read_end[0],read_start[1],read_end[1],read_start[2],read_end[2],read_start[3],read_end[3]);
    clim_writeannual=fopen(Cclim_writeannual,"w"); 
    if (clim_writeannual==NULL){cout << "Cannot open clim_writeannual " << Cclim_writeannual <<"\n"; exit(0);}
    if(writing>0)fprintf(clim_writeannual,"hsmu,hsmuID,year");
    for (int para=read_start[3];para<=read_end[3];para++)
    {
        if(writing>0)fprintf(clim_writeannual,",%s",pars[para]);
    }    

    // read_start[2] and read_start[2] give days ... convert to months 
    sprintf(Cclim_writemonth,"%s/%s%d-%d.%d-%d.%d-%d.%d-%d_mn.csv",clim_outpath,clim_type.c_str(),read_start[0],read_end[0],read_start[1],read_end[1],getMonth(read_start[2])+1,getMonth(read_end[2])+1,read_start[3],read_end[3]);
    clim_writemonth=fopen(Cclim_writemonth,"w");  //rb
    if (clim_writemonth==NULL){cout << "Cannot open clim_writemonth " << Cclim_writemonth <<"\n"; exit(0);}
    if(writing>1)fprintf(clim_writemonth,"hsmu,hsmuID,year,mnth");
    for (int para=read_start[3];para<=read_end[3];para++)
    {
        if(writing>1)fprintf(clim_writemonth,",%s",pars[para]);
    }    

    sprintf(Cclim_writeday,"%s/%s%d-%d.%d-%d.%d-%d.%d-%d_dy.csv",clim_outpath,clim_type.c_str(),read_start[0],read_end[0],read_start[1],read_end[1],read_start[2],read_end[2],read_start[3],read_end[3]);
    clim_writeday=fopen(Cclim_writeday,"w");  //rb
    if (clim_writeday==NULL){cout << "Cannot open clim_writeday " << Cclim_writeday <<"\n"; exit(0);}
    if(writing>2)fprintf(clim_writeday,"hsmu,hsmuID,year,jday");
    for (int para=read_start[3];para<=read_end[3];para++)
    {
        if(writing>2)fprintf(clim_writeday,",%s",pars[para]);
    }    

    sprintf(Cclim_writenodata,"%s/%s_%d-%d.%d-%d.nodata.txt",clim_outpath,clim_type.c_str(),read_start[0],read_end[0],read_start[1],read_end[1]);
    clim_writenodata=fopen(Cclim_writenodata,"w");  //rb
    if (clim_writenodata==NULL){cout << "Cannot open clim_writenodata " << Cclim_writenodata <<"\n"; exit(0);}
    if(listing>0) cout<<Cclim_writeannual<<endl<<Cclim_writemonth<<endl;    


    // START OF THE LOOP:
    // -- first the year (to determine the time-slice=="database"
    // -- then the ranking of dimensions: spatial unit/day/parameter
    for (int year=read_start[1];year<=read_end[1];year+=dim_stp[1])
    {
        //First determine the time-slice
        int filey1=yearstart;
        int filey2=filey1+yearsfile-1;
        while (filey1+yearsfile <= year) 
        { 
            filey1+=yearsfile ; 
            filey2=filey1+yearsfile-1; 
        }
    
    
        //The map-files must be opened for each year, as the file-name could be
        //integrated.
        if(clim_type=="ncu" || clim_type=="ncushort"){sprintf(Cclim_map1,"%s/%s.%d-%d.map",clim_inpath,Cclim_name1,yearstart,yearstart+yearsfile-1);}
        if(clim_type=="hsmu"){sprintf(Cclim_map1,"%s/%s.map",clim_inpath,Cclim_name1);}
        //if(clim_type=="remo"){sprintf(Cclim_map1,"%s/%s2001.map",clim_inpath,Cclim_name1);}
        if(clim_type=="remo" || clim_type=="ncushort"){sprintf(Cclim_map1,"%s/%s.%d-%d.map",clim_inpath,Cclim_name1,filey1,filey2);}

        clim_map1=fopen(Cclim_map1,"rb");  
        if (clim_map1==NULL){cout << "Cannot open " << Cclim_map1 <<"\n"; exit(0);}
        else{if(listing>0)cout<<Cclim_map1<<" could be opened!\n";}
        
        if(clim_type=="ncu"){
        sprintf(Cclim_map2,"%s/%s.%d-%d.map",clim_inpath,Cclim_name1,yearstart,yearstart+yearsfile-1);
        clim_map2=fopen(Cclim_map2,"rb");  
        if (clim_map2==NULL && read_end[3]==5){cout << "Cannot open " << Cclim_map2 <<"\n"; exit(0);}
        else{if(listing>0)cout<<Cclim_map2<<" could be opened!\n";}
        }
    
        
        for (int ncu=read_start[0];ncu<=read_end[0];ncu+=dim_stp[0])
        {
            //If NCU-ID is given, convert to NCU-number, otherwise get the ID
            //The below lines will not be triggered for HSMUs where no "ID" is used.
            int hsmuID=ncu;
            int hsmu=ncu;
            if(clim_type=="ncu"){
            if (hsmu>dim_end[0]) {hsmu=getNCU(hsmu,Cncu);}
            if (hsmuID==hsmu) {hsmuID=getNCU_ID(hsmu,Cncu);}}

            //Check position of first bit of the NCU in the climate-map.
            //Attention: a few NCUs are missing (small islands, not included in the NEU-data, so they should not occur)
            //           however, the program will not give any numbers for these.
            if(clim_type=="ncu" || clim_type=="hsmu") posmap1=posstart+12*(hsmu-1);
            if(clim_type=="ncushort" || clim_type=="remo") posmap1=posstart+36*(hsmu-1);
            fseek(clim_map1,posmap1,SEEK_SET);
            //Read Start-Bit for the NCU and calculated bit for the actual value
            if(clim_type=="ncu" || clim_type=="hsmu")
            {
            	idummy1=0;
            	fread(&idummy1,4,1,clim_map1);
            }
            if(clim_type=="ncushort" || clim_type=="remo")
            	{
            	idummy1=0;
            	fread(&ldummy1,8,1,clim_map1);
            	}
            fread(&posbin1,8,1,clim_map1);
            if(clim_type=="ncu"){
            	idummy1=0;
                fseek(clim_map2,posmap1,SEEK_SET);
                fread(&idummy1,4,1,clim_map2);
                fread(&posbin2,8,1,clim_map2);}
            if(listing>1){
                cout<<"mapfile="<<Cclim_map1;
                cout<<",position="<<posmap1;
                cout<<",unit="<<hsmu;
                if(clim_type=="ncu" || clim_type=="hsmu") cout<<",unit="<<idummy1;
                if(clim_type=="ncushort" || clim_type=="remo") cout<<",unit="<<ldummy1;
                cout<<",mapposition="<<posbin1;
                cout<<endl;}
            if(clim_type=="ncushort" || clim_type=="remo") {
                fread(char100,1,20,clim_map1);
                filesuffix=char100;
                if(listing>1)cout<<"filesuffix="<<filesuffix<<endl;}
            
            if((idummy1!=hsmu && ldummy1!=hsmu) || hsmuID==999 || posbin1==3 )
            {
                if(clim_type=="ncu" || clim_type=="hsmu")cout<<"Attention: wrong NCU in climate-map. NCU="<<hsmu<<" and read: "<<idummy1<<"\n";
                if(clim_type=="ncushort" || clim_type=="remo")cout<<"Attention: wrong NCU in climate-map. NCU="<<hsmu<<" and read: "<<ldummy1<<"\n";
                fprintf(clim_writenodata,"\n%d,%d,%d --> no data available",hsmu,hsmuID,year);
            }
            else
            {
                if(listing==0)cout<<","<<hsmu;
                char zeroa[10]="", zeroe[10]="", tmpa[10]="",tmpe[10]="";
                if(clim_type=="ncu" || clim_type=="hsmu"){
                  //Open Data-File!
                  //File names have trailing zeros ... must be included here
                  //Thiis only required for datatype 'hsmu' and 'ncu'
                  //'ncushort' and 'remo' have the ncu-range integrated,
                  //thus for each ncu the file containing the data is
                  //defined with input information (trunc) and info in
                  //header
                  startpix=(hsmu-1)/dim1_gap;
                  startpix=startpix*dim1_gap+1;
                  nnendpix=startpix+dim1_gap-1;
                  if(startpix>=10000){sprintf(tmpa,"");};
                  if(startpix<10000){sprintf(tmpa,"0");};
                  if(startpix<1000){sprintf(tmpa,"00");};
                  if(startpix<100){sprintf(tmpa,"000");};
                  if(startpix<10){sprintf(tmpa,"0000");};
                  if(startpix<100000 && clim_type=="hsmu"){sprintf(zeroa,"0%s",tmpa);};
                  if(nnendpix>=10000){sprintf(tmpe,"");};
                  if(nnendpix<10000){sprintf(tmpe,"0");};
                  if(nnendpix<1000){sprintf(tmpe,"00");};
                  if(nnendpix<100){sprintf(tmpe,"000");};
                  if(nnendpix<10){sprintf(tmpe,"0000");};
                  if(nnendpix<100000 && clim_type=="hsmu"){sprintf(zeroe,"0%s",tmpe);};
                }
                if(clim_type=="ncu"){
                  sprintf(Cclim_bin1,"%s/%s.%d-%d.%s%d_%s%d.bin",clim_inpath,Cclim_name1,filey1,filey2,zeroa,startpix,zeroe,nnendpix);
                  clim_bin1=fopen(Cclim_bin1,"rb"); 
                  if (clim_bin1==NULL){cout << "Cannot open 11 " << Cclim_bin1 <<"\n"; exit(0);}
                  else{if(listing>1)cout<<Cclim_bin1<<" could be opened!\n";}
                  sprintf(Cclim_bin2,"%s/%s.%d-%d.%s%d_%s%d.bin",clim_inpath,Cclim_name2,filey1,filey2,zeroa,startpix,zeroe,nnendpix);
                  clim_bin2=fopen(Cclim_bin2,"rb");
                  if (clim_bin2==NULL && read_end[3]==4){cout << "Cannot open 12" << Cclim_bin2 <<"\n"; exit(0);}}
                else if(clim_type=="ncushort"){
                  sprintf(Cclim_bin1,"%s/%s.%d-%d.%s.bin",clim_inpath,Cclim_name1,filey1,filey2,filesuffix.c_str());
                  clim_bin1=fopen(Cclim_bin1,"rb"); 
                  if (clim_bin1==NULL){cout << "Cannot open 11 " << Cclim_bin1 <<"\n"; exit(0);}
                  else{if(listing>1)cout<<Cclim_bin1<<" could be opened!\n";}}
                else if(clim_type=="remo"){
                  sprintf(Cclim_bin1,"%s/%s.%d-%d.%s.bin",clim_inpath,Cclim_name1,filey1,filey2,filesuffix.c_str());
                  clim_bin1=fopen(Cclim_bin1,"rb"); 
                  if (clim_bin1==NULL){cout << "Cannot open 11 " << Cclim_bin1 <<"\n"; exit(0);}
                  else{if(listing>1)cout<<Cclim_bin1<<" could be opened!\n";}}
                else if(clim_type=="hsmu"){
                  sprintf(Cclim_bin1,"%s/%s.%s%d_%s%d.bin",clim_inpath,Cclim_name1,zeroa,startpix,zeroe,nnendpix);
                  clim_bin1=fopen(Cclim_bin1,"rb"); 
                  if (clim_bin1==NULL){cout << "Cannot open 11 " << Cclim_bin1 <<"\n"; exit(0);}
                  else{if(listing>1)cout<<Cclim_bin1<<" could be opened!\n";}}
                
            
                // Needed parameter to for annual data
                float annpar[5]={0};
                float monpar[5][12]={0};
                int monday[12]={31,28,31,30,31,30,31,31,30,31,30,31};
                int curmonth=0,curday=0;
                int databytes;
                if(clim_type=="ncu" || clim_type=="hsmu") databytes=4;
                if(clim_type=="ncushort" || clim_type=="remo") databytes=2;
                if(writing>0)fprintf(clim_writeannual,"\n%d,%d,%d",hsmu,hsmuID,year);
                for (int day=read_start[2];day<=read_end[2];day+=dim_stp[2])
                {
                    //commented annual data
                    if(writing>2)fprintf(clim_writeday,"\n%d,%d,%d,%d",hsmu,hsmuID,year,day);
                    //!
                    for (int para=read_start[3];para<=read_end[3];para++)
                    {
                        posclim=posbin1+
                                databytes*(
                                +(year-filey1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]*dim_end[3]            // full years
                                +(day-dim_beg[2])/dim_stp[2]*dim_end[3]                                              // full days
                                +para);

                        if(listing>2)cout<<endl<<endl<<"posclim="<<posclim;
                        
                        //NCU files are split into two series of binary files,
                        //the second one containing only radiation
                        if(para==4 && clim_type=="ncu")
                        {
                            posclim=4*(
                            
                                    +(hsmu-startpix)*(filey2-filey1+1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]  // full ncu's
                                    +(year-filey1)/dim_stp[1]*(dim_end[2]-dim_beg[2]+1)/dim_stp[2]
                                    +(day-dim_beg[2])/dim_stp[2]
                                    
                                    );
                            fseek(clim_bin2,posclim,SEEK_SET);
                            fread(&fdummy,4,1,clim_bin2);
                        }else{
                          
                          if(clim_type=="ncu" || clim_type=="hsmu"){                 
                            fseek(clim_bin1,posclim,SEEK_SET);
                            fread(&fdummy,4,1,clim_bin1);}
                          else if (clim_type=="ncushort" || clim_type=="remo"){
                        	  //! initialise to zero the integer into which the data will be read. The integer is 3 or more bytes long and the fread statement will only write into the lower 2 bytes
                        	  idummy1=0;
                            fseek(clim_bin1,posclim,SEEK_SET);
                            fread(&idummy1,2,1,clim_bin1);
                            float aFloat =float(idummy1);
                            fdummy=aFloat*par_conv[para];};//! multiply the value by the conversion factor to obtain the data in scientific units
                        }
                        if(listing>2)cout<<endl<<hsmu<<","<<year<<","<<day<<","<<para<<","<<posbin1<<","<<posclim;
                        if(listing>2)cout<<","<<filey1<<","<<filey2; 
                        if(listing>2)cout<<","<<hsmu<<","<<startpix << endl;

                        if(listing>2)
                        	cout<<endl<<"idummy1="<<idummy1;
                        if(listing>2)
                        	cout<<endl<<"fdummy="<<fdummy;
                        //added annual data for precipitation sum for
                        //other parameters average
                        if(para==2){annpar[para]+=fdummy;}else{annpar[para]+=fdummy/365.;}
                        if(para==2){monpar[para][curmonth]+=fdummy;}else{monpar[para][curmonth]+=fdummy/monday[curmonth];}

                        if(writing>2)fprintf(clim_writeday,",%f",fdummy);

                        if(fdummy<par_min[para]) fprintf(clim_writenodata,"\n%d,%d,%d,%d --> Value small for %s (<%f): %f ",hsmu,hsmuID,year,day,pars[para],par_min[para],fdummy);
                        if(fdummy>par_max[para]) fprintf(clim_writenodata,"\n%d,%d,%d,%d --> Value large for %s (>%f): %f ",hsmu,hsmuID,year,day,pars[para],par_max[para],fdummy);

                        //just to have a control on the command-line,
                        //might be deleted
                        if(hsmu==5440)
                        {
                            cout<<"\n"
                            <<year<<"-"<<day<<"-"<<hsmu<<"-"
                            <<para<<"-"
                            <<posbin1<<"-"<<posclim<<" "
                            <<fdummy
                            ;
                        }
                    }
                    curday++;
                    //Next day will be next month: write-out month and
                    //switch month
                    if(curday+1>monday[curmonth]){
                        if(writing>1)fprintf(clim_writemonth,"\n%d,%d,%d,%d",hsmu,hsmuID,year,curmonth+1);
                        for (int para=read_start[3];para<=read_end[3];para++)
                        {
                            if(writing>1)fprintf(clim_writemonth,",%f",monpar[para][curmonth]);
                        }    
                        curday=0;curmonth++;
                    }
                }
                fclose(clim_bin1);
                if(clim_type=="ncu") fclose(clim_bin2);				
                //added annual data
                for (int para=read_start[3];para<=read_end[3];para++)
                {
                    if(writing>0)fprintf(clim_writeannual,",%f",annpar[para]);
                }
            }
        }
    fclose(clim_map1);
    }
 }   

int getNCU_ID(int hsmu,char Cncu[100])
{
    FILE *ncu;
    //char Cncu[100];
    ncu=fopen(Cncu, "r");
    int hsmuID=999, idummy1, idummy2;
    if (ncu==NULL){cout << "Cannot open 13" << Cncu <<"\n"; exit(0);}
    //else {cout << "could open " << Cncu <<"\n";}
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
    if (ncu==NULL){cout << "Cannot open 14" << Cncu <<"\n"; exit(0);}
    while ( fscanf(ncu, "%d %d", &idummy1,&idummy2) == 2 && hsmu==99999 )
    {
        if(idummy1==hsmuID){hsmu=idummy2;cout<<hsmu;}
        //cout<<idummy1<<","<<hsmuID<<"\n";
    }
    fclose(ncu);
    return hsmu;
}
int getMonth(int day)
{
    int monday[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int accday=0;
    int ii;

    for(ii=0; ii<12;ii++)
    {
        accday+=monday[ii];
        if(day<=accday) break;
    }
    return ii;

}
