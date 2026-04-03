///////////////////////////////////////////////////////////////////////////////////////
/// \file parameters.h
/// \brief The parameters module is responsible for reading in the instruction file
///
/// This module defines and makes available a lot of the instruction file parameters
/// used by the model, but also lets other modules define their own parameters or
/// access "custom" parameters without defining them.
///
/// A new parameter can be added by creating a new global variable here (or a new
/// Pft member variable if it's a PFT parameter), and then declaring it in
/// plib_declarations in parameters.cpp. See the many existing examples, and
/// documentation in the PLIB library for further documentation about this.
///
/// Sometimes, adding a new parameter shouldn't (or can't) be done here however.
/// A parameter specific for a certain input module, should only be declared if
/// that input module is used. In this case the input module should declare its
/// own parameters when it is created. This can also be a good idea simply to
/// make modules more independent. For parameters like this, we can either use
/// the "custom" parameters (\see Paramlist) which don't need to be declared at
/// all, or the parameters can be declared with the declare_parameter family of
/// functions.
///
/// \author Joe Siltberg
///
///////////////////////////////////////////////////////////////////////////////////////

#ifndef LPJ_GUESS_PARAMETERS_H
#define LPJ_GUESS_PARAMETERS_H

#include "gutil.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////////////
// Enums needed by some of the global instruction file parameters defined below


/// Vegetation 'mode', i.e. what each Individual object represents
/** Can be one of:
 *  1. The average characteristics of all individuals comprising a PFT
 *     population over the modelled area (standard LPJ mode)
 *  2. A cohort of individuals of a PFT that are roughly the same age
 *  3. An individual plant
 */
typedef enum {NOVEGMODE, INDIVIDUAL, COHORT, POPULATION} vegmodetype;

/// Land cover type of a stand. NLANDCOVERTYPES keeps count of number of items.
/*  NB. set_lc_change_array() must be modified when adding new land cover types
 */
typedef enum {URBAN, CROPLAND, PASTURE, FOREST, NATURAL, PEATLAND, BARREN, NLANDCOVERTYPES} landcovertype;

/// Water uptake parameterisations
/** \see water_uptake in canexch.cpp
  */
typedef enum {WR_WCONT, WR_ROOTDIST, WR_SMART, WR_SPECIESSPECIFIC} wateruptaketype;

///bvoc: define monoterpene species used
typedef enum {APIN, BPIN, LIMO, MYRC, SABI, CAMP, TRIC, TBOC, OTHR, NMTCOMPOUNDTYPES} monoterpenecompoundtype;

/// Fire model setting. Either use 
/**	One of
 *	BLAZE 		Use the BLAZE model to generate fire fluxes 
 *                      (must be accompanied by ignitionmode; DEFAULT)
 *	GLOBFIRM	fire parameterization following Thonicke et al. 2001
 *	NOFIRE		no fire model
 *	SPITFIRE	SPITFIRE fire model (Thonicke et al. 2010)
 *	STAT_BURNT_AREA		prescribed burnt area from statistical model
 *	PRESCR_BURNT_AREA	prescribed burnt area from input file
 *	PRESCR_NUMBER_FIRES	prescribed number of fires from input file
 *	FIXED_FIRE_RETURN	fixed fire return interval
 *	RANDOM_FIRE_RETURN	random fire with fixed mean return interval
 */
typedef enum {BLAZE, GLOBFIRM, NOFIRE, SPITFIRE, STAT_BURNT_AREA, PRESCR_BURNT_AREA, PRESCR_NUMBER_FIRES, FIXED_FIRE_RETURN, RANDOM_FIRE_RETURN} firemodeltype;

/// LandSyMM/SPITFIRE: Ignition mode
typedef enum {BOTH, HUMAN, LIGHTNING_ONLY, NOIGNITIONS} ignitionmodetype;
/// LandSyMM/SPITFIRE: Fuel moisture model
typedef enum {ORIGINAL, DAILY_VPD} fuelmoisturemodeltype;
/// LandSyMM/SPITFIRE: Pasture fire mode
typedef enum {NO_PASTURE_BURNING, FULL_PASTURE_BURNING, LIGHTNING_PASTURE_BURNING, PRESCRIBED_PASTURE_BURNING, MODELLED_PASTURE_BURNING} pasturefiretype;
/// LandSyMM/SPITFIRE: Cropland fire mode
typedef enum {NO_CROPLAND_BURNING, FULL_CROPLAND_BURNING, LIGHTNING_CROPLAND_BURNING, PRESCRIBED_CROPLAND_BURNING, MODELLED_CROPLAND_BURNING} cropfiretype;
/// LandSyMM/SPITFIRE: Wind speed limit model
typedef enum {NOLIMIT, LASSLOP, ROTHERMEL, ANDREWS} windlimittype;

/// IMOGEN simulation mode
typedef enum {ONLINE, OFFLINE} lpjgimogensimulationmode;
/// IMOGEN feedback mode
typedef enum {LPJG_OFFSET, MOVING_AVERAGE} lpjgimogenfeedbackmode;

/// Type of weathergenerator used 
/**     One of:
 *      GWGEN           Global Weather GENerator (needed by BLAZE, due to 
 *                      additional rel. humidity and wind; DEFAULT)
 *      INTERP          use standard interpolation scheme
 *      NONE            Should be set if daily input is used (e.g. in cfinput) 
 */
typedef enum {GWGEN, INTERP, NONE} weathergeneratortype;

///How to determine root distribution in soil layers
typedef enum {ROOTDIST_FIXED, ROOTDIST_JACKSON} rootdisttype;

///////////////////////////////////////////////////////////////////////////////////////
// Global instruction file parameters

/// Title for this run
extern xtring title;

/// Vegetation mode (population, cohort or individual)
extern vegmodetype vegmode;

/// Default number of patches in each stand
/** Should always be 1 in population mode,
 *  cropland stands always have 1 patch.
 *  Actual patch number for stand objects may differ and 
 *  should always be queried by stand.npatch()
 */
extern int npatch;

/// Number of patches in each stand for secondary stands
extern int npatch_secondarystand;

/// Whether to reduce equal percentage of all stands of a stand type at land cover change
extern bool reduce_all_stands;

/// Minimum age of stands to reduce at land cover change
extern int age_limit_reduce;

/// Patch area (m2) (individual and cohort mode only)
extern double patcharea;

/// Whether background establishment enabled (individual, cohort mode)
extern bool ifbgestab;

/// Whether spatial mass effect enabled for establishment (individual, cohort mode)
extern bool ifsme;

/// Whether establishment stochastic (individual, cohort mode)
extern bool ifstochestab;

/// Whether mortality stochastic (individual, cohort mode)
extern bool ifstochmort;

/// Fire-model switch
extern firemodeltype firemodel;

/// Weather Generator switch
extern weathergeneratortype weathergenerator;

/// Whether "generic" patch-destroying disturbance enabled (individual, cohort mode)
extern bool ifdisturb;

/// Generic patch-destroying disturbance interval (individual, cohort mode)
extern double distinterval;

/// Whether SLA calculated from leaf longevity (alt: prescribed)
extern bool ifcalcsla;

/// Whether leaf C:N ratio minimum calculated from leaf longevity (alt: prescribed)
extern bool ifcalccton;

/// Establishment interval in cohort mode (years)
extern int estinterval;

/// Whether C debt (storage between years) permitted
extern bool ifcdebt;

/// Water uptake parameterisation
extern wateruptaketype wateruptake;

/// Parameterisation of root distribution
extern rootdisttype rootdistribution;

/// whether CENTURY SOM dynamics (otherwise uses standard LPJ formalism)
extern bool ifcentury;

/// whether plant growth limited by available N
extern bool ifnlim;

/// number of years to allow spinup without nitrogen limitation
extern int freenyears;

/// fraction of nitrogen relocated by plants from roots and leaves
extern double nrelocfrac;

/// first term in nitrogen fixation eqn (Cleveland et al 1999)
extern double nfix_a;

/// second term in nitrogen fixation eqn (Cleveland et al 1999)
extern double nfix_b;

/// whether to use nitrification/denitrification in CENTURY SOM dynamics
extern bool ifntransform;
/// Fraction of microbial respiration assumed to produce DOC, 0.0,0.3
extern double frac_labile_carbon;

/// Soil pH (used for calculating N-transformation), 3.5,8.5
extern double pH_soil;
/// Maximum nitrification rate, 0.03,0.15
extern double f_nitri_max;
/// Constant in denitrification, 0.001,0.1
extern double k_N;
/// Constant in temperature function for denitrification, 0.005,0.05
extern double k_C;
/// Maximum gaseus losses in nitrification
extern double f_nitri_gas_max;
/// Maximum fraction of NO3 converted to NO2
extern double f_denitri_max;
/// Maximum fraction of NO2 converted to gaseus N
extern double f_denitri_gas_max;

// Mapping of text input file data in index file.
extern bool map_text_file;

///////////////////////////////////////////////////////////////////////////////////////
// Landuse and crop settings

/// Whether other landcovers than natural vegetation are simulated.
extern bool run_landcover;

/// Whether a specific landcover type is simulated (URBAN, CROPLAND, PASTURE, FOREST, NATURAL, PEATLAND, BARREN).
extern bool run[NLANDCOVERTYPES];

/// Whether landcover fractions are not read from input file.
extern bool lcfrac_fixed;

/// Whether fractions of stand types of a specific land cover are not read from input file.
extern bool frac_fixed[NLANDCOVERTYPES];

/// Whether BARREN landcover excluded from area fraction correction in cases of non-unity sum
extern bool no_barren_frac_corr;

/// Whether PEATLAND landcover excluded from area fraction correction in cases of non-unity sum
extern bool no_peatland_frac_corr;

/// Set to false by initio( ) if fraction input files have yearly data.
extern bool all_fracs_const;

/// Whether a fraction of harvested wood is put into a product pool
extern bool ifslowharvestpool;

/// Whether grass is allowed to grow between crop growingseasons
extern bool ifintercropgrass;

/// Whether to calculate dynamic potential heat units
extern bool ifcalcdynamic_phu;

/// Whether to use gross land transfer: read landcover transfer matrix input file (1), read stand type transfer matrix input file (2), or not (0)
extern int gross_land_transfer;

/// Whether gross land transfer input read for this gridcell
extern bool gross_input_present;

/// Whether to use primary/secondary land transition info in landcover transfer input file (1). or not (0)
extern bool ifprimary_lc_transfer;

/// Distinguish between primary and secondary natural stands at area reduction
extern bool use_primary_lc_transfer;

/// LandSyMM: Scaling factor for N fertiliser input
extern double Nfert_scale_factor;
/// LandSyMM: Manure C:N ratio
extern double manure_cn;
/// LandSyMM: Fraction of manure N in organic form
extern double manure_organic_frac;
/// LandSyMM/GGCMI: Whether to run potential yield simulations
extern bool do_potyield;
/// LandSyMM: Fix N fertilisation to a specific year (0=off, 1=fix, 2=cap, 3=floor)
extern int fixed_nfert;
/// LandSyMM: Year to use for fixed N fertilisation
extern int fixed_nfert_year;

/// LandSyMM/SPITFIRE: Control parameters
extern bool ifburnfullcell;
extern bool ifnesterovdistr;
extern double firereturninterval;
extern double pixeldegree;
extern double sapsizedecrease;
extern int fixburnday;
extern double fuelthreshold;
extern double mouillotmultiplier;
extern bool ifPandRresidencetime;
extern bool ifallocatebytreecover;
extern int min_days_between_burns;
extern double human_ignition_constant;
extern double fractionsoilmoistureinfuel;
extern int max_fire_duration;
extern int min_fire_duration;
extern bool ifpopulationsuppression;
extern double lightning_ctg_factor;
extern double crop_fraction_suppression_exponent;
/// LandSyMM/SPITFIRE: Mode enums
extern ignitionmodetype ignitionmode;
extern fuelmoisturemodeltype fuelmoisturemodel;
extern pasturefiretype pasturefiremode;
extern cropfiretype cropfiremode;
extern windlimittype windlimit;
/// LandSyMM/FireMIP: Experiment control flags
extern bool iffixedco2;
extern bool iffixedlightning;
extern bool iffixedburntarea;
extern bool iffixedlanduse;
extern bool iffixedhumanpopulation;

/// LandSyMM/GGCMI: Skip ALL processes except for PHU and PVD calculation
extern bool just_phu_pvd;
/// LandSyMM/GGCMI: GGCMI Phase 2 specific settings
extern bool ggcmi2;
/// LandSyMM/GGCMI: ISIMIP3 specific settings
extern bool isimip3;
/// LandSyMM/GGCMI: Per-growing-season crop outputs
extern bool crop_gs_out;
/// LandSyMM/GGCMI: Whether to read PHU from input
extern bool readphu;
/// LandSyMM/GGCMI: Whether to read PVD from input
extern bool readpvd;
/// LandSyMM/GGCMI: Whether to read growing season length from input
extern bool readgrowseaslength;
/// LandSyMM/GGCMI: Whether to read 2nd fertilization date from input
extern bool readNfertdate2;
/// LandSyMM/GGCMI: Whether fert rate not summing to 1 is an error
extern bool fertrate_error;
/// LandSyMM/GGCMI: Irrigation high-soil-moisture restriction threshold
extern double restrict_irr_wcont;
/// LandSyMM/GGCMI: Irrigation high-soil-ice restriction threshold
extern double restrict_irr_ice;
/// LandSyMM/GGCMI: Remove small water/ice amounts in update_ice_fraction
extern bool remove_smallvolfrac;
/// LandSyMM: First historic year after spinup
extern int firsthistyear;
/// LandSyMM: Last historic year of simulation
extern int lasthistyear;
/// LandSyMM: First calendar year for output
extern int firstoutyear;
/// LandSyMM: Last calendar year for output
extern int lastoutyear;
/// LandSyMM: Minimum seconds between progress messages
extern int mutesec;

/// Whether to use primary-to-secondary land transition info (within land cover type) in landcover transfer input file (1). or not (0)
extern bool ifprimary_to_secondary_transfer;

/// Pooling level of land cover transitions; 0: one big pool; 1: land cover-level; 2: stand type-level
extern int transfer_level;

/// Whether to create new stands in transfer_to_new_stand() according to the rules in copy_stand_type()
extern bool iftransfer_to_new_stand;

/// Whether to suppress disturbance and fire in forestry stands created as NATURAL stands in transfer_to_new_stand_from_stand() or transfer_to_new_stand_from_st_lc() (eg. LUH2 input)
extern bool suppress_disturbance_in_forestry_stands;

/// Whether to harvest (remove) wood at natural-to-forest transitions
extern bool harvest_natural_to_forest;

/// Whether to limit dynamic phu calculation to a period specified by nyear_dyn_phu
extern bool ifdyn_phu_limit;

/// Number of years to calculate dynamic phu if dynamic_phu_limit is true
extern int nyear_dyn_phu;

/// number of spinup years
extern int nyear_spinup;

/// Whether to use sowingdates from input file
extern bool readsowingdates;

/// Whether to use harvestdates from input file
extern bool readharvestdates;

/// Whether to read N fertilization from input file
extern bool readNfert;

/// Whether to read manure N fertilization from input file
extern bool readNman;

/// Whether to read N fertilization (stand type level) from input file
extern bool readNfert_st;

/// Whether to use forest harvested fraction from input file (using LUC functionality)
extern bool readwoodharvest_frac;

/// Whether to use wood harvest C mass from input file (using LUC functionality)
extern bool readwoodharvest_cmass;

/// Whether to create new stands at clearcut of secondary stands when using wood harvest input (LUC functionality)
extern bool harvest_secondary_to_new_stand;

/// Whether to read disturbance intervals from input file
extern bool readdisturbance;

/// Whether to read disturbance intervals for stand types from input file
extern bool readdisturbance_st;

/// Whether to read cutinterval for stand types from input file
extern bool readcutinterval_st;

/// Whether to read stand type elevation from input file
extern bool readelevation_st;

/// Whether to read firstmanageyear for stand types from input file
extern bool readfirstmanageyear_st;

// Whether to read target-cutting distribution for selection in mt from input file
extern bool readtargetcutting;

/// Whether to burn thin trees during tree harvest (ignoring pft.harvest_slow_frac)
extern bool harvest_burn_thin_trees;

/// Whether to print multiple stands within a stand type (except cropland) separately
extern bool printseparatestands;

/// Whether to simulate tillage by increasing soil respiration
extern bool iftillage;

/// Use silt/sand fractions per soiltype
extern bool textured_soil;

/// Whether pastures are affected by disturbance and fire (affects pastures' npatch)
extern bool disturb_pasture;

/// Whether to simulate cropland as pasture
extern bool grassforcrop;

///////////////////////////////////////////////////////////////////////////////////////
// IMOGEN-Intermediary parameters
///////////////////////////////////////////////////////////////////////////////////////

namespace IMOGENConfig {
	extern xtring DIR_COMMON;
	extern xtring DIR_COMMON_OUT;
	extern xtring DIR_PATT;
	extern xtring DIR_CLIM;
	extern xtring FILE_SCEN_EMITS;
	extern xtring FILE_NON_CO2_VALS;
	extern xtring FILE_CH4_N2O_EMITS;
	extern xtring FILE_LPJG_FLUX;
	extern xtring FILE_GRIDLIST;
	extern xtring FILE_LPJG_CH4_N2O_FLUX;
	extern xtring FILE_SCEN_CO2_PPMV;

	extern int STEP_DAY;
	extern double T_OCEAN_INIT;
	extern double KAPPA_O;
	extern double F_OCEAN;
	extern double LAMBDA_L;
	extern double LAMBDA_O;
	extern double MU;
	extern double Q2CO2;
	extern double TAU_DECAY_CH4;
	extern double TAU_DECAY_N2O;

	extern int NYR_NON_CO2;
	extern int NYR_EMISS_NONCO2;
	extern int NYR_EMISS;
	extern int NYR_LPJG_FLUX;

	extern double CO2_INIT_PPMV;
	extern double CH4_INIT_PPBV;
	extern double N2O_INIT_PPBV;

	extern bool NONCO2_EMISSIONS;
	extern bool NONCO2_EMISSIONS_LPJG;
	extern bool C_EMISSIONS;
	extern bool LPJG_CFLUX;
	extern bool INCLUDE_CO2;
	extern bool INCLUDE_NON_CO2;
	extern bool DAILYOUT;
	extern bool LAND_FEED;
	extern bool OCEAN_FEED;
	extern bool ANLG;
	extern bool ANOM;
	extern bool REGRID;
	extern bool CO2_RF_FAIR;
	extern bool FILE_NON_CO2;

	extern bool print_imogen_output;
	extern bool include_feedback;

	extern xtring simulation_mode;
	extern xtring feedback_mode;
	extern xtring interpolation_mode;

	extern int YEAR1;
	extern int IYEND;
	extern int YEAR1_LPJG;
	extern bool SPINUP;
	extern bool KEEPRUNNING;
	extern bool FIRSTCALL;

	extern xtring scenario;
	extern int firstyear;
	extern int lastyear;
	extern int lpjg_start_year;
	extern int lpjg_end_year;

	extern xtring baseDirectory;
	extern xtring pathToLogFile;
	extern xtring myPLUMDataFilePath;
	extern xtring lpjgOutputDirectory;

	extern xtring methaneEmissionOutputFilePath;
	extern xtring entericFermentationMethaneOutputFilePath;
	extern xtring manureManagementMethaneOutputFilePath;
	extern xtring nitrogenEmissionOutputFilePath;
	extern xtring methaneNitrogenTotalOutputFilePath;

	extern xtring historic_nitrogen_fertiizer_file_path;
	extern xtring arable_lands_nitrogen_fertilizer_path;

	extern xtring wetlandsAreaFilePath;
	extern xtring wetlandsMethaneEmissionsOutputPath;

	extern xtring basePathPLUMdata;
	extern xtring basePathPLUMdata_v2;
	extern xtring plumFertilizerOutputPath;
	extern xtring plumFertlizerPath;

	extern xtring file_LPJG_IPCC_Path;
	extern xtring lpjgMethane;
	extern xtring lpjgNitrogen;
	extern xtring lpjgCflux;
	extern xtring lpjgCflux_plus_IIASA_lpjg_co2;

	extern xtring IIASA_lpjg_co2_file_path;
	extern xtring IIASA_non_lpjg_1850_2100;
	extern xtring IIASA_lpjg_1850_2100;

	extern xtring livestock_counts_path;
	extern xtring fao_stats_path;

	extern xtring ssprcp;

	extern void print_all();
}

///////////////////////////////////////////////////////////////////////////////////////
// Settings controlling the saving and loading from state files

/// Location of state files
extern xtring state_path;

/// Whether to restart from state files
extern bool restart;

/// Whether to save state files
extern bool save_state;

/// Whether to write land use fraction data to memory for fast random access
/// Enables efficient usage of randomised gridlists for parallel simulations
extern bool lutomemory;

/// Whether to use fork's irrigation water uptake logic with patch.hydrology
/// enum dispatch, get_soil_water_status(), and dynamic vectors.
/// Default false = LTS management-based isirrigated boolean logic.
extern bool iflandsymm_irrigation_logic;

/// Whether to use fork's day-1 nfert initialization and add_fertilizer_manure()
/// instead of LTS per-event direct application with inline manure.
/// Default false = LTS per-event application with StandType-level nfert.
extern bool iflandsymm_nfert_init;

/// Whether to use fork's infiltration routing (infiltrate_upland() helper
/// and saturate_nonpeat_wetlands()) instead of LTS inline proportional
/// distribution. Default false = LTS inline infiltration.
extern bool iflandsymm_infiltration;

/// Whether to use fork's direct-to-plant BNF pathway (fixed N goes to
/// nmass_leaf, nmass_root, nmass_agpool) instead of LTS soil mineral pool
/// pathway (fixed N goes to soil.NH4_mass). Default false = LTS pathway.
extern bool iflandsymm_bnf_direct;

/// Whether to use simplified N-stress assignment (fork behavior):
/// nstress=false when labile N covers demand, nstress=ifnlim when not.
/// LTS default (false) uses n_opt_isabovelim to persist N-limitation on Vmax.
extern bool iflandsymm_nstress_simple;

/// Whether to use PFT-specific d3 parameter for leaf senescence threshold
/// (fork behavior) instead of hardcoded 1.0 at anthesis (LTS behavior)
extern bool iflandsymm_senescence_d3;

/// Whether to use LandSyMM's simplified forestry functions for externally-driven
/// forest management (cut_fraction, landsymm_clearcut, landsymm_copy_stand_type,
/// landsymm_transfer_to_new_stand) instead of the full LTS forestry system.
/// Only has effect when run_forest=1. Default false uses full LTS forestry.
extern bool landsymm_simple_forestry;

/// Save/restart year (LTS name)
extern int state_year;

/// LandSyMM: Restart year (alias for state_year when restarting)
extern int& restart_year;

/// LandSyMM: Save year (alias for state_year when saving)
extern int& save_year;

/// LandSyMM: Freeze-thaw scheme (0=energy_balance LTS default, 1=wania LandSyMM)
extern bool ifwania_freezethaw;

/// LandSyMM: CN solver (0=cnstep_full LTS default, 1=cnstep Wania simplified)
extern bool ifwania_cnsolver;

/// LandSyMM: pH-dependent N cycling (Val Martin 2023, Parton 1996, Ma 2022)
extern bool ifphdependent_ncycle;

/// LandSyMM: Nesterov index Tmax>0 filter (1=LTS default with filter, 0=LandSyMM without)
extern bool ifnesterov_tmax_filter;

/// LandSyMM: Chilldays warmest-day reset (1=LTS default with reset, 0=LandSyMM without)
extern bool ifchilldays_warmest_reset;

/// LandSyMM: BLAZE CWD scaling factor (1.0=LTS default, 2.0=LandSyMM)
extern double blaze_cwd_factor;

/// LandSyMM: GWGEN DTR half-range factor (0=LTS full range, 1=LandSyMM half range)
extern bool ifgwgen_dtr_halfrange;

/// LandSyMM: C-to-DM conversion factor for yield (2.0=LTS default, 2.2422=LandSyMM 1/0.446)
extern double c_to_dm_factor;


/// LandSyMM: Fire population density input method (0=undefined, 1=simfire binary, 2=netcdf)
extern int fire_popdens_method;

/// LandSyMM: Keep popdens at fixed year (cfxinput only)
extern int fixed_popdens_hist;

/// LandSyMM: Year to use for fixed popdens
extern int fixed_popdens_year;

/// The level of verbosity
extern int verbosity;

/// whether to vary mort_greff smoothly with growth efficiency (1) or to use the standard step-function (0)
extern bool ifsmoothgreffmort;

/// whether establishment is limited by growing season drought
extern bool ifdroughtlimitedestab;

/// rain on wet days only (1, true), or a little every day (0, false);
extern bool ifrainonwetdaysonly;

/// whether BVOC calculations are included
extern bool ifbvoc;

///////////////////////////////////////////////////////////////////////////////////////
// Arctic and wetland inputs

/// Use the original LPJ-GUESS v4 soil scheme, or not. If true, override many of the switches below.
extern bool iftwolayersoil; 

/// Use multilayer snow scheme, or the original LPJ-GUESS v4 scheme
extern bool ifmultilayersnow;

/// whether to reduce GPP if there's inundation (1), or not (0)
extern bool ifinundationstress;

/// Whether to limit soilC decomposition below 0 degC in upland soils (1), or not (0)
extern bool ifcarbonfreeze;

/// Extra daily water input or output, in mm, to wetlands. Positive values are run ON, negative run OFF.
extern double wetland_runon;

/// Whether methane calculations are included
extern bool ifmethane;

/// Whether soil C pool input is used to update soil properties
extern bool iforganicsoilproperties;

/// Whether to take water from runoff to saturate low latitide wetlands
extern bool ifsaturatewetlands;


///////////////////////////////////////////////////////////////////////////////////////
// The Paramlist class (and Paramtype)
//

/// Represents one custom "param" item
/** \see Paramlist */
struct Paramtype {
	xtring name;
	xtring str;
	double num;
};

/// List for the "custom" parameters
/** Functionality for storing and retrieving custom "param" items from the instruction
 *  script. "Custom" parameters can be accessed by other modules without the need to
 *  define them beforehand. This of course also means there is no help text associated
 *  with these parameters, so the user can't get any documentation about them from
 *  the command line.
 *
 * Custom keywords may be included in the instruction script using syntax similar to
 * the following examples:
 *
 * \code
 *     param "co2" (num 340)
 *     param "file_gridlist" (str "gridlist.txt")
 * \endcode
 *
 * To retrieve the values associated with the "param" strings in the above examples,
 * use the following function calls (may appear anywhere in this file; instruction
 * script must have been read in first):
 *
 * \code
 *     param["co2"].num
 *     param["file_gridlist"].str
 * \endcode
 *
 * Each "param" item can store EITHER a number (int or double) OR a string, but not
 * both types of data. Function fail is called to terminate output if a "param" item
 * with the specified identifier was not read in.
 */
class Paramlist : public ListArray<Paramtype> {

public:
	/// Adds a parameter with a numeric value, overwriting if it already existed
	void addparam(xtring name,xtring value);

	/// Adds a parameter with a string value, overwriting if it already existed
	void addparam(xtring name,double value);

	/// Fetches a parameter from the list, aborts the program if it didn't exist
	Paramtype& operator[](xtring name);

	/// Tests if param exists
	bool isparam(xtring name);

	/// Tries to find the parameter in the list
	/** \returns 0 if it wasn't there. */
	Paramtype* find(xtring name);
};

/// The global Paramlist object
/** Contains all the custom parameters after reading in the instruction file */
extern Paramlist param;

/// Reads in the instruction file
/** Uses PLIB library functions to read instructions from file specified by
 * 'insfilename'.
 */
void read_instruction_file(const char* insfilename);

/// Displays documentation about the instruction file parameters to the user
void printhelp();


///////////////////////////////////////////////////////////////////////////////////////
// Interface for declaring parameters from other modules

/// Declares an xtring parameter
/** \param name     The name of the parameter
 *  \param param    Pointer to variable where the value of the parameter is to be placed
 *  \param maxlen   Maximum allowed length of the parameter in the ins file
 *  \param help     Documentation describing the parameter to the user
 */
void declare_parameter(const char* name, xtring* param, int maxlen, const char* help = "");

/// Declares a std:string parameter
/** \param name     The name of the parameter
 *  \param param    Pointer to variable where the value of the parameter is to be placed
 *  \param maxlen   Maximum allowed length of the parameter in the ins file
 *  \param help     Documentation describing the parameter to the user
 */
void declare_parameter(const char* name, std::string* param, int maxlen, const char* help = "");

/// Declares an int parameter
/** \param name     The name of the parameter
 *  \param param    Pointer to variable where the value of the parameter is to be placed
 *  \param min      Minimum allowed value of the parameter in the ins file
 *  \param max      Maximum allowed value of the parameter in the ins file
 *  \param help     Documentation describing the parameter to the user
 */
void declare_parameter(const char* name, int* param, int min, int max, const char* help = "");

/// Declares a double parameter
/** \param name     The name of the parameter
 *  \param param    Pointer to variable where the value of the parameter is to be placed
 *  \param min      Minimum allowed value of the parameter in the ins file
 *  \param max      Maximum allowed value of the parameter in the ins file
 *  \param help     Documentation describing the parameter to the user
 */
void declare_parameter(const char* name, double* param, double min, double max, const char* help = "");

/// Declares a bool parameter
/** \param name     The name of the parameter
 *  \param param    Pointer to variable where the value of the parameter is to be placed
 *  \param help     Documentation describing the parameter to the user
 */
void declare_parameter(const char* name, bool* param, const char* help = "");

#endif // LPJ_GUESS_PARAMETERS_H
