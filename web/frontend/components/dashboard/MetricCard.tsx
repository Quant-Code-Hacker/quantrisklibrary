"use client";

import { motion } from 'framer-motion';
import { ArrowUpRight, ArrowDownRight, Minus } from 'lucide-react';
import { cn } from '@/lib/utils';
import { formatCurrency, formatPercent, formatNumber } from '@/lib/utils';

interface MetricCardProps {
  label: string;
  value: number | string;
  change?: number;
  format?: 'currency' | 'percent' | 'number' | 'duration';
  trend?: 'up' | 'down' | 'neutral';
  clickable?: boolean;
  onClick?: () => void;
  accent?: 'primary' | 'success' | 'warning' | 'danger' | 'gpu' | 'cpu' | 'omp';
}

export default function MetricCard({
  label,
  value,
  change,
  format = 'number',
  trend = 'neutral',
  clickable = false,
  onClick,
  accent,
}: MetricCardProps) {
  const formatValue = (val: number | string) => {
    if (typeof val === 'string') return val;
    switch (format) {
      case 'currency':
        return formatCurrency(val);
      case 'percent':
        return formatPercent(val);
      case 'number':
        return formatNumber(val);
      case 'duration':
        return `${val.toFixed(2)}ms`;
      default:
        return val.toString();
    }
  };

  const accentColors = {
    primary: 'border-accent-primary/30 hover:border-accent-primary/50',
    success: 'border-accent-success/30 hover:border-accent-success/50',
    warning: 'border-accent-warning/30 hover:border-accent-warning/50',
    danger: 'border-accent-danger/30 hover:border-accent-danger/50',
    gpu: 'border-gpu-color/30 hover:border-gpu-color/50',
    cpu: 'border-cpu-color/30 hover:border-cpu-color/50',
    omp: 'border-omp-color/30 hover:border-omp-color/50',
  };

  const TrendIcon = trend === 'up' ? ArrowUpRight : trend === 'down' ? ArrowDownRight : Minus;
  const trendColor = trend === 'up' ? 'text-accent-success' : trend === 'down' ? 'text-accent-danger' : 'text-foreground-subtle';

  return (
    <motion.div
      whileHover={clickable ? { scale: 1.02 } : {}}
      whileTap={clickable ? { scale: 0.98 } : {}}
      onClick={onClick}
      className={cn(
        "relative p-4 bg-surface border rounded-lg transition-all",
        accentColors[accent || 'primary'],
        clickable && "cursor-pointer hover:bg-surface-hover",
        "group"
      )}
    >
      <div className="flex items-start justify-between">
        <div className="flex-1">
          <p className="text-xs text-foreground-muted uppercase tracking-wider mb-1">{label}</p>
          <p className="text-2xl font-bold font-mono-numbers text-foreground">
            {formatValue(value)}
          </p>
        </div>
        
        {change !== undefined && (
          <div className={cn("flex items-center gap-1 text-xs", trendColor)}>
            <TrendIcon className="w-3.5 h-3.5" />
            <span className="font-mono-numbers">{formatPercent(Math.abs(change))}</span>
          </div>
        )}
      </div>

      {clickable && (
        <div className="absolute inset-0 border-2 border-transparent group-hover:border-accent-primary/20 rounded-lg transition-colors pointer-events-none" />
      )}
    </motion.div>
  );
}
