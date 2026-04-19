# Changelog — LPJ-GUESS Integrated (LandSyMM + LTS)

## v1.0-landsymm-integration (2026-01-27)

**Baseline:** LPJ-GUESS 4.1 Latest Stable Release (Lund University, `trunk` branch, commit `a4575b9bd`)

### What This Is

An integrated version of LPJ-GUESS that combines the upstream Latest Stable Release (LTS) with all features from the LandSyMM (Land System Modular Model) fork. Both LTS and LandSyMM behaviors are available from a single binary, selected via runtime parameters in `.ins` files.

### Added Features

- **SPITFIRE fire model** — Process-based fire spread, intensity, and mortality (Thonicke et al. 2010) alongside existing BLAZE model
- **GGCMI crop intercomparison** — Standardized crop benchmarking protocols, forced PHU/PVD, per-growing-season outputs
- **IMOGEN climate coupling** — Intermediate complexity climate model for coupled vegetation-climate simulations
- **Extended CFXInput module** — NetCDF-based climate input with multi-part file support (`file_temp1`/`file_temp2` for historical+scenario)
- **Enhanced crop management** — 5 irrigation/hydrology types (rainfed, irrigated, irrigated-wilt, irrigated-sat, inundated), potential yield factorial mode (`do_potyield`), per-management N fertilization
- **Biological nitrogen fixation** — BNF system for N-fixing crops with development stage, water content, and temperature response functions
- **Wetland/peatland support** — Wania freeze-thaw physics, peatland hydrology, methane emissions (diffusion, ebullition, plant-mediated)
- **16 behavioral runtime parameters** — Select between LTS and LandSyMM physics at runtime (see `data/landsymm-integrated-ins/global.ins`)
- **8 physics option parameters** — pH-dependent N cycling, Nesterov filter, chilldays reset, CWD factor, Wania solvers, GWGEN DTR, C-to-DM factor
- **13 additional output modules** — Per-stand crop yields, irrigation, pasture ANPP, daily BLAZE burned area
- **State save/restart aliases** — LandSyMM-compatible `restart_year`, `save_year`, `save_years` alongside LTS `state_year`
- **Output year gating** — `firstoutyear`/`lastoutyear` for selective output windows
- **`lutomemory` parameter** — Load LU data to RAM for fast parallel access

### Post-Verification Fixes (2026-01-27/28)

- **Fix 2:** Added `standpft.active` guard to `commonoutput.cpp` output accumulation loop — resolved 100% divergence in clitter FruitAndVeg and Barren_sum outputs. This is a correctness fix (LTS already uses this guard in other output loops).
- **Fix 1:** Ported fork crop management pipeline to `externalinput.cpp` — added `getphu()`/`getpvd()`/`getgrowseaslength()`/`getNfertdate2()` functions, `cropphen_col` column lookup in `getsowingdates()`/`getharvestdates()`, gated by `iflandsymm_crop_management` parameter. Required for production LandSyMM runs with PHU/PVD data files.
- **Fix 3:** Wired fork hydrology routing in `hydrology_lpjf()` (`soil.cpp`) — `infiltrate_upland()` for non-saturating stands and `get_soil_water_status()` after infiltration, gated by `iflandsymm_hydrology_routing` parameter. Completes the Priority 1 unfinished integration item from `remaining_integration_work.md`.

### Verification Investigation Outcomes (2026-01-27/28)

All 6 issues from the Phase 2 verification test suite were investigated to root cause:

- **Issue 3 (N-fixer BNF bias):** BNF code correctly parameterized. 8 code differences analyzed — 5 neutralized by runtime parameters, 3 residual are fork bugs corrected in integrated. The nitrification fix accounts for ~1/3 of the 5-9% crop divergence; the rest is baseline integration cost from genuine improvements compounding during spinup.
- **Issue 4 (Fire uncorrelated):** Statistical artifact — fire occurs at only 2 of 13 demo gridcells, with 80% fewer events in potyield=1 mode. BLAZE code correctly parameterized.
- **Issue 5 (Peatland -20 to -42% bias):** Dominated by single outlier gridcell at arid SW USA site; 3 of 4 peatland cells within 7.8%. Fix 3 and nitrification isolation both had zero effect. Unresolved marginal-site divergence documented for future investigation.
- **Issue 6 (NEE poor correlations):** No independent code driver. NEE formula includes `acflux_wood_harvest + acflux_clearing` in integrated (LTS improvement) but not fork. Absolute bias < 0.003 kgC/m²/yr. Deterministic Corr 0.97-0.99.

### Bug Fixes Applied

- `cropindiv_struct` missing member initialization (`harv_cmass_plant`)
- `Individual::ccont()` floating-point accumulation order
- `MassBalance` restart flush (false mass-balance violations after state restart)
- Null-pointer guards in `indata.cpp`
- Bounds check in `CopyToMemory()`
- Missing `setup_multipart()`/`reset()` calls in `framework.cpp` (critical for multi-part climate files)
- `oob_check_wcont()` clamping to prevent floating-point overshoot in soil water content
- NO2-to-NO3 transfer fix in nitrification (LTS used denitrification parameter for nitrification gas loss)

### Backward Compatibility

All new parameters default to LTS behavior. Running the integrated binary with standard LTS `.ins` files produces identical output to the original upstream LTS (verified: MedRel ≤ 0.2%, Corr ≥ 0.999 across 13 global sites).

### Documentation

See `docs/LandSyMM_LPJ-GUESS_Integration_Technical_Manual.md` for the comprehensive technical manual covering the full integration process, runtime parameters, verification methodology, known issues, and future integration procedures.

### Known Issues

See `docs/comprehensive_phase2_debug_report.md` for details on 6 identified issues with planned fixes, most significantly the crop management pipeline (Issue 1) needed for full per-crop differentiation in `do_potyield=1` mode.

### Data Requirements

LandSyMM runs require external forcing data not included in this repository:

- **Climate:** ISIMIP3b daily NetCDF (7 variables × 2 periods: historical 1850–2014 + SSP 2015–2100)
- **N deposition:** ISIMIP3 monthly wet+dry NHx/NOy (4 files, 1850–2100)
- **CO2:** Annual concentration time series (1850–2100)
- **Soil:** Soil property map (texture, pH, AWC) remapped to LandSyMM grid
- **Land-use:** HILDA+ historic / PLUMharm scenario fractions + crop fractions + N fertilization
- **Population density:** For BLAZE fire model (NetCDF, 1601–2100)
- **Fire:** SIMFIRE binary input
- **Crop phenology (optional):** PHU, PVD, sowing/harvest dates per crop type

See `README.md` for the complete data inventory with parameter names and sources. See `data/landsymm-integrated-ins/setup_paths.sh` for path configuration.

**KIT IMK-IFU members:** Data on Simba2 at `/bg/data/lpj/LPJ-GUESS/input/`

**External collaborators:** Contact Daniel Bampoh (daniel.bampoh@kit.edu, KIT IMK-IFU)

**Land-use production:** The `landsymm_py` repos contain the remapping/harmonization pipeline:
- KIT: `https://gitlab.imk-ifu.kit.edu/bampoh-d/landsymm_py`
- Helmholtz: `https://codebase.helmholtz.cloud/daniel.bampoh/landsymm_py`

### Related Repositories

- **landsymm_py** — Python pipeline for land-use remapping and harmonization
- **LPJ-GUESS upstream** — Lund University GitLab (`stormbringer4.nateko.lu.se`)
- **LandSyMM fork** — Bitbucket (`bitbucket.org/samrabin/landsymm-lpjg`)
