# Changelog — LPJ-GUESS Integrated (LandSyMM + LTS)

## v1.0-landsymm-integration (2026-01-27)

### Baseline

| Attribute | Value |
|-----------|-------|
| Version | LPJ-GUESS 4.1 (`reference/releasenotes.txt` in upstream tree) |
| Branch | `trunk` |
| HEAD commit | `a4575b9bd8cf86636a522154baebf29fac8ab422` |
| Commit date | 2026-03-12 (committer: `LPJ-GUESS_admin`) |
| Upstream remote | `git@stormbringer4.nateko.lu.se:lpj-guess-developers/LPJ-GUESS.git` (Lund University) |

### Scientific Context

The LandSyMM modifications integrated here exist to support the coupled LPJ-GUESS ↔ PLUM/PLUMv2 land-use modeling framework described in:

- Alexander, P., Rabin, S., Anthoni, P., Henry, R., Pugh, T. A. M., Rounsevell, M. D. A., & Arneth, A. (2018). *Adaptation of global land use and management intensity to changes in climate and atmospheric carbon dioxide.* Global Change Biology, 24, 2791–2809. doi:10.1111/gcb.14110
- Rabin, S. S., Alexander, P., Henry, R., Anthoni, P., Pugh, T. A. M., Rounsevell, M., & Arneth, A. (2020). *Impacts of future agricultural change on ecosystem service indicators.* Earth System Dynamics, 11, 357–376. doi:10.5194/esd-11-357-2020

The LPJ-GUESS modifications in the LandSyMM fork enable: (a) externally prescribed land-use forcing from PLUM scenarios via the companion `landsymm_py` pipeline, (b) potential-yield factorial runs (3 fertilizer rates × 2 irrigation regimes per crop) that generate the biophysical inputs PLUMv2 consumes, (c) optional wetland/peatland CH₄ modeling for IMOGEN-coupled climate runs, and (d) related crop management, fire, and nitrogen-cycling improvements. See `README.md` and `docs/LandSyMM_LPJ-GUESS_Integration_Technical_Manual.md` Section 1.1 for the full scientific motivation.

### What This Is

An integrated version of LPJ-GUESS that combines the upstream Latest Stable Release (LTS) with all features from the LandSyMM (Land System Modular Model) fork. Both LTS and LandSyMM behaviors are available from a single binary, selected via runtime parameters in `.ins` files.

### Added Features

- **SPITFIRE fire model** — Process-based fire spread, intensity, and mortality (Thonicke et al. 2010) alongside existing BLAZE model
- **GGCMI crop intercomparison** — Standardized crop benchmarking protocols, forced PHU/PVD, per-growing-season outputs
- **IMOGEN climate coupling** — Intermediate complexity climate model for coupled vegetation-climate simulations
- **Extended CFXInput module** — NetCDF-based climate input with multi-part file support (`file_temp1`/`file_temp2` for historical+scenario)
- **Potential-yield factorial mode** (`do_potyield`/`isforpotyield`) — Enables a single LPJ-GUESS run to produce per-grid-cell yield surfaces for crops under six management treatments (3 fertilizer rates × 2 irrigation regimes), the biophysical input PLUMv2 consumes for its economic land-use optimization (Alexander et al., 2018; Rabin et al., 2020)
- **Enhanced crop management** — 5 irrigation/hydrology types (rainfed, irrigated, irrigated-wilt, irrigated-sat, inundated), per-crop phenology pipeline (`iflandsymm_crop_management`), per-management N fertilization
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
- **Fix 4:** Corrected `initial_infiltration()` wetland gate in `soilwater.cpp` — added `&& ifsaturatewetlands` to match the fork's `do_saturate()` logic. The integrated was unconditionally saturating low-latitude wetland soil daily, ignoring the `ifsaturatewetlands` parameter. Result: peatland outlier gridcell resolved from +141% to -0.5% bias; Peatland_sum MedRel from 8.96% to 0.82%, Corr from 0.73 to 0.99.
- **Completeness sweep items:** (a) Timer convenience methods `write_outputs()` and `is_firsthist_or_restart_year()` added to Date class in `guess.h`. (b) Evaporation snow threshold parameterized in `hydrology_lpjf`: fork uses `snowpack` (SWE mm), LTS uses `dsnowdepth` (actual depth mm); now gated by `iflandsymm_hydrology_routing`. (c) `rain_melt_orig` saved before routing to fix baseflow percolation limiter. (d) `isinundated` percolation guards added to `hydrology_lpjf_twolayer` — fork skips overflow and percolation for INUNDATED stands during growing season; 4 guards added, gated by `iflandsymm_hydrology_routing`.
- **NBP/AET output improvements** (from earlier tech_rename_aet_nee branch, D. Bampoh 2023): (a) Separated NEE (atmosphere-ecosystem exchange only) from NBP (full biome budget including harvest/LUC/manure/leaching) in `cflux.out` — the previous "NEE" column was actually closer to NBP. (b) Renamed monthly output from `file_mnee` to `file_mnbp`. (c) Decomposed `aaet.out` from per-PFT columns to Evap/Intercep/Transp/Total + per-landcover sums. (d) Added new `file_atransp` for per-PFT annual transpiration. (e) Added `file_mnee` as permanent backward-compatible alias for `file_mnbp` — existing ins files referencing `file_mnee` continue to work. Updated all standard and template ins files from `!file_mnee` to `!file_mnbp`.

### Documentation Expansion (2026-04-22)

- Added `docs/final_comprehensive_verification_report.md` — 500-line evidence-based analysis of final 24-run verification suite
- Technical manual: Added Section 10A (ins file architecture, import chain, override semantics, common pitfalls), Section 10B (5 generic simulation recipes from production to quick-test), Section 10C (step-by-step verification suite workflow with copy-paste commands)
- README: Added final verification report to documentation table
- Fixed stale `landsymm_py` URL in `landcover.ins` (`git.scc.kit.edu` → `gitlab.imk-ifu.kit.edu`)

### Final Comprehensive Verification (2026-04-21)

Full 24-run verification suite with freshly compiled binaries (12 configs × 2 binaries): Historical + SSP126 × Deterministic + Stochastic × do_potyield=0/1/peatland. Results confirm integration fidelity:

- Core ecosystem pools: 0.1–0.8% MedRel, >0.993 correlation across all 12 configs
- Natural vegetation PFTs: 0.00–0.36% MedRel, >0.989 correlation
- Hydrology: 0.5–3.4% MedRel (routing improvement), >0.996 correlation
- Peatland (post-Fix 4): 0.9–2.5% MedRel, >0.997 correlation (was +141% pre-fix)
- Crop yields: 5–30% due to nitrification gas fix (genuine improvement)
- 78 variables >5% MedRel in baseline config — 100% categorized and explained:
  - 55% crop/yield (nitrification fix), 15% nitrogen fluxes (nitrification fix)
  - 15% BNE/BINE PFTs + tiny fluxes, 8% fire (stochastic), 4% NEE (redefined), 3% hydrology
- Zero unexplained divergences across any configuration

See `docs/final_comprehensive_verification_report.md` for the full evidence-based analysis.

### Verification Investigation Outcomes (2026-01-27/28)

All 6 issues from the Phase 2 verification test suite were investigated to root cause:

- **Issue 3 (N-fixer BNF bias):** BNF code correctly parameterized. 8 code differences analyzed — 5 neutralized by runtime parameters, 3 residual are fork bugs corrected in integrated. The nitrification fix accounts for ~1/3 of the 5-9% crop divergence; the rest is baseline integration cost from genuine improvements compounding during spinup.
- **Issue 4 (Fire uncorrelated):** Statistical artifact — fire occurs at only 2 of 13 demo gridcells, with 80% fewer events in potyield=1 mode. BLAZE code correctly parameterized.
- **Issue 5 (Peatland -20 to -42% bias):** Root cause identified — `initial_infiltration()` gate in `soilwater.cpp` bypassed `ifsaturatewetlands` parameter, causing unconditional daily wetland saturation. Fix 4 (one-line correction) resolved the outlier from +141% to -0.5%. Peatland_sum Corr improved from 0.73 to 0.99.
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

See `docs/comprehensive_phase2_debug_report.md` for details on the 6 identified issues — all investigated to root cause, 4 resolved with code fixes (Fixes 1-4), 2 explained as acceptable divergence from genuine LTS improvements.

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
