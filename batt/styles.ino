const char STYLES_CSS[] PROGMEM = R"rawliteral(

*,
*::before,
*::after {
  box-sizing: border-box;
}

html,
body {
  height: 100%;
}

html {
  font-size: 100%;
}

body {
  min-height: 100%;
  margin: 0;
  font-family: system-ui, -apple-system, Segoe UI, Roboto, Arial, sans-serif;
  color: #111827;
  -webkit-text-size-adjust: 100%;
}

.bg {
  background: #f3f4f6;
}

.wrap {
  min-height: 100%;
  min-height: 100dvh;
  display: grid;
  place-items: center;
  padding: clamp(1rem, 4vw, 1.5rem);
}

.card {
  width: min(100%, 35rem);
  background: #fff;
  padding:
    clamp(1.125rem, 4.5vw, 1.75rem)
    clamp(1.125rem, 4.5vw, 1.75rem)
    clamp(1rem, 4vw, 1.125rem);
  border-radius: clamp(1rem, 4vw, 1.375rem);
  box-shadow: 0 0.625rem 1.5625rem rgba(0, 0, 0, .08);
  border: 0.0625rem solid rgba(0, 0, 0, .05);
}

.card__head {
  text-align: center;
  margin-bottom: clamp(0.5rem, 2.5vw, 0.75rem);
}

.title {
  font-size: clamp(1.25rem, 5vw, 1.5rem);
  font-weight: 700;
  margin: 0 0 0.25rem;
  line-height: 1.2;
}

.subtitle {
  font-size: clamp(1rem, 3.8vw, 1.125rem);
  font-weight: 600;
  margin: clamp(0.5rem, 3vw, 0.75rem) 0 clamp(0.25rem, 1.5vw, 0.375rem);
  text-align: center;
}

.muted {
  color: #6b7280;
  margin: 0;
}

.tiny {
  color: #6b7280;
  font-size: clamp(0.6875rem, 2.8vw, 0.75rem);
  margin: clamp(0.25rem, 1.5vw, 0.375rem) 0 0;
  text-align: center;
}

.micro {
  color: #9ca3af;
  font-size: clamp(0.625rem, 2.5vw, 0.6875rem);
  margin: 0.25rem 0 0;
  text-align: center;
}

.reading {
  background: #f9fafb;
  padding: clamp(0.875rem, 4vw, 1.25rem);
  border-radius: clamp(0.75rem, 3.5vw, 0.875rem);
  text-align: center;
}

.distance {
  font-size: clamp(2.5rem, 12vw, 3.5rem);
  font-weight: 800;
  letter-spacing: -0.03em;
  margin: clamp(0.25rem, 1.5vw, 0.375rem) 0;
  color: #1f2937;
  line-height: 1.05;
  overflow-wrap: anywhere;
}

.distance .unit {
  font-size: clamp(1.125rem, 6vw, 1.625rem);
  font-weight: 700;
  color: #374151;
}

.badge {
  display: inline-block;
  padding:
    clamp(0.375rem, 2vw, 0.5rem)
    clamp(0.625rem, 3vw, 0.875rem);
  border-radius: 999rem;
  font-weight: 700;
  color: #fff;
  transition: background-color .25s;
  max-width: 100%;
  overflow-wrap: anywhere;
}

.badge--gray {
  background: #9ca3af;
}

.badge--green {
  background: #10b981;
}

.badge--orange {
  background: #f59e0b;
}

.badge--red {
  background: #ef4444;
}

.sep {
  border: 0;
  height: 0.0625rem;
  background: rgba(0, 0, 0, .08);
  margin:
    clamp(0.75rem, 3vw, 1rem)
    0
    clamp(0.875rem, 3.5vw, 1.125rem);
}

.control {
  margin:
    clamp(0.75rem, 3vw, 0.875rem)
    0
    clamp(0.875rem, 3.5vw, 1.125rem);
}

.row {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 0.75rem;
  color: #4b5563;
  font-weight: 600;
  font-size: clamp(0.875rem, 3.5vw, 1rem);
}

.value {
  color: #2563eb;
  font-weight: 800;
}

input[type=range] {
  -webkit-appearance: none;
  appearance: none;
  width: 100%;
  height: clamp(0.375rem, 2vw, 0.5rem);
  border-radius: 0.375rem;
  background: #e5e7eb;
  outline: none;
  opacity: .96;
  transition: opacity .2s, background-color .2s;
}

input[type=range]:hover {
  opacity: 1;
}

input[type=range]::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: clamp(1rem, 5vw, 1.125rem);
  height: clamp(1rem, 5vw, 1.125rem);
  border-radius: 999rem;
  background: #3b82f6;
  box-shadow: 0 0.0625rem 0.125rem rgba(0, 0, 0, .15);
  cursor: pointer;
}

input[type=range]::-moz-range-thumb {
  width: clamp(1rem, 5vw, 1.125rem);
  height: clamp(1rem, 5vw, 1.125rem);
  border-radius: 999rem;
  background: #3b82f6;
  box-shadow: 0 0.0625rem 0.125rem rgba(0, 0, 0, .15);
  cursor: pointer;
}

)rawliteral";
