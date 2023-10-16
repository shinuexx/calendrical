#include "calendars/iso_calendar.hpp"

namespace calendrical::calendars {
	iso_calendar::iso_calendar() {
		m_gregorian = gregorian_calendar::standard;

	}
}