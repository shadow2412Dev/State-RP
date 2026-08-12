stock bool:IsLeapYear(year)
{
    return (
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0)
    );
}

stock FormatUnixTime(timestamp, output[], size = sizeof(output))
{
    timestamp += 3 * 3600;

    new
        second,
        minute,
        hour,
        day,
        month,
        year;

    // Время
    second = timestamp % 60;
    timestamp /= 60;

    minute = timestamp % 60;
    timestamp /= 60;

    hour = timestamp % 24;

    // Количество полных дней с 01.01.1970
    new days = timestamp / 24;

    // Год
    year = 1970;

    for (;;)
    {
        new daysInYear = IsLeapYear(year) ? 366 : 365;

        if (days < daysInYear)
            break;

        days -= daysInYear;
        year++;
    }

    // Количество дней в месяцах
    static const daysInMonth[] =
    {
        31, // Январь
        28, // Февраль
        31, // Март
        30, // Апрель
        31, // Май
        30, // Июнь
        31, // Июль
        31, // Август
        30, // Сентябрь
        31, // Октябрь
        30, // Ноябрь
        31  // Декабрь
    };

    month = 0;

    while (month < 12)
    {
        new monthDays = daysInMonth[month];

        // Февраль високосного года
        if (month == 1 && IsLeapYear(year))
            monthDays = 29;

        if (days < monthDays)
            break;

        days -= monthDays;
        month++;
    }

    day = days + 1;
    month++;

    format(
        output,
        size,
        "%04d-%02d-%02d %02d:%02d:%02d",
        year,
        month,
        day,
        hour,
        minute,
        second
    );

    return 1;
}