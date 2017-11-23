/*! \mainpage Field operation timelines

\section intro_sec Initialisation

Model is controlled using an instance of the modelControlClass class. The initialisation begins with 
the modelControlClass::Initialise function. This begins with the creation of the results file and the 
creation of two instances of the class bsTime. The latter are initialised to the start and end date for 
the simulations. If detailed output of the model variables is desired (outputDetail is true), the file for 
detailed output is created. An instance of the class message is created to handle errors trapped by 
the programme. The instance is initialised with the name of the file to which errors are logged 
(”details.txt”).

An instance of the class trafworkCharClass is created to hold the data on trafficability and 
workability for all soil types. This is initialised with the name of the file holding the data 
("trafwork.txt"). An instance of the class locationClass is created. This contains and manages the 
modelling for a single location (NCU). It is initialised with the name of the file containing the 
meteorological data, a pointer to the output file, the identity number of the soil type at the location 
and a pointer to the instance of trafworkCharClass.

Within the function locationClass::Initialise, the meteorological data file is opened and initialised 
using the climateClass class. The current directory is then searched for files containing the crop 
management plans (crops planted and the associated field operations) for one or more fields. These 
are called fieldn.txt, where n=0..N. Note that if the files are not in the current directory, searching 
continues up through the hierarchy of directories. The file are read using the base class so if no such 
files are found, the model reports an error and then terminates. The soil type to be used for each 
field is also read these files, via a soil type identity integer (theSoilType). The initialise function of 
the field class is then called.

The field::Initialise function creates a linked list of pointers to the fieldOp class to hold the list of 
field operations. The trafficability and workability parameters passed in a pointer to the 
trafworkDataClass class is then copied. The pointer to the output file and the field number are 
assigned. The initialise function of the ecosystem class is then called (see below). The field 
operations are then loaded using the loadOps function and then sorted in order of starting date, 
using the sortOps function. Note that the generalised field operation class (fieldOp) is specialised 
for tillage (fieldOpTill), sowing (fieldOpSow), fertilisation (fieldOpFert), the start of grazing 
(fieldOpGrass) and irrigation (fieldOpIrr).

\section sim_sec Simulations

The simulations are hosted by the modelControlClass::Simulate function. The purpose of this 
function is to call the location::dailyRoutine function on a daily basis until the end point of the 
simulations is reached. The location::dailyRoutine itself manages the calculation of the temperature 
sum but otherwise just calls the field::dailyRoutine function, passing as parameters the relevant 
meteorological variables 

The field::dailyRoutine cycles through the list of field operations, using the doOp function to 
determine whether the next field operation can be performed. In doing so, it first calls the doOpNow 
function that returns an integer to a variable Optest, where 1 indicates that the temperature sum has 
been achieved on the first day of the operation can be performed, 2 indicates that the temperature 
sum has been achieved at some date after the start date but before the end date, and 3 indicates that 
the end date has been reached before the temperature sum has been achieved. The function then 
loads trafficability and workability limits according to crop type (grass or other), adjusts the limits 
to the appropriate soil depth (50 millimetres for trafficability and the tillage depth for workability) 
and then checks whether the soil moisture deficit is sufficient to permit the operation to be 
performed. If Optest is 1 or 2, the fieldOp::outputOps function is called, using Optest to supply the 
status integer. The fieldOp::outputOps outputs the field number, Julian Day, field operation 
identifier, status integer and additional informative data to the output file. If Optest is 3, 
fieldOp::outputOps is also called but with the status integer sets to 3 if the operation could not be 
completed before the end date because the temperature sum was not achieved or to 4 if the soil 
moisture limitation prevented the operation being performed before the end date. If an operation can 
be performed, the doOp function returns a pointer to that operation. The name of the operation is 
used to determine which of the ecosystem class functions should be called. In the case of field 
operations starting or stopping grazing or irrigation, the operation is used to toggle the variables 
graze and irrigate respectively. If these variables are set to true, the ecosystem functions 
implementing grazing or irrigation are performed on a daily basis.

\section term_sec Termination

Where appropriate, classes contain a function called closeDown which closes any open files and 
tidied up memory.

The internal ecosystem model

The ecosystem model consists of a tipping-bucket type soil moisture model, with the addition of a 
crude simulation of capillary flow, and a temperature-sum type crop growth model. For reasons of 
expedience, the modelling of all water flows is handled by the soil model. This includes the 
dynamics of water in pools that are not physically part of the soil (snow on the soil surface and in 
canopy storage).

Initialisation

The ecosystem::Initialise function calls the equivalent functions for the crop and soil models (see 
below). The parameters for all soil types are loaded into theSoilChars class and the soil model is 
initialised with the instance of the soilDataClass class appropriate for the soil type. The parameters 
for all crops are loaded into the linked list theCropChars.

The initialisation of the crop model via simplePlant::Initialise sets all variables to their default 
values (often zero). The simpleSoil::Initialise function initialised if this were properties using the 
data in a pointer to the instance of the soilDataClass class. The budgetary variables used in 
maintaining the water budget (waterIn and waterOut) are initialised. The physical properties of the 
soil are stored in the linked list theLayers, which contains a pointer to one instance of the layerClass 
class for each layer in the soil. Note that these instances only contain the soil parameters; the 
variables relating to soil water are managed as pools. The pools are theSnow (contains snow), 
canopyInterception (contains rain or meltwater intercepted by the crop canopy), theEvaporation 
(describes that part of the rooting pool from which water can evaporate), theRooting (contains the 
water down to the rooting depth that can be lost by evaporation or transpiration) and the 
theSubZone pool that contains the remaining water in the soil. All pools are described using 
instances of the pool class. All pools are initialised here. Note that the pool theEvaporation has a 
fixed capacity of 10.0 millimetres and that the pool theRooting always has an equivalent capacity.

Simulations

The sole role of the ecosystem::dailyRoutine function is to call the respective daily functions of the 
crop and soil models. The simpleSoil::dailyRoutine function first calculates the accumulation, 
evaporation, melting and drainage from the snow pool. Drainage from the soil pool is assumed to 
pass to the canopy interception pool. The remaining evapotranspirative potential is then partitioned 
using a Beer’s Law function of crop LAI between that which will be available for soil evaporation 
and that which will be intercepted by the crop canopy. The soil evaporation is then calculated.  Any 
potential evaporative demand of the soil that cannot be satisfied by evaporation is assumed to be 
lost. Note that the evaporation pool is a part of the rooting pool, so the soil evaporation is also 
deducted from the rooting pool.

The evapotranspirative potential partitioned to the crop is first allocated to canopy. The 
accumulation, evaporation and drainage from the canopy interception pool are calculated, deducting 
any evaporation from the evapotranspirative potential that can subsequently be used for 
transpiration. The capacity of the canopy interception pool depends on the LAI of the crop. The 
transpiration is then calculated, using a breakpoint approach to introduce a limitation due to the lack 
of availability of water in the rooting zone. The water in the rooting pool is then adjusted to account 
for the input via drainage from the canopy interception pool and capillary flow from the sub-zone 
pool and the loss via transpiration and drainage to the sub-zone pool. The water in the sub-zone 
pool is adjusted to account for the input as drainage from the rooting pool and the loss via capillary 
flow and drainage out of the soil.

The simplePlant::dailyRoutine simulates the dynamics of root depth and the development of green 
and dead LAI. The simulation of LAI depends on whether the crop is grass or not. If it is grass, 
once the crop has emerged it continues to produce green LAI ad infinitum (subject to a maximum 
value) and no dead LAI is produced. If any other crop is simulated, the process of crop maturation 
is simulated, so that the crop matures and eventually all the LAI is dead. Note that separate 
procedures are used to simulate the non-grass crops in the autumn.

Termination

Where appropriate, classes contain a function called closeDown which closes any open files and 
tidied up memory.

*/

