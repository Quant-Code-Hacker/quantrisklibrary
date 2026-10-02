import type { Config } from "tailwindcss";

const config: Config = {
  content: [
    "./pages/**/*.{js,ts,jsx,tsx,mdx}",
    "./components/**/*.{js,ts,jsx,tsx,mdx}",
    "./app/**/*.{js,ts,jsx,tsx,mdx}",
  ],
  theme: {
    extend: {
      colors: {
        background: "var(--background)",
        surface: "var(--surface)",
        'surface-elevated': "var(--surface-elevated)",
        'surface-hover': "var(--surface-hover)",
        border: "var(--border)",
        'border-subtle': "var(--border-subtle)",
        foreground: "var(--foreground)",
        'foreground-muted': "var(--foreground-muted)",
        'foreground-subtle': "var(--foreground-subtle)",
        'accent-primary': "var(--accent-primary)",
        'accent-primary-dim': "var(--accent-primary-dim)",
        'accent-secondary': "var(--accent-secondary)",
        'accent-success': "var(--accent-success)",
        'accent-warning': "var(--accent-warning)",
        'accent-danger': "var(--accent-danger)",
        'gpu-color': "var(--gpu-color)",
        'cpu-color': "var(--cpu-color)",
        'omp-color': "var(--omp-color)",
      },
      fontFamily: {
        sans: ["var(--font-sans)", "sans-serif"],
        mono: ["var(--font-mono)", "monospace"],
      },
      animation: {
        'pulse-subtle': 'pulse-subtle 2s cubic-bezier(0.4, 0, 0.6, 1) infinite',
      },
    },
  },
  plugins: [],
};
export default config;
