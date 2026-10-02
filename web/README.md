# QuantForge Web UI

A modern web interface for the QuantForge financial risk computing engine.

## Prerequisites

- Python 3.8+
- Node.js 18+
- The QuantForge CLI must be built (see parent directory README)

## Setup

### Backend (FastAPI)

```powershell
cd web/backend
pip install -r requirements.txt
python main.py
```

The API will run on http://localhost:8000

### Frontend (Next.js)

```powershell
cd web/frontend
npm install
npm run dev
```

The UI will run on http://localhost:3000

## Features

- **Risk Calculator**: Calculate VaR and CVaR metrics with customizable confidence levels and simulation counts
- **Benchmark**: Compare performance across CPU, OpenMP, and CUDA backends with visual charts
- **Stress Testing**: Run stress scenarios (market crash, correction, volatility spike) to test portfolio resilience

## API Endpoints

- `POST /api/risk` - Calculate risk metrics
- `POST /api/benchmark` - Run backend benchmarks
- `POST /api/stress` - Run stress tests
- `GET /api/sample-portfolio` - Get sample portfolio CSV
- `GET /health` - Health check

## Architecture

- **Backend**: FastAPI wrapping the CLI executable
- **Frontend**: Next.js 14 with React, TailwindCSS, and Recharts
- **Communication**: REST API with JSON responses
