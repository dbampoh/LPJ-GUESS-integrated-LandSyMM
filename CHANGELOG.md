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

### Post-Verification Fixes (2026-01-27)

- **Fix 2:** Added `standpft.active` guard to `commonoutput.cpp` output accumulation loop — resolved 100% divergence in clitter FruitAndVeg and Barren_sum outputs
- **Fix 1:** Ported fork crop management pipeline to `externalinput.cpp` — added `getphu()`/`getpvd()`/`getgrowseaslength()`/`getNfertdate2()` functions, `cropphen_col` column lookup in `getsowingdates()`/`getharvestdates()`, gated by `iflandsymm_crop_management` parameter. Required for production LandSyMM runs with PHU/PVD data files.

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

LandSyMM runs require external forcing data (climate, land-use, soil, N deposition, CO2, population density) not included in this repository. See the technical manual Section 3 and `data/landsymm-integrated-ins/` for path configuration. Contact the maintainers for data access.

### Related Repositories

- **landsymm_py** — Python pipeline for land-use remapping and harmonization (produces the LU fraction input files)
- **LPJ-GUESS upstream** — Lund University GitLab (`stormbringer4.nateko.lu.se`)
- **LandSyMM fork** — Bitbucket (`bitbucket.org/samrabin/landsymm-lpjg`)
