#!/bin/bash
# setup_paths.sh — Replace <DATA_ROOT> placeholders with actual data paths
#
# Usage:
#   ./setup_paths.sh /path/to/your/data/root [/path/to/lu/root]
#
# Arguments:
#   $1  DATA_ROOT     — Root directory for climate, ndep, soil, CO2, popdens data
#                       On KIT IMK-IFU Simba2: /bg/data/lpj/LPJ-GUESS/input
#   $2  LU_ROOT       — Root directory for land-use fraction files (optional)
#                       On KIT IMK-IFU Simba2: /bg/data/lpj/$USER/landsymm_lu
#
# This script modifies ins files IN PLACE. Run it once after cloning the repo.
# To reset: git checkout -- data/landsymm-integrated-ins/

set -euo pipefail

if [ $# -lt 1 ]; then
    echo "Usage: $0 <DATA_ROOT> [<LU_ROOT>]"
    echo ""
    echo "  DATA_ROOT: Path to climate/ndep/soil/CO2/popdens input data"
    echo "  LU_ROOT:   Path to land-use fraction files (optional)"
    echo ""
    echo "KIT IMK-IFU Simba2 example:"
    echo "  $0 /bg/data/lpj/LPJ-GUESS/input /bg/data/lpj/\$USER/landsymm_lu"
    exit 1
fi

DATA_ROOT="$1"
LU_ROOT="${2:-<LU_ROOT>}"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Substituting paths in ins files..."
echo "  DATA_ROOT = $DATA_ROOT"
echo "  LU_ROOT   = $LU_ROOT"

for f in "$SCRIPT_DIR"/*.ins "$SCRIPT_DIR"/ssp126-overrides/*.ins; do
    if [ -f "$f" ]; then
        sed -i "s|<DATA_ROOT>|${DATA_ROOT}|g" "$f"
        sed -i "s|<LU_ROOT>|${LU_ROOT}|g" "$f"
        sed -i "s|<USER>|${USER}|g" "$f"
        echo "  Updated: $(basename "$f")"
    fi
done

echo "Done. Verify paths in main.ins before running."
