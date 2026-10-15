# Explicit fixed-date conversion

As part of issue #20, `fixed_date_t::operator int64_t()` is explicit. Code can no
longer use a date as a scalar implicitly. The `angle_t` and `moment_t` numeric
conversions are separate parts of issue #4.

Keep integer date arithmetic typed:

```cpp
const fixed_date_t date{719163};
const fixed_date_t next = date + 1;
const fixed_date_t previous = date - 1;
const int64_t elapsed_days = next - previous;
```

For an algorithm defined on integer RD indices (such as an integer search or
modulo operation), explicitly extract the coordinate with `date.value()`.
`static_cast<int64_t>(date)` remains available, but `value()` names the unit at
the boundary. For example, `adjusted_remainder(date.value(), 7)` computes an
ISO weekday number from the RD coordinate.

For fractional-day calculations, convert the date to its midnight moment, then
round back to a date using the typed helper when required:

```cpp
const moment_t midnight = date;
const moment_t afternoon = midnight + 0.75;
const fixed_date_t containing_date = floor(afternoon);
const double fractional_day_coordinate = afternoon.days();
```

The implicit date-to-moment conversion and the `floor`, `ceil`, `round` helpers
were introduced in issue #18. As documented by `moment_t`, not every integer
day outside the range [-2^53, 2^53] is exactly representable in its `double`.
Use integer dates and date differences when exact large day counts are needed.

`date + 0.5`, `0.5 + date`, `date + angle` and `angle + date` are compilation
errors. A fractional-day offset belongs to a moment; an angle must not be added
to a date. Date-to-date differences remain `int64_t`, and date shifts by integer
days remain `fixed_date_t`.

[Compile-time checks](../tests/src/type_safety.cc) cover these contracts, including
integer date arithmetic above 2^53. Existing reference tests continue to verify
calendar, astronomy and holiday results.
