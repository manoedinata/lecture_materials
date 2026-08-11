#!/bin/bash
# ─────────────────────────────────────────────────
#  Public Transport Planner — Build & Run Script
#  Requires: JDK (javac + java) installed
# ─────────────────────────────────────────────────

set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC="$SCRIPT_DIR/src"
OUT="$SCRIPT_DIR/out"

echo " Compiling..."
mkdir -p "$OUT"

javac -d "$OUT" \
  "$SRC/model/Station.java" \
  "$SRC/model/Route.java" \
  "$SRC/model/RouteResult.java" \
  "$SRC/graph/Graph.java" \
  "$SRC/tree/MinHeap.java" \
  "$SRC/graph/SimpleQueue.java" \
  "$SRC/tree/Trie.java" \
  "$SRC/graph/Dijkstra.java" \
  "$SRC/graph/BFS.java" \
  "$SRC/DataLoader.java" \
  "$SRC/tree/ApiServer.java" \
  "$SRC/Main.java"

echo " Compilation successful!"
echo ""
echo " Starting Public Transport Planner..."
echo ""

java -cp "$OUT" Main
