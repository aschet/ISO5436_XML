#include <opengps/cxx/opengps.hxx>

size_t ogps_GetNameInfo(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetName(&src);
   return src.CopyTo(text, size);
}

size_t ogps_GetAboutInfo(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetAbout(&src);
   return src.CopyTo(text, size);
}

size_t ogps_GetVersionInfo(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetVersion(&src);
   return src.CopyTo(text, size);
}

void ogps_PrintVersionInfo()
{
   OpenGPS::Info::PrintVersion();
}

size_t ogps_GetCopyrightInfo(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetCopyright(&src);
   return src.CopyTo(text, size);
}

void ogps_PrintCopyrightInfo()
{
   OpenGPS::Info::PrintCopyright();
}

size_t ogps_GetLicenseInfo(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetLicense(&src);
   return src.CopyTo(text, size);
}

void ogps_PrintLicenseInfo()
{
   OpenGPS::Info::PrintLicense();
}
