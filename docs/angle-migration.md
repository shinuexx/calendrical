# Explicit angle conversion

As part of issue #19, `angle_t::operator double()` is explicit. Code can no
longer use an angle as a floating-point scalar implicitly. The `fixed_date_t`
and `moment_t` numeric conversions are separate parts of issue #4.

Keep angle arithmetic typed:

```cpp
const angle_t alpha = 45_deg;
const angle_t beta = 90_deg;
const angle_t sum = alpha + beta;          // angle_t{135}
const angle_t diff = beta - alpha;         // angle_t{45}
const angle_t scaled = alpha * 2.0;        // angle_t{90}
const double ratio = beta / alpha;         // 2.0 (dimensionless)
```

For algorithms that require raw degrees (such as standard library trigonometry
or external numerical routines), explicitly extract the value with `angle.degrees()`.
`static_cast<double>(angle)` remains available, but `degrees()` names the unit at
the boundary:

```cpp
const angle_t lon = 120.5_deg;
const double raw_degrees = lon.degrees();
```

For fractions of a full circle (turns or cyclic phases, such as synodic month
fractions), divide by `angle_t::full_circle` (yielding a dimensionless `double`):

```cpp
const angle_t phase = mod(lunar_phase(t) - target_phase, 360);
const double cycle_fraction = phase / angle_t::full_circle;
```

For Earth rotation and hour angle conversions where an angle represents an elapsed
fraction of a day, use the typed helper `day_fraction`:

```cpp
const double d_frac = day_fraction(ujjain.longitude - location.longitude);
```

Angle enums (`season`, `lunar_phase`) retain implicit conversion to `angle_t`:

```cpp
const angle_t spring = season::spring;          // 0_deg
const angle_t full = lunar_phase::full_moon;    // 180_deg
```

`angle + double`, `double + angle`, `angle - double`, `double - angle`,
`angle < double`, `10_rd + 20_deg` and `angle + moment` are compilation errors.
Angles cannot mix implicitly with scalars, calendar dates, or moments in time.

[Compile-time checks](../tests/src/type_safety.cc) cover these contracts.
Existing reference tests continue to verify calendar, astronomy and holiday results.
