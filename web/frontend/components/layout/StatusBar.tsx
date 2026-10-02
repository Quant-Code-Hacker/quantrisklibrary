"use client";

import { Cpu, Cpu as GpuIcon, Activity, Clock, Zap } from 'lucide-react';
import { cn } from '@/lib/utils';

export default function StatusBar() {
  return (
    <div className="h-12 bg-surface border-b border-border flex items-center justify-between px-4">
      {/* Left: Engine Status */}
      <div className="flex items-center gap-6">
        <div className="flex items-center gap-2">
          <div className="w-2 h-2 bg-accent-success rounded-full animate-pulse-subtle" />
          <span className="text-sm font-medium">ENGINE ONLINE</span>
        </div>
        
        <div className="flex items-center gap-2 text-xs text-foreground-subtle">
          <span>Backend:</span>
          <span className="text-accent-primary font-mono">CUDA</span>
        </div>

        <div className="flex items-center gap-2 text-xs text-foreground-subtle">
          <GpuIcon className="w-3.5 h-3.5 text-gpu-color" />
          <span className="font-mono">RTX 4090</span>
          <span className="text-foreground-muted">24GB</span>
        </div>
      </div>

      {/* Center: Simulation Status */}
      <div className="flex items-center gap-4">
        <div className="flex items-center gap-2 text-xs">
          <Activity className="w-3.5 h-3.5 text-accent-warning" />
          <span className="text-foreground-muted">Simulation:</span>
          <span className="text-accent-success font-mono">IDLE</span>
        </div>
      </div>

      {/* Right: System Metrics */}
      <div className="flex items-center gap-6 text-xs">
        <div className="flex items-center gap-2">
          <Cpu className="w-3.5 h-3.5 text-cpu-color" />
          <span className="text-foreground-muted">CPU:</span>
          <span className="font-mono">23%</span>
        </div>
        
        <div className="flex items-center gap-2">
          <Zap className="w-3.5 h-3.5 text-omp-color" />
          <span className="text-foreground-muted">GPU:</span>
          <span className="font-mono">0%</span>
        </div>

        <div className="flex items-center gap-2">
          <Clock className="w-3.5 h-3.5 text-foreground-subtle" />
          <span className="font-mono text-foreground-muted">
            {new Date().toLocaleTimeString()}
          </span>
        </div>
      </div>
    </div>
  );
}
