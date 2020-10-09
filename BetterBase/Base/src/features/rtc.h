#include <TimeLib.h>
#include <Wire.h>

#if ENABLED(RTC_TYPE_D3231)
#include <RtcDS3231.h>
RtcDS3231<TwoWire> RTC(Wire);
#endif

String printDigits(int digits)
{
    String out;
    out += ":";
    if (digits < 10)
        out += '0';
    out += digits;
    return out;
}

#if ENABLED(RTC_TYPE_D1207)

String printTime(time_t t)
{
    String out;
    // display the given time
    out += hour(t);
    out += printDigits(minute(t));
    out += printDigits(second(t));
    out += " ";
    out += day(t);
    out += "/";
    out += month(t);
    out += "/";
    out += year(t);
    return out;
}
#endif
#if ENABLED(RTC_TYPE_D3231)
String printTime(const RtcDateTime &dt)
{
    String out;
    // display the given time
    out += dt.Hour();
    out += printDigits(dt.Minute());
    out += printDigits(dt.Second());
    out += " ";
    out += dt.Day();;
    out += "/";
    out += dt.Month();
    out += "/";
    out += dt.Year();
    
    return out;
}
#endif

void initRTC()
{
#if ENABLED(RTC_TYPE_D3231)
    PRINT_DEBUG_LN("Initializing D3231 RTC");
    RTC.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    PRINT_DEBUG_LN(printTime(compiled));
    PRINT_DEBUG_LN("------");

    if (!RTC.IsDateTimeValid())
    {
        PRINT_DEBUG_LN("[RTC] Some sort of Time Problem detected!");
        if (RTC.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            PRINT_DEBUG("[RTC] RTC communications error = ");
            PRINT_DEBUG_LN(RTC.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            PRINT_DEBUG_LN("[RTC] RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            RTC.SetDateTime(compiled);
        }
    }
    PRINT_DEBUG_LN("[RTC] Passed validation of time!");

    if (!RTC.GetIsRunning())
    {
        PRINT_DEBUG_LN("[RTC] RTC was not actively running, starting now");
        RTC.SetIsRunning(true);
    }

    RtcDateTime now = RTC.GetDateTime();
    if (now < compiled)
    {
        PRINT_DEBUG_LN("[RTC] RTC is older than compile time!  (Updating DateTime)");
        RTC.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
        PRINT_DEBUG_LN("[RTC] RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled)
    {
        PRINT_DEBUG_LN("[RTC] RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    RTC.Enable32kHzPin(false);
    RTC.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
#endif
}

String getTimeNow()
{
#if ENABLED(RTC_TYPE_D1207)
    return printTime(now());
#endif
#if ENABLED(RTC_TYPE_D3231)
    return printTime(RTC.GetDateTime());
#endif
}