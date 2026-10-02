import subprocess
import json
import os
from pathlib import Path
from typing import Optional
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="QuantForge API")

# Enable CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Path to the CLI executable
CLI_PATH = Path(__file__).parent.parent.parent / "build" / "Release" / "quantforge.exe"
EXAMPLES_PATH = Path(__file__).parent.parent.parent / "examples"




def run_cli_command(args: list) -> dict:
    """Run the CLI executable and parse JSON output."""
    if not CLI_PATH.exists():
        raise HTTPException(status_code=500, detail="CLI executable not found. Build the project first.")
    
    try:
        result = subprocess.run(
            [str(CLI_PATH)] + args,
            capture_output=True,
            text=True,
            timeout=300,
        )
        
        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=f"CLI error: {result.stderr}")
        
        # Parse JSON output
        output_lines = result.stdout.strip().split('\n')
        json_line = output_lines[-1] if output_lines else result.stdout
        
        try:
            return json.loads(json_line)
        except json.JSONDecodeError:
            # Handle CSV output (benchmark)
            if "backend,total_ms" in result.stdout:
                lines = result.stdout.strip().split('\n')
                headers = lines[0].split(',')
                data = []
                for line in lines[1:]:
                    values = line.split(',')
                    data.append(dict(zip(headers, values)))
                return {"benchmarks": data}
            
            raise HTTPException(status_code=500, detail=f"Could not parse CLI output: {result.stdout}")
    
    except subprocess.TimeoutExpired:
        raise HTTPException(status_code=500, detail="CLI command timed out")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/")
async def root():
    return {"message": "QuantForge API", "version": "0.1.0"}


@app.get("/health")
async def health():
    return {"status": "healthy", "cli_exists": CLI_PATH.exists()}


@app.post("/api/risk")
async def calculate_risk(request: dict):
    """Calculate risk metrics (VaR, CVaR) for a portfolio."""
    # Save portfolio to temporary file
    temp_file = Path(__file__).parent / "temp_portfolio.csv"
    temp_file.write_text(request.portfolio_csv)
    
    try:
        args = [
            "risk",
            str(temp_file),
            "--metric", request.get("metric", "var"),
            "--confidence", str(request.get("confidence", 0.99)),
            "--simulations", str(request.get("simulations", 1000000))
        ]
        result = run_cli_command(args)
        return result
    finally:
        if temp_file.exists():
            temp_file.unlink()


@app.post("/api/benchmark")
async def benchmark(request: BenchmarkRequest):
    """Benchmark different backends (CPU, OpenMP, CUDA)."""
    temp_file = Path(__file__).parent / "temp_portfolio.csv"
    temp_file.write_text(request.portfolio_csv)
    
    try:
        args = [
            "benchmark",
            str(temp_file),
            "--simulations", str(request.get("simulations", 1000000))
        ]
        result = run_cli_command(args)
        return result
    finally:
        if temp_file.exists():
            temp_file.unlink()


@app.post("/api/stress")
async def stress_test(request: dict):
    """Run stress test scenarios."""
    temp_file = Path(__file__).parent / "temp_portfolio.csv"
    temp_file.write_text(request.portfolio_csv)
    
    try:
        args = [
            "stress",
            str(temp_file),
            "--scenario", request.get("scenario", "market-crash")
        ]
        result = run_cli_command(args)
        return result
    finally:
        if temp_file.exists():
            temp_file.unlink()


@app.get("/api/sample-portfolio")
async def get_sample_portfolio():
    """Get the sample portfolio CSV."""
    sample_path = EXAMPLES_PATH / "sample_portfolio.csv"
    if not sample_path.exists():
        raise HTTPException(status_code=404, detail="Sample portfolio not found")
    
    return {"csv": sample_path.read_text()}


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
