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

LandSyMM runs require external forcing data not included in this repository:

- **Climate:** ISIMIP3b daily NetCDF (historical + SSP scenarios)
- **Land-use:** Remapped HILDA+ historic / PLUMharm scenario fractions
- **Soil:** Soil property maps (texture, pH, AWC)
- **N deposition:** ISIMIP3 monthly wet+dry NHx/NOy
- **CO2:** Annual concentration time series
- **Population density:** For fire models (SIMFIRE/BLAZE)

**KIT IMK-IFU members:** Data available on Simba2 cluster at `/bg/data/lpj/`

**External collaborators:** Contact the maintainers for data access.

**Land-use data production:** The `landsymm_py` repository contains the complete Python pipeline for producing LU fraction files from raw HILDA+ and PLUM scenario data.

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
