# LPJ-GUESS Integrated (LandSyMM + LTS)

An integrated version of **LPJ-GUESS 4.1** that combines the upstream Latest Stable Release (LTS) with all features from the **LandSyMM** (Land System Modular Model) fork. Both LTS and LandSyMM behaviors are available from a single binary, selected entirely via runtime parameters in `.ins` files.

## Quick Start

```bash
# Build
mkdir build && cd build
cmake .. -DCMAKE_CXX_FLAGS="-O2"
make -j$(nproc)

# Run in standard LTS mode (no LandSyMM features):
./guess -input demo ../data/ins/demo_global.ins

# Run in LandSyMM mode (requires external forcing data):
# 1. Configure data paths:
cd ../data/landsymm-integrated-ins
./setup_paths.sh /path/to/data/root /path/to/lu/root
# 2. Run:
cd ../../build
./guess -input cfx ../data/landsymm-integrated-ins/main.ins
```

## Key Design Principle

All LandSyMM features default to OFF (LTS behavior). Enable them by setting runtime parameters in `global.ins`:

```ini
iflandsymm_blaze_fork 1          ! Fork BLAZE fire behavior
iflandsymm_bnf_direct 1          ! BNF direct to plant tissue
iflandsymm_vegdyn_fork 1         ! Fork establishment/mortality
! ... (16 behavioral + 8 physics parameters total)
```

See `data/landsymm-integrated-ins/global.ins` for the complete parameter set with annotations.

## Documentation

| Document | Description |
|----------|-------------|
| **[Technical Manual](docs/LandSyMM_LPJ-GUESS_Integration_Technical_Manual.md)** | Comprehensive guide: integration process, runtime parameters, verification, debugging, future integration procedures |
| **[Debug Report](docs/comprehensive_phase2_debug_report.md)** | Phase 2 verification results, 6 identified issues with root causes and fix plans |
| **[Changelog](CHANGELOG.md)** | Summary of all additions, bug fixes, and known issues |

## Data Requirements

LandSyMM runs require external forcing data not included in this repository due to size (tens of GB) and licensing restrictions. The data are organized by category below, with the corresponding ins file parameter names.

### Climate Forcing (ISIMIP3b, daily NetCDF)

| Parameter | Variable | Description | Example File |
|-----------|----------|-------------|-------------|
| `file_temp1` | `tas` | Near-surface air temperature (K), historical 1850–2014 | `mri-esm2-0_..._historical_tas_..._1850_2014.nc4` |
| `file_prec1` | `pr` | Precipitation (kg/m²/s), historical | `..._historical_pr_..._1850_2014.nc4` |
| `file_insol1` | `rsds` | Surface downwelling shortwave radiation (W/m²) | `..._historical_rsds_..._1850_2014.nc4` |
| `file_wind1` | `sfcwind` | Near-surface wind speed (m/s) | `..._historical_sfcwind_..._1850_2014.nc4` |
| `file_relhum1` | `hurs` | Near-surface relative humidity (%) | `..._historical_hurs_..._1850_2014.nc4` |
| `file_min_temp1` | `tasmin` | Daily minimum temperature (K) | `..._historical_tasmin_..._1850_2014.nc4` |
| `file_max_temp1` | `tasmax` | Daily maximum temperature (K) | `..._historical_tasmax_..._1850_2014.nc4` |
| `file_temp2` ... `file_max_temp2` | Same variables | SSP126 scenario, 2015–2100 | `..._ssp126_..._2015_2100.nc4` |

Source: ISIMIP3b climate forcing, bias-adjusted, land-only (e.g., MRI-ESM2-0). Available from [ISIMIP](https://www.isimip.org/gettingstarted/input-data-bias-adjustment/).

### Nitrogen Deposition (ISIMIP3, monthly NetCDF)

| Parameter | Variable | Description |
|-----------|----------|-------------|
| `file_mNHxdrydep` | `drynhx` | Dry NHx deposition (monthly, 1850–2100) |
| `file_mNOydrydep` | `drynoy` | Dry NOy deposition |
| `file_mNHxwetdep` | `wetnhx` | Wet NHx deposition |
| `file_mNOywetdep` | `wetnoy` | Wet NOy deposition |

Source: ISIMIP3 N-deposition, histsoc+ssp126soc, wet+dry split, LPJ-GUESS format.

### CO2 Concentration

| Parameter | Description |
|-----------|-------------|
| `file_co2` | Annual CO2 concentration (ppm), 1850–2100. Text file with year and CO2 columns. |

Source: ISIMIP3 CO2 forcing (historical + SSP126).

### Soil Properties

| Parameter | Description |
|-----------|-------------|
| `file_soildata` | Soil property map: texture class, AWC, pH, etc. Remapped to LandSyMM gridlist. Binary `.dat` format. |

Source: ISRIC SoilGrids / HWSD, processed and remapped for LandSyMM grid.

### Land-Use Fractions

| Parameter | Description |
|-----------|-------------|
| `file_lu` | Land-use fractions (cropland, pasture, natural, urban, peatland). Text format, annual time series per gridcell. |
| `file_lucrop` | Crop-type fractions within cropland. Text format, annual. |
| `file_Nfert` | Nitrogen fertilization rates per crop type. Text format, annual. |
| `file_irrigintens` | Irrigation intensity fractions (for SSP scenarios). |

**Historical LU** is produced from HILDA+ data via the LandSyMM Python remapping pipeline.
**Scenario LU** is produced from PLUM scenario output via PLUMharm harmonization.

For peatland runs, use `LU.remapv10_old_62892_gL_peatland.txt` and `landcover_peatland.txt` variants.

**LU data production pipeline:** The `landsymm_py` repository contains the complete Python pipeline:
- KIT GitLab: `https://gitlab.imk-ifu.kit.edu/bampoh-d/landsymm_py`
- Helmholtz GitLab: `https://codebase.helmholtz.cloud/daniel.bampoh/landsymm_py`

### Fire Model Inputs

| Parameter | Description |
|-----------|-------------|
| `file_popdens` | Population density (persons/km²), NetCDF, annual 1601–2100. For BLAZE fire model ignition. |
| `file_simfire` | SIMFIRE input binary. Alternative fire population density source. |

Source: ISIMIP3 population data (HYDE + SSP projections).

### Crop Phenology (Optional, for `iflandsymm_crop_management=1`)

| Parameter | Description |
|-----------|-------------|
| `file_phu_in` | Potential Heat Units per crop type. Text format, per `cropphen_col` column. |
| `file_pvd_in` | Potential Vernalization Days per crop type. |
| `file_sdates` | Sowing dates per crop type. |
| `file_hdates` | Harvest dates per crop type. |
| `file_growseaslength_in` | Growing season length per crop type. |

Source: GGCMI crop calendar data or LandSyMM-specific phenology files. Only needed when `iflandsymm_crop_management=1` and per-crop differentiation is required.

### Gridlist

| Parameter | Description |
|-----------|-------------|
| `file_gridlist` / `file_gridlist_cf` | List of gridcell coordinates (lon, lat). Text file. |

Several gridlists are provided in `data/landsymm-integrated-ins/` for testing (1-cell, 2-cell, 480-cell, full global).

### Data Access

**KIT IMK-IFU members:** All forcing data available on the Simba2 HPC cluster:
- Climate: `/bg/data/lpj/LPJ-GUESS/input/isimip/isimip3/`
- N deposition: `/bg/data/lpj/LPJ-GUESS/input/isimip/isimip3/n-deposition/`
- CO2: `/bg/data/lpj/LPJ-GUESS/input/isimip/isimip3/co2/`
- Population: `/bg/data/lpj/LPJ-GUESS/input/isimip/isimip3/pop/`
- Fire: `/bg/data/lpj/LPJ-GUESS/input/fire/`
- Land-use: `/bg/data/lpj/$USER/landsymm_lu/`

**External collaborators:** Contact Daniel Bampoh (daniel.bampoh@kit.edu, KIT IMK-IFU) for data access arrangements.

### Path Configuration

After obtaining the data, configure paths using the provided setup script:
```bash
cd data/landsymm-integrated-ins
./setup_paths.sh /path/to/your/data/root /path/to/your/lu/root
```
This replaces `<DATA_ROOT>` and `<LU_ROOT>` placeholders in all ins files. See the script's help (`./setup_paths.sh` without arguments) for KIT Simba2 example paths.

## Repository Structure

```
framework/                  Core data structures, parameters, simulation loop
modules/                    Science modules (canopy exchange, fire, crops, SOM, ...)
data/
  ins/                      Standard LTS instruction files
  landsymm-integrated-ins/  LandSyMM template instruction files (with setup_paths.sh)
docs/                       Technical manual and debug reports
```

## Branch Structure

- `trunk` — Original upstream LTS (Lund University)
- `landsymm/integration` — The integrated codebase (main working branch)
- `landsymm/*` — Feature branches for individual integration steps

## License

Mozilla Public License 2.0 (inherited from upstream LPJ-GUESS)
