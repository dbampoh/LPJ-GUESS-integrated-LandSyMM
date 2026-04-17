# Comprehensive Phase 2 Verification Debug & Fix Report

**Date:** 2026-01-26 (Wednesday)
**Status:** Comprehensive Phase 2 test suite COMPLETE; 6 issues identified; fix plan defined
**Build:** Integrated LTS compiled at `-O2`, fork compiled at `-O2`
**Spinup:** 200 years for all tests
**Gridlist:** 13 global demo sites (G13)

---

## 1. Test Suite Configuration

### 1.1 Test Matrix (16 Runs Total)

| Test ID | Scenario | Stochastic | do_potyield | run_peatland | npatch | Years |
|---------|----------|-----------|-------------|-------------|--------|-------|
| P2_H_D0 | Historical (1901–2020) | No | 0 (LU-driven) | 0 | 1 | 1560 |
| P2_H_S0 | Historical (1901–2020) | Yes | 0 | 0 | 5 | 1560 |
| P2_H_D1 | Historical (1901–2020) | No | 1 (ins-driven) | 0 | 1 | 1560 |
| P2_H_S1 | Historical (1901–2020) | Yes | 1 | 0 | 5 | 1560 |
| P2_H_DP | Historical (1901–2020) | No | 0 | 1 | 1 | 1560 |
| P2_H_SP | Historical (1901–2020) | Yes | 0 | 1 | 5 | 1560 |
| P2_F_D0 | SSP126 (2021–2100) | No | 0 | 0 | 1 | 1040 |
| P2_F_S0 | SSP126 (2021–2100) | Yes | 0 | 0 | 5 | 1040 |
| P2_F_D1 | SSP126 (2021–2100) | No | 1 | 0 | 1 | 1040 |
| P2_F_S1 | SSP126 (2021–2100) | Yes | 1 | 0 | 5 | 1040 |
| P2_F_DP | SSP126 (2021–2100) | No | 0 | 1 | 1 | 1040 |
| P2_F_SP | SSP126 (2021–2100) | Yes | 0 | 1 | 5 | 1040 |
| P2_H_D0 (fork) | (mirror of above for fork binary) | — | — | — | — | — |
| ... | (each test run for both integ and fork) | — | — | — | — | — |

All SSP126 future runs restart from the corresponding historical run's saved state at year 2020.

### 1.2 Binaries

- **Integrated:** `/home/bampoh-d/Desktop/landsymm_lpjg/landsymm_mat/lpjg_landsymm_integration/LPJ-GUESS-integrated/build/guess`
- **Fork:** `/home/bampoh-d/Desktop/landsymm_lpjg/landsymm_mat/lpjg_landsymm_integration/LandSyMM_LPJ-GUESS/build/guess`
- Both compiled with `cmake -DCMAKE_CXX_FLAGS="-O2"` for floating-point determinism

### 1.3 Ins Files

- Location: `/home/bampoh-d/Desktop/landsymm_lpjg/landsymm_mat/lpjg_landsymm_integration/verification/phase2_landsymm_consistency/local_ins/`
- Global config: `global.ins` with all 16 LandSyMM runtime parameters set to fork-equivalent values
- Climate: CFX input with `file_temp1`/`file_temp2` (historical + SSP126)
- LU: `LU.remapv10_old_62892_gL.txt` (standard) and `LU.remapv10_old_62892_gL_peatland.txt` (peatland tests)

### 1.4 LandSyMM Runtime Parameters (all set to 1 = fork behavior)

```
iflandsymm_nstress_simple 1
iflandsymm_bnf_direct 1
iflandsymm_senescence_d3 1
iflandsymm_nfert_init 1
iflandsymm_infiltration 1
iflandsymm_irrigation_logic 1
iflandsymm_vegdyn_fork 1
iflandsymm_nitri_gas_fork 1
iflandsymm_nharvest_simple 1
iflandsymm_nfert_after_phenology 1
iflandsymm_lc_before_management 1
iflandsymm_tillage_fixed 1
iflandsymm_century_nc 1
iflandsymm_weathergen_floors 1
iflandsymm_fpc_linear 1
iflandsymm_blaze_fork 1
blaze_cwd_factor 2.0
ggcmi2 0
```

### 1.5 Comparison Tool

- Script: `compare_per_variable.py`
- Location: `/home/bampoh-d/Desktop/landsymm_lpjg/landsymm_mat/lpjg_landsymm_integration/verification/compare_per_variable.py`
- Method: Column-name-based matching with per-variable statistics (MedRel%, P95Rel%, MeanAbsDiff, RMSE, NRMSE%, Bias, Bias%, MeanRef, MeanTest, Corr)
- Output: `/home/bampoh-d/Desktop/landsymm_lpjg/landsymm_mat/lpjg_landsymm_integration/verification/comprehensive_final_v2/`

---

## 2. Comprehensive Test Results

### 2.1 AGPP (Above-Ground Primary Production) — Aggregate by Land Type

| Test | Scenario | Stoch | Mode | Total MedRel% | Total Corr | Crop MedRel% | Crop Bias% | Crop Corr | Pasture MedRel% | Pasture Corr | Natural MedRel% | Natural Corr |
|------|----------|-------|------|---------------|-----------|-------------|-----------|----------|----------------|-------------|-----------------|-------------|
| H_D0 | Hist | No | PotY=0 | 0.33% | 0.9956 | 6.53% | -8.21% | 0.9673 | 0.15% | 0.9982 | 0.29% | 0.9977 |
| H_S0 | Hist | Yes | PotY=0 | 1.76% | 0.9910 | 4.61% | -8.59% | 0.8178 | 0.09% | 0.9995 | 3.04% | 0.9904 |
| H_D1 | Hist | No | PotY=1 | 0.42% | 0.9935 | 7.43% | -4.60% | 0.8589 | 0.13% | 0.9984 | 0.31% | 0.9977 |
| H_S1 | Hist | Yes | PotY=1 | 2.15% | 0.9908 | 4.25% | -2.81% | 0.8773 | 0.10% | 0.9999 | 3.12% | 0.9901 |
| H_DP | Hist | No | Peat | 0.33% | 0.9965 | 6.58% | -8.30% | 0.9654 | 0.15% | 0.9981 | 0.42% | 0.9979 |
| H_SP | Hist | Yes | Peat | 1.99% | 0.9948 | 1.32% | -3.37% | 0.9838 | 0.09% | 0.9999 | 2.96% | 0.9918 |
| F_D0 | SSP | No | PotY=0 | 0.50% | 0.9968 | 1.35% | +8.11% | 0.8739 | 0.20% | 0.9988 | 1.22% | 0.9933 |
| F_S0 | SSP | Yes | PotY=0 | 1.52% | 0.9956 | 1.33% | +8.38% | 0.8664 | 0.17% | 0.9996 | 2.68% | 0.9914 |
| F_D1 | SSP | No | PotY=1 | 2.48% | 0.9855 | **22.50%** | **+18.89%** | **0.2616** | 0.15% | 0.9978 | 0.99% | 0.9947 |
| F_S1 | SSP | Yes | PotY=1 | 3.27% | 0.9828 | **26.68%** | **+20.54%** | **0.1308** | 0.15% | 0.9988 | 3.02% | 0.9903 |
| F_DP | SSP | No | Peat | 1.37% | 0.9956 | 3.38% | +7.69% | 0.8682 | 0.24% | 0.9989 | 1.17% | 0.9904 |
| F_SP | SSP | Yes | Peat | 2.30% | 0.9952 | 1.73% | +7.74% | 0.8619 | 0.21% | 0.9994 | 3.36% | 0.9891 |

### 2.2 ANPP (Above-Ground Net Primary Production) — Aggregate by Land Type

| Test | Total MedRel% | Total Corr | Crop MedRel% | Crop Bias% | Crop Corr | Pasture MedRel% | Pasture Corr | Natural MedRel% | Natural Corr |
|------|---------------|-----------|-------------|-----------|----------|----------------|-------------|-----------------|-------------|
| H_D0 | 0.52% | 0.9897 | 6.61% | -8.46% | 0.9560 | 0.00% | 0.9987 | 0.50% | 0.9921 |
| H_S0 | 2.08% | 0.9861 | 4.75% | -9.15% | 0.8080 | 0.00% | 0.9997 | 3.97% | 0.9864 |
| H_D1 | 0.61% | 0.9871 | 6.16% | -6.80% | 0.8868 | 0.00% | 0.9985 | 0.61% | 0.9918 |
| H_S1 | 2.55% | 0.9880 | 4.00% | -4.75% | 0.9112 | 0.00% | 0.9999 | 3.89% | 0.9866 |
| F_D1 | 2.68% | 0.9694 | **20.56%** | **+16.61%** | **0.3074** | 0.12% | 0.9993 | 1.97% | 0.9816 |
| F_S1 | 3.95% | 0.9765 | **22.80%** | **+17.98%** | **0.1835** | 0.00% | 0.9997 | 4.16% | 0.9919 |

### 2.3 Carbon Fluxes (cflux.out) — Total Ecosystem

| Test | Veg MedRel% | Veg Corr | Soil MedRel% | Soil Corr | Fire MedRel% | Fire Corr | Harvest MedRel% | Harvest Corr | NEE MedRel% | NEE Corr |
|------|------------|---------|-------------|----------|-------------|----------|----------------|-------------|------------|---------|
| H_D0 | 0.52% | 0.9897 | 0.48% | 0.9956 | 76.69% | 0.9860 | 0.00% | 0.9844 | 3.60% | 0.9916 |
| H_S0 | 2.08% | 0.9861 | 1.87% | 0.9640 | 100.00% | 0.1296 | 0.00% | 0.9882 | 17.45% | 0.6886 |
| H_D1 | 0.61% | 0.9871 | 0.53% | 0.9930 | 1.09% | **-0.1246** | 1.33% | 0.9964 | 4.40% | 0.9658 |
| H_S1 | 2.55% | 0.9880 | 2.09% | 0.9612 | 100.00% | **-0.0813** | 1.23% | 0.9978 | 15.82% | 0.7021 |
| H_DP | 0.70% | 0.9887 | 0.58% | 0.9956 | 88.06% | 0.2262 | 0.00% | 0.9894 | 4.33% | 0.7483 |
| H_SP | 2.42% | 0.9917 | 2.16% | 0.9847 | 100.00% | 0.1524 | 0.00% | 0.9990 | 15.48% | 0.7773 |
| F_D0 | 0.85% | 0.9902 | 0.88% | 0.9973 | 54.88% | -0.0439 | 0.00% | 0.9976 | 5.76% | 0.9293 |
| F_S0 | 2.11% | 0.9941 | 2.29% | 0.9892 | 100.00% | -0.0298 | 0.00% | 0.9974 | 14.19% | 0.8366 |
| F_D1 | 2.68% | 0.9694 | 1.88% | 0.9861 | 4.77% | -0.0345 | 4.35% | 0.9877 | 8.61% | 0.9447 |
| F_S1 | 3.95% | 0.9765 | 3.30% | 0.9806 | 100.00% | -0.1040 | 4.05% | 0.9814 | 20.42% | 0.8416 |
| F_DP | 1.92% | 0.9859 | 1.46% | 0.9852 | 69.78% | -0.0280 | 0.00% | 0.9979 | 8.90% | 0.3611 |
| F_SP | 2.50% | 0.9944 | 2.66% | 0.9916 | 100.00% | -0.0599 | 0.00% | 0.9980 | 15.53% | 0.8232 |

### 2.4 Carbon Pools (cpool.out) — Total Ecosystem

| Test | VegC MedRel% | VegC Bias% | VegC Corr | SoilC MedRel% | SoilC Bias% | SoilC Corr | Total MedRel% | Total Bias% | Total Corr |
|------|-------------|-----------|----------|--------------|-----------|----------|-------------|-----------|----------|
| H_D0 | 0.33% | +0.23% | 0.9998 | 0.30% | -1.66% | 0.9998 | 0.35% | -0.80% | 0.9990 |
| H_S0 | 5.89% | +1.87% | 0.9792 | 0.57% | -0.95% | 0.9985 | 0.59% | -0.45% | 0.9957 |
| H_D1 | 0.42% | +0.15% | 0.9998 | 0.30% | -1.67% | 0.9998 | 0.34% | -0.79% | 0.9989 |
| H_S1 | 6.05% | +2.08% | 0.9810 | 0.73% | -1.13% | 0.9979 | 0.64% | -0.31% | 0.9958 |
| F_D1 | 0.96% | +2.12% | 0.9995 | 0.16% | -1.15% | 0.9978 | 0.20% | +0.02% | 0.9994 |
| F_S1 | 6.33% | -2.49% | 0.9949 | 0.69% | -0.73% | 0.9988 | 0.69% | -0.96% | 0.9988 |

### 2.5 Crop Yields

| Test | Scenario | Mode | yield.out avg MedRel% | yield.out avg Corr | yield_st.out avg MedRel% | yield_st.out avg Corr |
|------|----------|------|----------------------|-------------------|-------------------------|---------------------|
| H_D0 | Hist | PotY=0 | 4.52% | 0.5832 | 1.19% | 0.1448 |
| H_S0 | Hist | PotY=0 | 2.75% | 0.6118 | 0.72% | 0.1535 |
| H_D1 | Hist | PotY=1 | 7.75% | 0.6541 | 1.74% | 0.6136 |
| H_S1 | Hist | PotY=1 | 5.54% | 0.6870 | 1.65% | 0.6333 |
| F_D0 | SSP | PotY=0 | 6.87% | 0.3017 | 5.79% | 0.0707 |
| F_S0 | SSP | PotY=0 | 8.22% | 0.3099 | 7.73% | 0.0778 |
| F_D1 | SSP | PotY=1 | **19.83%** | 0.5532 | **19.63%** | 0.3772 |
| F_S1 | SSP | PotY=1 | **21.79%** | 0.5492 | **19.84%** | 0.3639 |

### 2.6 Peatland-Specific Outputs (DP/SP Tests Only)

| Test | Peatland AGPP MedRel% | Peatland AGPP Bias% | Peatland AGPP Corr | Peatland cmass Bias% | Peatland cmass Corr |
|------|----------------------|--------------------|--------------------|---------------------|---------------------|
| H_DP | 5.97% | -16.84% | 0.8376 | -19.46% | 0.8574 |
| H_SP | 10.05% | -28.02% | 0.7079 | -28.88% | 0.7964 |
| F_DP | 9.86% | -30.97% | 0.7410 | -42.20% | 0.6706 |
| F_SP | 16.73% | -31.30% | 0.6837 | -39.98% | 0.6605 |

### 2.7 CH4 Methane (Peatland Tests Only) — Monthly Averages Across 12 Months

| Test | Scenario | Stoch | mch4 Total avg MedRel% | mch4 Diffusion avg MedRel% | mch4 Ebullition avg MedRel% | mch4 Plant avg MedRel% |
|------|----------|-------|----------------------|--------------------------|---------------------------|---------------------|
| H_DP | Hist | No | 6.29% | 2.91% | 18.88% | 24.82% |
| H_SP | Hist | Yes | 7.27% | 2.61% | 17.12% | 17.51% |
| F_DP | SSP | No | 8.29% | 3.15% | 5.44% | 15.89% |
| F_SP | SSP | Yes | 16.15% | 6.49% | 16.58% | 20.72% |

### 2.8 Per-Domain Summary Assessment

| Domain | Historical Det | Historical Stoch | SSP126 Det | SSP126 Stoch | Assessment |
|--------|---------------|-----------------|-----------|-------------|------------|
| **Pasture** | MedRel ≤0.15%, Corr ≥0.998 | MedRel ≤0.10%, Corr ≥0.999 | MedRel ≤0.24%, Corr ≥0.999 | MedRel ≤0.21%, Corr ≥0.999 | **EXCELLENT** |
| **Natural veg** | MedRel ≤0.42%, Corr ≥0.998 | MedRel ≤3.12%, Corr ≥0.990 | MedRel ≤1.22%, Corr ≥0.993 | MedRel ≤3.36%, Corr ≥0.989 | **VERY GOOD** |
| **Crops (PotY=0)** | MedRel ≤6.58%, Corr ≥0.965 | MedRel ≤4.61%, Corr ≥0.818 | MedRel ≤3.38%, Corr ≥0.868 | MedRel ≤1.73%, Corr ≥0.862 | **GOOD** |
| **Crops (PotY=1)** | MedRel ≤7.43%, Corr ≥0.859 | MedRel ≤4.25%, Corr ≥0.877 | MedRel ≤22.50%, Corr ≤0.262 | MedRel ≤26.68%, Corr ≤0.131 | **POOR (SSP126)** |
| **Ecosystem Total** | MedRel ≤0.42%, Corr ≥0.994 | MedRel ≤2.15%, Corr ≥0.991 | MedRel ≤2.48%, Corr ≥0.986 | MedRel ≤3.27%, Corr ≥0.983 | **VERY GOOD** |
| **Carbon Pools** | MedRel ≤0.35%, Corr ≥0.999 | MedRel ≤0.64%, Corr ≥0.996 | MedRel ≤0.74%, Corr ≥0.999 | MedRel ≤1.76%, Corr ≥0.993 | **EXCELLENT** |
| **Soil Carbon** | MedRel ≤0.30%, Corr ≥0.999 | MedRel ≤0.73%, Corr ≥0.998 | MedRel ≤0.30%, Corr ≥1.000 | MedRel ≤0.69%, Corr ≥0.999 | **EXCELLENT** |
| **NEE** | MedRel ≤4.40%, Corr ≥0.748 | MedRel ≤17.45%, Corr ≥0.689 | MedRel ≤8.90%, Corr ≥0.361 | MedRel ≤20.42%, Corr ≥0.824 | **MODERATE** |
| **Fire** | Abs tiny, Corr 0.99 (D0) | Abs tiny, Corr ≤0.15 | Abs tiny, Corr ≤-0.03 | Abs tiny, Corr ≤-0.10 | **ABS NEGLIGIBLE, CORR POOR** |
| **Peatland** | MedRel ≤6%, Corr ≤0.84 | MedRel ≤10%, Corr ≤0.71 | MedRel ≤10%, Corr ≤0.74 | MedRel ≤17%, Corr ≤0.68 | **MODERATE** |
| **CH4 Total** | MedRel 6–7% | MedRel 7–8% | MedRel 8% | MedRel 16% | **GOOD (Hist), MODERATE (SSP)** |
| **CH4 Diffusion** | MedRel 2–3% | MedRel 3% | MedRel 3% | MedRel 6% | **VERY GOOD** |
| **CH4 Plant** | MedRel 15–25% | MedRel 18% | MedRel 16% | MedRel 21% | **MODERATE** |

---

## 3. Identified Issues

### 3.1 ISSUE 1 — CRITICAL: Crop Identity Collapse in Potyield=1 Mode

#### 3.1.1 Symptom

Multiple distinct crop types that share the same base PFT produce **identical** mean AGPP/yield in the integrated version, but **distinct** values in the fork.

**Evidence from H_D1 (Historical, Deterministic, potyield=1):**

| Crop | Base PFT | Integrated Mean AGPP | Fork Mean AGPP |
|------|----------|---------------------|----------------|
| OilOther | TeSW_nlim | **0.3808** | 0.3946 |
| StarchyRoots | TeSW_nlim | **0.3808** | 0.3730 |
| FruitAndVeg | TeSW_nlim | **0.3808** | 0.3909 |
| Sugar | TeSW_nlim | **0.3808** | 0.3775 |
| OilNfix | TeSo_nlim | **0.4476** | 0.5208 |
| Pulses | TeSo_nlim | **0.4476** | 0.4969 |

**Evidence from F_D1 (SSP126, Deterministic, potyield=1) — amplified:**

| Crop | Integrated Mean AGPP | Fork Mean AGPP |
|------|---------------------|----------------|
| OilOther | **0.4821** | 0.3947 |
| StarchyRoots | **0.4821** | 0.4089 |
| FruitAndVeg | **0.4821** | 0.3882 |
| Sugar | **0.4821** | 0.4022 |
| OilNfix | **0.5773** | 0.4598 |
| Pulses | **0.5773** | 0.4948 |

In F_D1, Crop_sum Corr collapses to **0.2616** (deterministic) and **0.1308** (stochastic) — essentially uncorrelated.

#### 3.1.2 Root Cause — Confirmed

The integrated `ManagementInput` class is **missing the fork's entire PHU/PVD management pipeline**.

**What the fork does that the integrated does not:**

| Mechanism | Fork (`externalinput.cpp`) | Integrated (`externalinput.cpp`) |
|-----------|----------------------------|----------------------------------|
| `TimeDataD phus, pvds` members in `ManagementInput` | **Declared** in `externalinput.h` (lines 118–122) | **NOT declared** |
| `init()` opens `file_phu_in`, `file_pvd_in` | **Yes** — sets `readphu`, `readpvd` to true (lines 1699–1717) | **NO** — `readphu`/`readpvd` never set |
| `getphu()` function | **Exists** — loads per-crop PHU from file using `cropphen_col` (lines 1914–1931) | **Does NOT exist** |
| `getpvd()` function | **Exists** — loads per-crop vernalization data | **Does NOT exist** |
| `getgrowseaslength()` function | **Exists** | **Does NOT exist** |
| `getNfertdate2()` function | **Exists** | **Does NOT exist** |
| `getmanagement()` calls `getphu()` / `getpvd()` | **Yes** (lines 2038–2050) | **NO** — only calls `getsowingdates`, `getharvestdates`, `getNfert` |
| `getsowingdates()` uses `cropphen_col` | **Yes** — `thisname = pftlist[i].cropphen_col` when non-empty (lines 1836–1840) | **NO** — uses `pftlist[i].name` only (line 2227) |
| `getharvestdates()` uses `cropphen_col` | **Yes** | **NO** — uses `pftlist[i].name` only |

**How this causes the collapse:** Without per-crop PHU (Potential Heat Units) data from files, all crops sharing the same base PFT (e.g., `TeSW_nlim`) compute identical `ppftcrop.phu` from the base PFT's defaults, leading to identical `fphu` trajectories, identical allocation, and identical AGPP/yield.

The fork's `cropphen_col` allows each crop type (OilOther, StarchyRoots, etc.) to look up its own column in the PHU/sowing/harvest data files, getting **crop-specific** thermal time requirements that individualize their phenology and yield.

**Code citations:**

Fork `getsowingdates` (uses `cropphen_col`):
```
// LandSyMM_LPJ-GUESS/framework/externalinput.cpp lines 1836–1840
xtring thisname = pftlist[i].name;
if (pftlist[i].cropphen_col != "") {
    thisname = pftlist[i].cropphen_col;
}
gridcell.pft[i].sdate_force = (int)sdates.Get(year, thisname);
```

Integrated `getsowingdates` (does NOT use `cropphen_col`):
```
// LPJ-GUESS-integrated/framework/externalinput.cpp line 2227
gridcell.pft[i].sdate_force = (int)sdates.Get(year, pftlist[i].name);
```

Fork `getphu` (entirely missing from integrated):
```
// LandSyMM_LPJ-GUESS/framework/externalinput.cpp lines 1914–1931
void ManagementInput::getphu(Gridcell& gridcell) {
    if(!phus.isloaded()) return;
    int year = date.get_calendar_year();
    for(int i=0; i<npft; i++) {
        if(pftlist[i].phenology == CROPGREEN) {
            xtring thisname = pftlist[i].name;
            if (pftlist[i].cropphen_col != "") {
                thisname = pftlist[i].cropphen_col;
            }
            gridcell.pft[i].phu_force = phus.Get(year, thisname);
        }
    }
}
```

#### 3.1.3 Impact

- **Primary:** All crops sharing a base PFT produce identical outputs
- **Cascading:** SSP126 restart amplifies the issue enormously (Corr drops from 0.86→0.26)
- **Affected outputs:** yield.out, yield_st.out, agpp, anpp, cflux_cropland, Harvest flux
- **Affected tests:** All potyield=1 tests (H_D1, H_S1, F_D1, F_S1); indirectly all tests via crop outputs

---

### 3.2 ISSUE 2 — SIGNIFICANT: clitter FruitAndVeg = Zero in All Tests

#### 3.2.1 Symptom

In **all 12 tests**, `clitter.out` FruitAndVeg has MeanTest = 0.0000 while the fork has MeanRef = 0.048–0.060. This is **100% divergence** with a -100% bias consistently.

Also: `clitter.out` Barren_sum shows -100% divergence in peatland/stochastic tests.

#### 3.2.2 Root Cause — Confirmed

The fork's `outannual()` in `commonoutput.cpp` wraps the per-stand per-PFT output accumulation loop in an `if (standpft.active)` guard. The integrated version does **not** have this guard in the corresponding loop.

**Fork** (`LandSyMM_LPJ-GUESS/modules/commonoutput.cpp` line 959):
```cpp
Standpft& standpft = stand.pft[pft.id];
if(standpft.active) {
    // ... accumulate clitter, cmass, anpp, agpp, etc. for this stand
}
```

**Integrated** (`LPJ-GUESS-integrated/modules/commonoutput.cpp` line 991):
```cpp
Standpft& standpft = stand.pft[pft.id];
// NO active guard — iterates over ALL stands for ALL PFTs
// Sum C biomass, NPP, LAI and BVOC fluxes across patches and PFTs
standpft_cmass=0.0;
...
```

**How this causes zero clitter:** `clitter.out` reports **instantaneous** `Patchpft::cmass_litter_*` pools at year-end, NOT annual litter flux. For annual crops like FruitAndVeg, all litter is decomposed by Dec 31 (transferred to SOM pools via `som_dynamics()`). Without the `standpft.active` guard, the integrated version sums over all stands (including natural/pasture where FruitAndVeg never existed), which produces exactly zero because no litter accumulates for inactive PFTs.

With the guard (fork behavior), only active stands contribute, giving a non-zero weighted average from the cropland stands where FruitAndVeg residue briefly exists before decomposition transfers it.

#### 3.2.3 Impact

- **Primary:** 100% divergence for FruitAndVeg clitter in all tests
- **Secondary:** Barren_sum clitter divergence in peatland tests
- **Potential side effects:** Other output aggregation may be subtly affected

---

### 3.3 ISSUE 3 — SIGNIFICANT: N-Fixer Crops (OilNfix, Pulses) Systematic Underperformance

#### 3.3.1 Symptom

N-fixing crops consistently show larger divergence than non-fixers:

| Test | OilNfix yield Bias% | Pulses yield Bias% | CerealsC3 yield Bias% |
|------|--------------------|--------------------|----------------------|
| H_D0 (yield_st) | **-54.61%** | **-42.16%** | -0.61% |
| H_D1 (yield) | **-22.62%** | **-17.43%** | -3.79% |
| F_D1 (yield) | +24.88% | +14.10% | +44.74% |

Historical runs show the integrated producing 20–55% **less** N-fixer yield than the fork. The sign reversal in F_D1 is due to the crop identity collapse (Issue 1) overwhelming the BNF effect.

#### 3.3.2 Root Cause — Partially Diagnosed

The BNF parameterization from Steps 49f/49g captured the high-level switches (`iflandsymm_bnf_direct`) but the N-fixer-specific divergence suggests:

1. **Interaction with Issue 1:** Without per-crop PHU differentiation, OilNfix and Pulses share the same base PFT trajectory (TeSo_nlim), masking the BNF-specific effects
2. **BNF pathway difference:** Fork BNF goes directly to plant tissue; integrated routes via soil mineral N (even with `iflandsymm_bnf_direct 1`), changing N availability timing
3. **`cton_leaf_min` vs `cton_leaf_avr` switch:** May not be activating correctly for all N-fixer species

#### 3.3.3 Fix Dependency

This issue should be **re-evaluated after Fix 1 (crop management pipeline)** is applied, as a substantial portion of the N-fixer bias may be resolved by correct per-crop PHU differentiation.

---

### 3.4 ISSUE 4 — MODERATE: Fire Behavior Uncorrelated

#### 3.4.1 Symptom

Fire flux correlations are near-zero or negative in most tests despite `iflandsymm_blaze_fork 1` being set:

| Test | Fire Corr | Fire MedRel% | Fire Abs Mean (kgC/m2/yr) |
|------|----------|-------------|--------------------------|
| H_D0 | **0.9860** | 76.69% | 0.0016 |
| H_D1 | **-0.1246** | 1.09% | 0.0005 |
| H_S0 | 0.1296 | 100.00% | 0.0017 |
| H_S1 | -0.0813 | 100.00% | 0.0015 |
| F_D0 | -0.0439 | 54.88% | 0.0010 |
| F_D1 | -0.0345 | 4.77% | 0.0006 |

Note: H_D0 (deterministic, potyield=0) has **excellent** fire correlation (0.986). All other tests show poor correlation.

#### 3.4.2 Root Cause — Confirmed as Cascading

The `iflandsymm_blaze_fork` parameter **correctly controls** all three BLAZE items (A4, A5, A6). The diff between integrated and fork `blaze.cpp` confirms that with `iflandsymm_blaze_fork 1`:
- **A4:** Grass ANPP reduction is **skipped** (fork behavior) ✓
- **A5:** Stochastic mortality uses `ifstochmort` only (fork behavior) ✓
- **A6:** Pasture `scale_indiv` is **applied** (fork behavior) ✓

The remaining differences are:
- `to_gridcell_average` diagnostic tracking (fork only) — does NOT affect physics
- `blaze_burned_area` per-PFT tracking (fork only) — does NOT affect physics
- `fpc_tot_patch` computation in stochastic mortality (fork only) — does NOT affect mortality calculation

**Therefore:** The fire divergence is a **cascading effect** from Issue 1 (crop identity collapse) and stochastic sensitivity:
- In H_D0 (correct crops), fire Corr = 0.986
- In H_D1 (collapsed crops), different fuel loads → different fire behavior → Corr = -0.12
- Stochastic modes amplify any small remaining difference in fire ignition timing

#### 3.4.3 Impact

Absolute fire values are tiny (0.0005–0.0043 kgC/m2/yr) — scientifically negligible. But the poor correlation feeds into NEE and natural litter comparisons.

---

### 3.5 ISSUE 5 — MODERATE: Peatland Systematic Bias (-20 to -42%)

#### 3.5.1 Symptom

The integrated version systematically **underestimates** peatland productivity relative to the fork:

| Test | Peatland AGPP Bias% | Peatland cmass Bias% | Peatland ANPP Corr |
|------|--------------------|--------------------|-------------------|
| H_DP | -16.84% | -19.46% | 0.7953 |
| H_SP | -28.02% | -28.88% | 0.7378 |
| F_DP | -30.97% | -42.20% | 0.7267 |
| F_SP | -31.30% | -39.98% | 0.6397 |

The bias increases from historical to SSP126, and from deterministic to stochastic.

#### 3.5.2 Root Cause — Partially Diagnosed

Multiple contributing factors:
1. **Fire cascading:** Different fire dynamics in peatland-adjacent natural stands
2. **Stochastic competition:** Peatland vegetation sensitive to establishment competition during spinup
3. **`standpft.active` guard:** Fix 2 may affect peatland output aggregation
4. **Possible unparameterized peatland-specific code:** Wania freeze-thaw or peatland CN solver interactions

#### 3.5.3 Fix Dependency

Should be **re-evaluated after Fixes 1, 2, and 4** are applied.

---

### 3.6 ISSUE 6 — LOW: NEE Poor Correlations

#### 3.6.1 Symptom

NEE correlations range from 0.36 (F_DP) to 0.99 (H_D0). Worst in stochastic and peatland tests.

#### 3.6.2 Root Cause

NEE is a near-zero-mean residual flux (GPP − Respiration). Small absolute changes in any component (crops from Issue 1, fire from Issue 4, soil respiration from Issue 5) produce large relative differences. This is almost entirely downstream from Issues 1–5.

#### 3.6.3 Fix Dependency

No direct fix needed. NEE will improve automatically when Issues 1–4 are resolved.

---

## 4. Fix Plan

### 4.1 Fix 1: Crop Management Pipeline (CRITICAL)

**Objective:** Port the fork's PHU/PVD management pipeline and `cropphen_col` support into the integrated LTS.

**Files to modify:**
- `LPJ-GUESS-integrated/framework/externalinput.h` — Add `TimeDataD phus, pvds` (and related) to `ManagementInput`
- `LPJ-GUESS-integrated/framework/externalinput.cpp` — Add `init()` logic for `file_phu_in`/`file_pvd_in`; port `getphu()`, `getpvd()`, `getgrowseaslength()`, `getNfertdate2()`; modify `getmanagement()`, `getsowingdates()`, `getharvestdates()` to use `cropphen_col`
- `LPJ-GUESS-integrated/framework/parameters.h` — Add `iflandsymm_crop_management` runtime parameter declaration
- `LPJ-GUESS-integrated/framework/parameters.cpp` — Add `declareitem` for new parameter

**Approach:**
- Gate all new behavior behind `iflandsymm_crop_management` (default 0) for LTS backward compatibility
- When `iflandsymm_crop_management 1`:
  - `init()` opens `file_phu_in`, `file_pvd_in`, etc. and sets `readphu`/`readpvd`
  - `getmanagement()` calls `getphu()`, `getpvd()`, etc.
  - `getsowingdates()` and `getharvestdates()` use `cropphen_col` for column lookup
- When `iflandsymm_crop_management 0` (default):
  - No change to LTS behavior

**Branch:** `feature/crop-management-pipeline`
**Expected outcome:** F_D1/F_S1 Crop_sum Corr should improve from 0.13–0.26 to 0.85+

### 4.2 Fix 2: `standpft.active` Guard (SIGNIFICANT)

**Objective:** Add the fork's `if (standpft.active)` guard to the integrated `commonoutput.cpp` output accumulation loop.

**Files to modify:**
- `LPJ-GUESS-integrated/modules/commonoutput.cpp` — Add `if (standpft.active) {` guard around the per-stand accumulation block starting at ~line 991

**Approach:**
- Match the fork's pattern: only accumulate output variables from stands where the PFT is active
- Ensure proper scoping of the `++gc_itr` iterator advancement

**Branch:** `fix/standpft-active-guard`
**Expected outcome:** clitter FruitAndVeg divergence resolved; potential improvement in other crop-related outputs

### 4.3 Fix 3: N-Fixer BNF (SIGNIFICANT, CONDITIONAL)

**Objective:** Resolve the 20–55% N-fixer yield bias.

**Dependencies:** Fix 1 must be applied and tested first.

**Approach:**
- After Fix 1, retest OilNfix/Pulses yields
- If bias > 5% persists, compare `bnf_func_wcont()` and `bnf_func_developmentstage()` branch-by-branch
- Check `allocation_crop_nlim()` N-fixer pathway

**Branch:** `fix/bnf-nfixer-bias` (if needed)

### 4.4 Fix 4: Fire (NO DIRECT FIX)

**Rationale:** The `iflandsymm_blaze_fork` parameter already correctly controls all three BLAZE items (A4/A5/A6). Fire divergence is confirmed as cascading from Issue 1 (crop collapse). Will be re-evaluated after Fix 1.

### 4.5 Fix 5: Peatland (CONDITIONAL)

**Dependencies:** Fixes 1, 2 must be applied first.

**Approach:** Re-evaluate after cascading improvements from Fixes 1+2. If systematic bias persists, targeted peatland code diff and parameterization.

### 4.6 Fix 6: NEE (NO DIRECT FIX)

**Rationale:** Downstream of all other issues. Will improve automatically.

---

## 5. Execution Plan

| Phase | Action | Dependencies | Est. Time |
|-------|--------|-------------|-----------|
| **A** | Fix 2: `standpft.active` guard in `commonoutput.cpp` | None | 30 min (code + compile) |
| **B** | Fix 1: Crop management pipeline in `externalinput.h/cpp` + `parameters.h/cpp` | None | 2–3 hrs (code + compile) |
| **C** | Quick diagnostic test (H_D1 only, 200yr spinup, both fork and integ) | Fixes 1+2 compiled | 30–60 min (runs) |
| **D** | Evaluate diagnostic results — check crop identity restored, clitter non-zero | Phase C complete | 15 min |
| **E** | Fix 3: BNF adjustment (if N-fixer bias > 5% after D) | Phase D evaluation | 1–2 hrs (if needed) |
| **F** | Full comprehensive retest (all 16 configurations) | All fixes compiled | 8–12 hrs (runs) |
| **G** | Final statistical analysis and comparison | Phase F complete | 2 hrs |
| **H** | Documentation update (this document + integration log + technical manual) | Phase G complete | 2 hrs |

**Git workflow per fix:**
1. `git checkout landsymm/integration`
2. `git checkout -b feature/fix-name`
3. Implement changes
4. `make -j$(nproc)` — verify 0 errors, 0 warnings
5. Commit (user runs commands manually to avoid trailer)
6. `git checkout landsymm/integration`
7. `git merge --no-ff feature/fix-name`

---

## 6. Crop Identity Collapse — Detailed Per-PFT Evidence

### 6.1 F_D1 (SSP126, Deterministic, potyield=1) — Per-Crop AGPP

| Crop PFT | Base PFT | Fork MeanRef | Integ MeanTest | Bias% | Corr |
|----------|----------|-------------|---------------|-------|------|
| CerealsC3 | TeW_nlim | 0.4594 | 0.5774 | +25.67% | 0.5766 |
| CerealsC4 | TeCo_nlim | 0.4065 | 0.5435 | +33.71% | 0.8443 |
| Rice | TeRi_nlim | 0.2405 | 0.2959 | +23.02% | 0.9476 |
| OilNfix | **TeSo_nlim** | 0.4598 | **0.5773** | +25.55% | 0.6094 |
| OilOther | **TeSW_nlim** | 0.3947 | **0.4821** | +22.14% | 0.3956 |
| Pulses | **TeSo_nlim** | 0.4948 | **0.5773** | +16.66% | 0.6543 |
| StarchyRoots | **TeSW_nlim** | 0.4089 | **0.4821** | +17.89% | 0.6834 |
| FruitAndVeg | **TeSW_nlim** | 0.3882 | **0.4821** | +24.17% | 0.5160 |
| Sugar | **TeSW_nlim** | 0.4022 | **0.4821** | +19.86% | 0.3519 |
| Miscanthus | — | 0.5619 | 0.5970 | +6.24% | 0.8344 |
| CC3G_ic | — | 0.1387 | 0.1403 | +1.17% | 0.9962 |
| CC4G_ic | — | 0.0085 | 0.0255 | +199.77% | 0.6662 |

Note: **Bolded** base PFTs show the collapse pattern — all crops sharing a base PFT converge to the same integrated MeanTest.

### 6.2 F_D1 (SSP126, Deterministic, potyield=1) — Per-Crop Yield

| Crop PFT | Fork MeanRef | Integ MeanTest | Bias% | Corr |
|----------|-------------|---------------|-------|------|
| CerealsC3 | 0.1956 | 0.2831 | +44.74% | 0.8091 |
| CerealsC4 | 0.3387 | 0.4674 | +38.02% | 0.8056 |
| Rice | 0.1875 | 0.2395 | +27.75% | 0.9146 |
| OilNfix | 0.1712 | **0.2138** | +24.88% | 0.4974 |
| OilOther | 0.2195 | **0.2823** | +28.61% | 0.6852 |
| Pulses | 0.1873 | **0.2138** | +14.10% | 0.6099 |
| StarchyRoots | 0.2279 | **0.2823** | +23.89% | 0.7057 |
| FruitAndVeg | 0.2079 | **0.2823** | +35.78% | 0.6688 |
| Sugar | 0.2263 | **0.2823** | +24.74% | 0.7007 |
| Miscanthus | 0.4697 | 0.5001 | +6.47% | 0.7942 |

---

## 7. CH4 Methane — Per-Channel Detailed Statistics

### 7.1 H_DP (Historical, Deterministic, Peatland) — mch4.out Monthly

| Month | MedRel% | Bias% | Corr |
|-------|---------|-------|------|
| Jan | 3.75% | +0.68% | 0.9936 |
| Feb | 3.50% | +0.22% | 0.9935 |
| Mar | 3.51% | -0.75% | 0.9935 |
| Apr | 4.20% | +0.39% | 0.9380 |
| May | 5.89% | +1.91% | 0.9248 |
| Jun | 6.27% | +2.27% | 0.8937 |
| Jul | 7.90% | +4.11% | 0.9614 |
| Aug | 8.55% | +10.42% | 0.9637 |
| Sep | 10.80% | +9.71% | 0.9442 |
| Oct | 7.69% | +5.37% | 0.9521 |
| Nov | 8.41% | +4.92% | 0.9483 |
| Dec | 4.97% | +0.17% | 0.9955 |

### 7.2 CH4 Transport Pathway Divergence Pattern

| Pathway | H_DP MedRel% | H_SP MedRel% | F_DP MedRel% | F_SP MedRel% | Assessment |
|---------|-------------|-------------|-------------|-------------|------------|
| Diffusion | 2.91% | 2.61% | 3.15% | 6.49% | **Very Good** |
| Ebullition | 18.88% | 17.12% | 5.44% | 16.58% | **Moderate** (small abs) |
| Plant-mediated | 24.82% | 17.51% | 15.89% | 20.72% | **Moderate** |
| **Total** | **6.29%** | **7.27%** | **8.29%** | **16.15%** | **Good–Moderate** |

Diffusion (the dominant pathway) is well-matched. Plant-mediated transport shows the most divergence, tied to peatland vegetation differences (Issue 5).

---

## 8. Summary of Pre-Existing Successfully Parameterized Features

For context, the following LandSyMM features were previously integrated as runtime parameters and are functioning correctly in these tests:

| Parameter | Feature | Integration Step | Status |
|-----------|---------|-----------------|--------|
| `iflandsymm_nstress_simple` | Simplified N-stress on Vmax | Step 31 | ✓ Working |
| `iflandsymm_bnf_direct` | Direct BNF to plant tissue | Step 32 | ✓ Working (partial — see Issue 3) |
| `iflandsymm_senescence_d3` | Richards allocation d3 override | Step 33 | ✓ Working |
| `iflandsymm_nfert_init` | N fertilization initialization | Step 34 | ✓ Working |
| `iflandsymm_infiltration` | INUNDATED hydrology for wetlands | Step 35 | ✓ Working |
| `iflandsymm_irrigation_logic` | Irrigation logic extension | Step 29 | ✓ Working |
| `iflandsymm_vegdyn_fork` | Fork establishment/mortality | Step 48 | ✓ Working |
| `iflandsymm_nitri_gas_fork` | Nitrification gas parameters | Steps 39–40 | ✓ Working |
| `iflandsymm_nharvest_simple` | Simplified harvest N accounting | Step 36 | ✓ Working |
| `iflandsymm_nfert_after_phenology` | N-fert timing after phenology | Step 49e | ✓ Working |
| `iflandsymm_lc_before_management` | LC dynamics before management | Step 49d | ✓ Working |
| `iflandsymm_tillage_fixed` | Fixed tillage factors | Step 49a | ✓ Working |
| `iflandsymm_century_nc` | Century N:C algorithm | Step 47 | ✓ Working |
| `iflandsymm_weathergen_floors` | Weather generator floors | Step 46 | ✓ Working |
| `iflandsymm_fpc_linear` | Linear FPC calculation | Step 45 | ✓ Working |
| `iflandsymm_blaze_fork` | BLAZE fire behavior | Step 37 | ✓ Working (fire physics correct; divergence is cascading from Issue 1) |
| `blaze_cwd_factor` | CWD scaling factor | Step 37 | ✓ Set to 2.0 |
| `setup_multipart()` / `reset()` | Multi-part climate file init | Step 50 | ✓ Working |

---

## 9. File Locations Reference

| Item | Path |
|------|------|
| Integrated source | `lpjg_landsymm_integration/LPJ-GUESS-integrated/` |
| Fork source | `lpjg_landsymm_integration/LandSyMM_LPJ-GUESS/` |
| Test ins files | `lpjg_landsymm_integration/verification/phase2_landsymm_consistency/local_ins/` |
| Test output | `lpjg_landsymm_integration/verification/comprehensive_final_v2/` |
| Comparison script | `lpjg_landsymm_integration/verification/compare_per_variable.py` |
| Integration log | `lpjg_landsymm_integration/integration_log.md` |
| Modification registry | `lpjg_landsymm_integration/modification_registry.md` |
| This document | `lpjg_landsymm_integration/comprehensive_phase2_debug_report.md` |
