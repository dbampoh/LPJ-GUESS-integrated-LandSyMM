#!/usr/bin/env python3
"""Per-variable statistical comparison of LPJ-GUESS output files.
Breaks down multi-column files to individual variable statistics.
"""
import os, sys, numpy as np

dir_a = sys.argv[1]  # reference (fork or original LTS)
dir_b = sys.argv[2]  # test (integrated LTS)

print(f"{'File':25s} {'Variable':20s} {'N':>7s} {'MedRel%':>8s} {'P95Rel%':>9s} {'MeanAbsDiff':>12s} {'RMSE':>12s} {'NRMSE%':>7s} {'Bias':>12s} {'Bias%':>8s} {'MeanRef':>12s} {'MeanTest':>12s} {'Corr':>7s}")
print('-' * 195)

for fname in sorted(os.listdir(dir_a)):
    if not fname.endswith('.out'):
        continue
    fa = os.path.join(dir_a, fname)
    fb = os.path.join(dir_b, fname)
    if not os.path.exists(fb):
        continue

    with open(fa) as f1, open(fb) as f2:
        ha = f1.readline().split()
        hb = f2.readline().split()
        cols_a = ha[3:]
        cols_b = hb[3:]
        common = [c for c in cols_a if c in cols_b]
        idx_a = {c: i for i, c in enumerate(cols_a)}
        idx_b = {c: i for i, c in enumerate(cols_b)}

        col_data = {c: ([], []) for c in common}

        for la, lb in zip(f1, f2):
            sa = la.split()
            sb = lb.split()
            for cname in common:
                ia = 3 + idx_a[cname]
                ib = 3 + idx_b[cname]
                if ia < len(sa) and ib < len(sb):
                    try:
                        a = float(sa[ia])
                        b = float(sb[ib])
                        col_data[cname][0].append(a)
                        col_data[cname][1].append(b)
                    except ValueError:
                        pass

    for cname in common:
        vals_a, vals_b = col_data[cname]
        if not vals_a:
            continue
        aa = np.array(vals_a)
        bb = np.array(vals_b)
        n = len(aa)

        mask = np.abs(aa) > 1e-10
        if mask.any():
            rel = np.abs(aa[mask] - bb[mask]) / np.abs(aa[mask])
            med_rel = np.median(rel) * 100
            p95_rel = np.percentile(rel, 95) * 100
        else:
            med_rel = p95_rel = 0

        abs_diff = np.abs(aa - bb)
        mean_abs_diff = np.mean(abs_diff)
        rmse = np.sqrt(np.mean((aa - bb)**2))
        ma = np.mean(aa)
        mb = np.mean(bb)
        bias = mb - ma
        bias_pct = (bias / ma * 100) if abs(ma) > 1e-10 else 0
        rng = np.max(aa) - np.min(aa)
        nrmse = (rmse / rng * 100) if rng > 1e-10 else 0
        nonz = (np.abs(aa) > 1e-10) | (np.abs(bb) > 1e-10)
        if nonz.sum() > 1:
            try:
                corr = np.corrcoef(aa[nonz], bb[nonz])[0, 1]
            except:
                corr = 0
        else:
            corr = 0

        print(f"{fname:25s} {cname:20s} {n:7d} {med_rel:7.2f}% {p95_rel:8.2f}% {mean_abs_diff:11.4f} {rmse:11.4f} {nrmse:6.1f}% {bias:+11.4f} {bias_pct:+7.2f}% {ma:11.4f} {mb:11.4f} {corr:7.4f}")
