# Explicit moment conversion

As part of issue #21, `moment_t::operator double()` is explicit. Code can no
longer use a moment as a floating-point scalar implicitly, so it cannot be
compared with a `double` or passed where a `double` is expected without saying
so. `moment_t` still represents both a point in time and an interval in days;
a separate duration type is outside the scope of #21.

Keep day arithmetic typed:

```cpp
const moment_t t = 730120.5_m;
const moment_t later = t + 0.25;           // a quarter of a day later
const moment_t earlier = t - 1;            // one day earlier
const moment_t elapsed = later - earlier;  // 1.25_m
const double ratio = elapsed / 0.5_m;      // 2.5 (dimensionless)
const bool before = earlier < t;           // moments compare with moments
```

For algorithms that work on raw day values (such as `binary_search`,
`inverse_angular`, polynomial evaluation or `std::` math functions), explicitly
extract the value with `t.days()`. `static_cast<double>(t)` remains available,
but `days()` names the unit at the boundary:

```cpp
const double c = (t - J2000).days() / 36525;  // Julian centuries
const moment_t found{ inverse_angular(f, y, a.days(), b.days()) };
```

To go back to a calendar date, use the typed rounding helpers instead of casting
the day value: `floor(t)` (same as `t.to_fixed_date()`), `ceil(t)` and `round(t)`
all return `fixed_date_t`. A `fixed_date_t` still converts implicitly to the
`moment_t` of its midnight.

`moment < double`, `moment == double`, `double d = moment` and `moment + angle`
are compilation errors.

[Compile-time checks](../tests/src/type_safety.cc) cover these contracts.
Existing reference tests continue to verify calendar, astronomy and holiday results.
