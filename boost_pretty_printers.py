import gdb

class BoostMultiprecisionPrinter:
    """Pretty-printer for boost::multiprecision::cpp_int and boost::multiprecision::cpp_rational"""

    def __init__(self, val):
        self.val = val

    def to_string(self):
        """Returns the string representation of the value using str()"""
        return str(self.val["str"]())

def lookup_boost_type(val):
    """Registers a pretty-printer for Boost.Multiprecision types."""
    type_name = val.type.strip_typedefs().name
    if type_name in ["boost::multiprecision::cpp_int", "boost::multiprecision::cpp_rational"]:
        return BoostMultiprecisionPrinter(val)
    return None

def register_boost_printers():
    """Registers the Boost pretty printers."""
    gdb.pretty_printers.append(lookup_boost_type)

register_boost_printers()
