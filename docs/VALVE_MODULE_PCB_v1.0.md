# Smart Valve Module - PCB Design Specification v1.0

**Design for**: Digispark ATtiny85 carrier board with valve driver circuit

---

## PCB: 50x40mm, 2-layer, 4x M3 mounting holes

## Circuit

```
Power routing (with jumper and fuse):
                ┌──── [JP1 solder jumper] ────┐
                │                             │
J1 12V (IN) ────┴─────────┬───────────────────┴──→ 12V rail ──→ J2 12V (OUT)
                          │
J4 12V (inject) ──[F1 3A]──[D2]────┘

Mode selection:
- PASS-THROUGH (default): JP1 CLOSED - power flows J1→12V rail (fuse bypassed)
- INJECTION: JP1 OPEN - power flows J4→F1→D2→12V rail (fuse protects this module)

Rest of circuit:
12V rail ──┬──→ Digispark VIN
           └──→ MOSFET drain

Digispark P1 ──[1K]──→ MOSFET gate
MOSFET source ──→ Valve+ ──[1N4007 flyback to GND]──→ Valve- ──→ GND

I2C (daisy-chained through J1/J2):
Digispark P0 (SDA) ←── J1/J2 SDA [pull-ups on main Arduino board]
Digispark P2 (SCL) ←── J1/J2 SCL [pull-ups on main Arduino board]

Status LEDs:
Digispark P3 ──[1K]──→ Green LED ──→ GND (power indicator)
Digispark P4 ──[1K]──→ Yellow LED ──→ GND (valve active)
```

## Components (One Design Fits All)

| Ref | Part | Notes |
|-----|------|-------|
| U1 | Digispark ATtiny85 | Female header 1x8, USB left |
| Q1 | IRLZ44N TO-220 | Logic-level MOSFET |
| D1 | 1N4007 | Flyback across valve |
| D2 | Schottky SB560 DO-201AD | Power injection isolation (always populate) |
| D3, D4 | 5mm LED green, yellow | Status indicators |
| F1 | Fuse holder 5x20mm | 3A fast-blow glass fuse (populate on injection modules) |
| R1-R3 | 1K | Gate + LED resistors |
| J1, J2 | JST XH 4P right-angle | Power + signal daisy-chain |
| J3 | Screw term 2P 5mm | Valve connection |
| J4 | Screw term 2P 5mm | Power injection input |
| JP1 | Solder jumper 2-pad | Mode select |

**Notes**:
- I2C pull-ups (4.7K) are on main Arduino board, not on modules
- F1 + J4 only needed on injection modules, but footprints on all PCBs for flexibility

## BOM Cost (per module)
- Base components: €4.10
- Fuse holder + 3A fuse (injection modules only): +€0.50
- Total per pass-through module: €4.10
- Total per injection module: €4.60
- PCB: €10 for 5pcs (JLCPCB)

**Complete BOM with LCSC part numbers**:
| Component | LCSC # | Cost | Notes |
|-----------|--------|------|-------|
| Digispark ATtiny85 | - | €3.00 | AliExpress |
| IRLZ44N | C2586 | €0.40 | |
| 1N4007 | C106926 | €0.03 | |
| SB560 Schottky | C22452 | €0.15 | |
| 5mm LED green/yellow | C72033/C72038 | €0.15 | |
| Fuse holder 5x20mm PCB | C709153 | €0.30 | |
| Fuse 3A fast-blow 5x20mm | C369959 | €0.20 | |
| 1K resistor | C176197 | €0.03 | |
| JST XH 4P R/A | C144395 | €0.24 | ×2 |
| Screw term 2P 5mm | C474881 | €0.12 | ×2 |
| Female header 1x8 | C124375 | €0.10 | |

## Power Distribution (LED Strip Approach)

**Power Budget**:
- Valve draw: 0.25-1A @ 12V (3-12W, depends on pressure)
- Low pressure aquarium pump → expect ~0.25A typical
- Digispark: ~50mA

**JST XH Rating**: 3A max

**Design Strategy**: Like WS2812 LED strips - daisy-chain everything, inject power every 3-4 modules

```
PSU Term 1 ───[18AWG]───→ Module 1 [INJECTION]
                          ↓ JST (4-pin: 12V/GND/SDA/SCL)
                          Module 2
                          ↓ JST
                          Module 3
                          ↓ JST
PSU Term 2 ───[18AWG]───→ Module 4 [INJECTION]
                          ↓ JST
                          Module 5
                          ↓ JST
                          ...
```

**Current per JST segment** (worst case, all valves ON):
- 4 modules × 1A = 4A peak (slightly over 3A rating)
- 4 modules × 0.25A = 1A typical (well under rating)

**One PCB Design for All Modules**:
- **J1**: JST XH 4P (12V/GND/SDA/SCL IN) - always populated
- **J2**: JST XH 4P (12V/GND/SDA/SCL OUT) - always populated
- **J3**: Screw term 2P (Valve) - always populated
- **J4**: Screw term 2P (12V/GND INJECTION) - **optional, populate every 3-4 modules**

**Power flow**:
- If J4 populated: Power enters via J4 + J1, exits via J2
- If J4 not populated: Power flows J1 → PCB → J2 (pass-through)

**Wiring in practice** (12 valves, 3 injection points):
```
PSU Terminal 1 → Module 1 [J4] → JST chain → Modules 2,3,4
PSU Terminal 2 → Module 5 [J4] → JST chain → Modules 6,7,8
PSU Terminal 3 → Module 9 [J4] → JST chain → Modules 10,11,12
```

**Benefits**:
- ✅ One PCB design (simplify BOM, assembly)
- ✅ Flexible: adjust injection points based on actual current draw
- ✅ Easy assembly: JST cables between all modules, add power where needed
- ✅ Proven approach (LED strips use this)

---

## Fusing & Protection

**PSU-level fusing** (at each injection point):
```
PSU Term 1 ──[5A fuse]── 18AWG ──→ Module 1 [J4, JP1 open]
PSU Term 2 ──[5A fuse]── 18AWG ──→ Module 5 [J4, JP1 open]
PSU Term 3 ──[5A fuse]── 18AWG ──→ Module 9 [J4, JP1 open]
```

**Fuse selection**:
- **Rating**: 5A (4 modules × 1A max + 25% margin)
- **Type**: Automotive blade or inline holder with glass tube
- **Blow**: Medium-slow (handle valve inrush, ~50ms)

**Why 5A, not 10A?**
- Protects JST connectors (3A rated)
- Fast enough to prevent wire damage
- 10A would allow dangerous overload before blowing

**Backfeed protection**:
- Schottky diode D2 on each module
- JP1 open on injection modules = diode active
- JP1 closed on pass-through modules = diode bypassed
- **If injection fuse blows**: diode prevents backfeed from next segment

**Assembly per module**:
| Module | JP1 Jumper | F1 Fuse | J4 Populated | D2 Required | PSU Connection |
|--------|------------|---------|--------------|-------------|----------------|
| 1,3,5,7,9,11 | OPEN | YES | YES | YES | 22AWG from Wago |
| 2,4,6,8,10,12 | CLOSED | NO | Optional | YES (safety) | None |

**Safety notes**:
- Always populate D2 on all modules for redundancy
- F1 holder footprint on all PCBs, only populate fuse on injection modules
- Green LED off = blown fuse or no power (easy diagnosis)
