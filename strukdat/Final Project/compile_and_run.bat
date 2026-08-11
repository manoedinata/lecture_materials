@echo off
REM ─────────────────────────────────────────────────
REM  Public Transport Planner — Windows Build Script
REM  Requires: JDK installed, javac in PATH
REM ─────────────────────────────────────────────────

set SRC=%~dp0src
set OUT=%~dp0out

echo Compiling...
if not exist "%OUT%" mkdir "%OUT%"

javac -d "%OUT%" ^
  "%SRC%\data\Station.java" ^
  "%SRC%\data\Route.java" ^
  "%SRC%\data\RouteResult.java" ^
  "%SRC%\data\Graph.java" ^
  "%SRC%\structures\MinHeap.java" ^
  "%SRC%\structures\SimpleQueue.java" ^
  "%SRC%\structures\Trie.java" ^
  "%SRC%\algorithms\Dijkstra.java" ^
  "%SRC%\algorithms\BFS.java" ^
  "%SRC%\app\DataLoader.java" ^
  "%SRC%\app\TransportApp.java"

echo Compilation done!
echo.
echo Starting Public Transport Planner...
echo.

java -cp "%OUT%" app.TransportApp
pause
