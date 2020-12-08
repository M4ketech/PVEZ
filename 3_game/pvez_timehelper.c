class PVEZ_Date : Managed {
	static PVEZ_Date Now() {
		int year, month, day;
		GetYearMonthDay(year, month, day);
		autoptr PVEZ_Date date = new PVEZ_Date(year, month, day);
		return date;
	}

	int Year, Month, Day;
	void PVEZ_Date(int year = 2000, int month = 1, int day = 1) {
		Year = year;
		Month = month;
		Day = day;
	}
}

class PVEZ_DateTime : Managed {
	static PVEZ_DateTime Now() {
		int year, month, day, hour, minute;
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, NULL);
		autoptr PVEZ_DateTime date = new PVEZ_DateTime(year, month, day, hour, minute);
		return date;
	}

	int Year, Month, Day, Hour, Minute;
	void PVEZ_DateTime(int year = 2000, int month = 1, int day = 1, int hour = 0, int minute = 0) {
		Year = year;
		Month = month;
		Day = day;
		Hour = hour;
		Minute = minute;
	}
}

class PVEZ_TimeHelper : Managed {

	// Returns current date+time (real world) in format: "January 1, 21:10:02"
	static string GetFormattedDateTime() {
		int year, month, day, hour, minute, second;
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);
		string monthNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		string monthName = monthNames[month - 1];
		return monthName + " " + day + ", " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	}

	static string GetFormattedDateTimeFromPVEZ_DateTime(PVEZ_DateTime dt) {
		string monthNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		string monthName = monthNames[dt.Month - 1];
		return monthName + " " + dt.Day.ToString() + ", " + dt.Hour.ToStringLen(2) + ":" + dt.Minute.ToStringLen(2);
	}

	// 1st date must be the past, 2nd date the future
	static int CountDaysBetweenDates(PVEZ_DateTime date1, PVEZ_DateTime date2) {
		int years = date1.Year - date2.Year;
		int months = date1.Month - date2.Month;
		int days = date1.Day - date2.Day;
		int result = years * 365 + months * 31 + days;
		return -result;
	}

	static int CountHoursBetweenDates(PVEZ_DateTime date1, PVEZ_DateTime date2) {
		int years = date1.Year - date2.Year;
		int months = date1.Month - date2.Month;
		int days = date1.Day - date2.Day;
		int hours = date1.Hour - date2.Hour;
		int result = years * 365 + months * 31 + days * 24 + hours;
		return -result;
	}

	static int CountMinutesBetweenDates(PVEZ_DateTime date1, PVEZ_DateTime date2) {
		int years = date1.Year - date2.Year;
		int months = date1.Month - date2.Month;
		int days = date1.Day - date2.Day;
		int hours = date1.Hour - date2.Hour;
		int minutes = date1.Minute - date2.Minute;
		int result = years * 365 + months * 31 + days * 24 + hours * 60 + minutes;
		return -result;
	}

	// Argument Date8 is the string that should represent a date with 8 digits: e.g., 20200517 which means year-2020 month-05 day-17.
	// It's used in <PVEZ_LawbreakersRoster> (Lawbreakers persistence system).
	static PVEZ_Date GetDateFromDate8(string Date8) {
		if (Date8.Length() < 8)
			return NULL;
		autoptr PVEZ_Date date = new PVEZ_Date;
		date.Year = Date8.Substring(0, 4).ToInt();
		date.Month = Date8.Substring(4, 2).ToInt();
		date.Day = Date8.Substring(6, 2).ToInt();
		return date;
	}

	// Returns current date in 8-digits format. E.g., today is January, 4 2019, in this case the result will be "20190104".
	static string GetStringDate8() {
		int year, month, day;
		GetYearMonthDay(year, month, day);
		string result = year.ToStringLen(4) + month.ToStringLen(2) + day.ToStringLen(2);
		return result;
	}

	// Returns the number of the day in a week.
	static int GetDayOfWeek(bool SundayFirst) {
		int c, y, m, d;
		GetYearMonthDay(y, m, d);

		c = y/100;
		if (m < 3) {
			y -= 1;
			m += 10;
		}
		else m -= 2;
		
		int dowraw = (d + Math.Floor(2.6 * m - 0.2) - 2 * c + y + Math.Floor(y / 4) + Math.Floor(c / 4));

		// Here we get the remainder of dowraw divided by 7.
		// Basically, should have been just "dowraw % 7", but DayZ says it doesn't know the % operator,
		// Yet, it is definitely used in enscript though. Odd stuff.
		int dow = Math.Floor(dowraw / 7);
		dow = dow * 7;
		dow = dowraw - dow;
		if (dow == 0) dow = 7;

		// Move days for Mon-Sun week.
		if (!SundayFirst) {
			dow -= 1;
			if (dow == 0) dow = 7;
		}

		return dow;
	}
}