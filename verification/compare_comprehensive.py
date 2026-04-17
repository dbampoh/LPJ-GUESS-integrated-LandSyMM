#!/usr/bin/env python3
"""Comprehensive statistical comparison of LPJ-GUESS output files.
Provides per-file, per-gridcell, and per-year analysis with bias metrics.
"""
import os, sys, numpy as np
from collections import defaultdict

dir_a = sys.argv[1]  # reference (fork or original LTS)
dir_b = sys.argv[2]  # test (integrated LTS)
mode = sys.argv[3] if len(sys.argv) > 3 else "summary"  # summary, pergridcell, peryear

def parse_file(fa, fb):
    with open(fa) as f1, open(fb) as f2:
        ha = f1.readline().split()
        hb = f2.readline().split()
        cols_a = ha[3:]
        cols_b = hb[3:]
        common = [c for c in cols_a if c in cols_b]
        idx_a = {c: i for i, c in enumerate(cols_a)}
        idx_b = {c: i for i, c in enumerate(cols_b)}

        rows = []
        for la, lb in zip(f1, f2):
            sa = la.split()
            sb = lb.split()
            lon_a, lat_a, yr_a = float(sa[0]), float(sa[1]), int(sa[2])
            lon_b, lat_b, yr_b = float(sb[0]), float(sb[1]), int(sb[2])
            for cname in common:
                ia = 3 + idx_a[cname]
                ib = 3 + idx_b[cname]
                if ia < len(sa) and ib < len(sb):
                    try:
                        a = float(sa[ia])
                        b = float(sb[ib])
                        rows.append((lon_a, lat_a, yr_a, cname, a, b))
                    except ValueError:
                        pass
    return rows, common

for fname in sorted(os.listdir(dir_a)):
    if not fname.endswith('.out'):
        continue
    fa = os.path.join(dir_a, fname)
    fb = os.path.join(dir_b, fname)
    if not os.path.exists(fb):
        continue

    rows, common = parse_file(fa, fb)
    if not rows:
        continue

    all_a = np.array([r[4] for r in rows])
    all_b = np.array([r[5] for r in rows])

    if mode == "summary":
        n = len(all_a)
        ident = np.sum(np.array([rows[i][4] for i in range(n)]) == np.array([rows[i][5] for i in range(n)]))
        # Wait, string comparison for identity - let me redo
        # Actually use the raw approach
        mask = np.abs(all_a) > 1e-10
        if mask.any():
            rel = np.abs(all_a[mask] - all_b[mask]) / np.abs(all_a[mask])
            med_rel = np.median(rel) * 100
            p95_rel = np.percentile(rel, 95) * 100
            mean_rel = np.mean(rel) * 100
        else:
            med_rel = p95_rel = mean_rel = 0

        abs_diff = np.abs(all_a - all_b)
        mean_abs_diff = np.mean(abs_diff)
        max_abs_diff = np.max(abs_diff)
        rmse = np.sqrt(np.mean((all_a - all_b)**2))
        mean_a = np.mean(all_a)
        mean_b = np.mean(all_b)
        bias = mean_b - mean_a
        bias_pct = (bias / mean_a * 100) if abs(mean_a) > 1e-10 else 0
        nonz = (np.abs(all_a) > 1e-10) | (np.abs(all_b) > 1e-10)
        if nonz.sum() > 1:
            try:
                corr = np.corrcoef(all_a[nonz], all_b[nonz])[0, 1]
            except:
                corr = 0
        else:
            corr = 0
        nrmse = rmse / (np.max(all_a) - np.min(all_a)) * 100 if (np.max(all_a) - np.min(all_a)) > 1e-10 else 0

        print(f"{fname:25s}  N={n:6d}  MedRel={med_rel:6.2f}%  P95Rel={p95_rel:7.2f}%  "
              f"MeanAbsDiff={mean_abs_diff:10.4f}  RMSE={rmse:10.4f}  NRMSE={nrmse:5.1f}%  "
              f"Bias={bias:+10.4f} ({bias_pct:+6.2f}%)  "
              f"MeanRef={mean_a:10.4f}  MeanTest={mean_b:10.4f}  Corr={corr:.4f}")

    elif mode == "pergridcell":
        gc_data = defaultdict(lambda: ([], []))
        for lon, lat, yr, col, a, b in rows:
            gc_data[(lon, lat)][0].append(a)
            gc_data[(lon, lat)][1].append(b)

        print(f"\n=== {fname} — Per-Gridcell Analysis ===")
        print(f"{'Lon':>8s} {'Lat':>7s}  {'N':>5s}  {'MedRel%':>8s}  {'Bias%':>8s}  {'RMSE':>10s}  {'Corr':>7s}  {'MeanRef':>10s}  {'MeanTest':>10s}")
        for (lon, lat), (vals_a, vals_b) in sorted(gc_data.items()):
            aa = np.array(vals_a)
            bb = np.array(vals_b)
            n = len(aa)
            mask = np.abs(aa) > 1e-10
            med_rel = np.median(np.abs(aa[mask] - bb[mask]) / np.abs(aa[mask])) * 100 if mask.any() else 0
            rmse = np.sqrt(np.mean((aa - bb)**2))
            ma = np.mean(aa)
            mb = np.mean(bb)
            bias_pct = ((mb - ma) / ma * 100) if abs(ma) > 1e-10 else 0
            nonz = (np.abs(aa) > 1e-10) | (np.abs(bb) > 1e-10)
            corr = np.corrcoef(aa[nonz], bb[nonz])[0, 1] if nonz.sum() > 1 else 0
            print(f"{lon:8.2f} {lat:7.2f}  {n:5d}  {med_rel:7.2f}%  {bias_pct:+7.2f}%  {rmse:10.4f}  {corr:7.4f}  {ma:10.4f}  {mb:10.4f}")

    elif mode == "peryear":
        yr_data = defaultdict(lambda: ([], []))
        for lon, lat, yr, col, a, b in rows:
            yr_data[yr][0].append(a)
            yr_data[yr][1].append(b)

        print(f"\n=== {fname} — Per-Year Analysis ===")
        print(f"{'Year':>6s}  {'N':>5s}  {'MedRel%':>8s}  {'Bias%':>8s}  {'RMSE':>10s}  {'Corr':>7s}  {'MeanRef':>10s}  {'MeanTest':>10s}")
        for yr in sorted(yr_data.keys()):
            vals_a, vals_b = yr_data[yr]
            aa = np.array(vals_a)
            bb = np.array(vals_b)
            n = len(aa)
            mask = np.abs(aa) > 1e-10
            med_rel = np.median(np.abs(aa[mask] - bb[mask]) / np.abs(aa[mask])) * 100 if mask.any() else 0
            rmse = np.sqrt(np.mean((aa - bb)**2))
            ma = np.mean(aa)
            mb = np.mean(bb)
            bias_pct = ((mb - ma) / ma * 100) if abs(ma) > 1e-10 else 0
            nonz = (np.abs(aa) > 1e-10) | (np.abs(bb) > 1e-10)
            corr = np.corrcoef(aa[nonz], bb[nonz])[0, 1] if nonz.sum() > 1 else 0
            print(f"{yr:6d}  {n:5d}  {med_rel:7.2f}%  {bias_pct:+7.2f}%  {rmse:10.4f}  {corr:7.4f}  {ma:10.4f}  {mb:10.4f}")
