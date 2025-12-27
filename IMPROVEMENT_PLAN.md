# Water Cycle System - Improvement Plan

**Project**: Brussels Sewer Museum - Water Cycle Controller
**Date**: 2025-12-27
**Current Status**: Production system, 6 years operational, reliable
**Goal**: Modernize valve control while maintaining reliability

---

## Problem Statement

Current system works but has maintainability issues:
- Wago connectors and loose wiring = hard to troubleshoot
- 8-channel relay failure = replace entire module
- Flyback diodes spliced into cables = messy
- Bulky brass manifold with large valves - difficult to expand/modify

---

## Proposed Solution: Smart I2C Valve Modules

**Concept**: Replace brass manifold with distributed smart modules
- Each valve gets its own intelligent control module
- Digispark ATtiny85 per valve (I2C addressable, USB programmable)
- Integrated MOSFET driver + flyback protection + per-module fuse
- Status LEDs on each module
- 4-wire bus replaces 20+ wires (12V, GND, SDA, SCL)
- One PCB design fits all roles (injection or pass-through via solder jumper)

**Technical Specification**: See `docs/VALVE_MODULE_PCB_v1.0.md` for complete circuit design

**Benefits**:
- ✅ Compact valves eliminate bulky manifold, easier placement
- ✅ Lower power consumption with 2V025-08 valves (optional upgrade)
- ✅ Reduce wiring from 20+ wires to 4-wire bus
- ✅ Individual fault isolation (one module fails, others work)
- ✅ Visual diagnostics (LED status per module)
- ✅ Easy programming (USB, no external programmer)
- ✅ Addressable like WS2812 LEDs
- ✅ Clean, professional appearance
- ✅ Future expansion easy (add/remove valves, add sensors)

---

## Deployment Scenarios

### Scenario A: Retrofit Current System

**Use existing 2W025-08 valves** (13-20W / 1-1.6A each)

**Changes**:
- Remove relay modules (MCP1 + ULN2803)
- Install 12x valve modules with injection every 2 modules
- Connect to existing shield via A4/A5 headers (I2C) + 12V/GND

**Power distribution**:
- 6 injection points: Modules 1, 3, 5, 7, 9, 11
- PSU terminals via Wago splitters + per-module fuses (3A)

**Code changes**:
- Disable MCP1 initialization
- Replace `mcp1.digitalWrite()` with `setValve()` I2C calls

**Advantages**: Immediate upgrade, reuse existing valves, test system with known hardware

---

### Scenario B: New System with Compact Valves

**Upgrade to 2V025-08 valves** (3-12W / 0.25-1A each)

**Changes**:
- Replace all valves with compact 2V025-08 (G1/4" DN08)
- Install 12x valve modules with injection every 4 modules
- Optional: Design new shield v1.3 with test points, status LEDs

**Power distribution**:
- 3 injection points: Modules 1, 5, 9 (one per PSU terminal)
- Lower power draw allows fewer injection points

**Valve comparison**:
| Model | Power | Current | Notes |
|-------|-------|---------|-------|
| 2W025-08 (current) | 13-20W | 1-1.6A | Bulky, higher power |
| 2V025-08 (upgrade) | 3-12W | 0.25-1A | Compact (~39mm), 50-70% lower power |

**Advantages**: Lower power, more compact, cleaner installation, modernized system

---

## Phase 1: Prototype (Start Here)

**Goal**: Build and validate 2-3 working modules

**Tasks**:
1. Design PCB (see `docs/VALVE_MODULE_PCB_v1.0.md`)
2. Order components (see Budget Table below)
3. Write I2C firmware for ATtiny85
4. Bench test (I2C communication, valve switching, power distribution)
5. Museum field test (1-2 modules for 1 month)

**Success Criteria**:
- [ ] I2C communication reliable across 12 modules
- [ ] Valves switch consistently
- [ ] No interference between modules
- [ ] Easy to program and debug
- [ ] Physical mounting works
- [ ] Per-module fusing protects against faults
- [ ] Museum staff approves

**If successful**: Proceed to Phase 2 (full deployment)
**If issues**: Iterate or fall back to traditional relay upgrade

---

## Phase 2: Full Deployment (Future)

**Only after Phase 1 validates successfully**

**Choose deployment scenario**:
- **Scenario A**: Retrofit with existing valves (faster, lower cost)
- **Scenario B**: Full upgrade with new valves (cleaner, lower power)

**Steps**:
1. Finalize PCB design (incorporate prototype lessons)
2. Order 12x production modules
3. Optional: Design Shield v1.3 with improvements
4. Schedule museum downtime
5. Install complete system
6. Update main controller code
7. Test all 5 scenarios (S1-S5)

---

## Connection to Main Board

### Option 1: Use Existing Shield (Retrofit)

**Existing shield has breakout headers** - perfect for I2C connection!

**Connections**:
- SDA → Nano A4 header
- SCL → Nano A5 header
- 12V → Top screw terminals
- GND → Any GND point

**Wiring**:
```
Shield → 4-wire harness → First module J1 → JST daisy-chain → All modules
```

**Code**: Disable MCP1, add I2C valve control functions

---

### Option 2: New Shield v1.3 (Future Upgrade)

**Improvements** (see Phase 4 in original plan):
- Test points (5V, 12V, GND, I2C, key signals)
- Power indicator LEDs (5V, 12V)
- I2C bus activity LED
- Dedicated I2C header for valve modules
- Fix home LED dimmer circuit (PWM control)
- Spare GPIO breakout

**Design when**: Full system upgrade (Scenario B) or if prototype reveals issues

---

## Budget Table

**To be completed with real vendor prices and links**

### Prototype (3 modules)
| Item | Qty | Unit Cost | Total | Vendor Link |
|------|-----|-----------|-------|-------------|
| Digispark ATtiny85 | 3 | | | |
| Test valves (2V025-08) | 3 | | | |
| PCB fabrication (5pcs) | 1 | | | JLCPCB |
| LCSC components (bulk) | 1 | | | |
| JST cables | 5 | | | |
| Wire (18AWG, 22AWG) | 5m | | | |
| Wago connectors 3P | 2 | | | |
| Fuses + holders | 10 | | | |
| **Prototype Total** | | | **~€55** | |

### Full System (12 modules)
| Item | Qty | Unit Cost | Total | Vendor Link |
|------|-----|-----------|-------|-------------|
| Digispark ATtiny85 | 12 | | | |
| Valves (choose scenario) | 12 | | | |
| PCB production (12pcs) | 1 | | | |
| LCSC components | 1 | | | |
| Complete wiring kit | 1 | | | |
| **Full System Total** | | | **€180-250** | |

---

## Immediate Actions

1. **Finalize PCB design** (`docs/VALVE_MODULE_PCB_v1.0.md`)
   - Complete schematic in EasyEDA
   - Layout PCB (50x40mm estimated)
   - Generate Gerber files

2. **Order prototype components**
   - Complete Budget Table with vendor links
   - Get approval for ~€55 prototype budget
   - Place orders (1 week delivery)

3. **Write firmware skeleton**
   - I2C slave library for ATtiny85
   - State machine for valve control
   - LED status indicators

**Estimated time**: 2-3 weeks to prototype completion

---

## Fallback Plan

If smart modules don't work out:
- Test valves can be used with traditional relay modules
- Prototype components (Digispark, etc.) can be repurposed
- Total loss: ~€55 (acceptable R&D cost)
- Fall back to custom relay PCB (Option C from original plan)

---

## Next Steps

**Awaiting Decision**:
- [ ] Choose deployment scenario (A: retrofit vs B: new valves)
- [ ] Approve prototype budget (~€55)
- [ ] Confirm museum access for field testing
- [ ] Timeline flexibility (3-6 months total)

**Once approved**:
1. Complete PCB design
2. Order components
3. Develop firmware
4. Build and test prototypes
