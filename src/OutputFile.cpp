/**
 * @file    OutputFile.cpp
 * @ingroup LoggerCpp
 * @brief   Output to the standard console using printf
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "OutputFile.h"
#include "Exception.h"

#include <cstdio>


namespace Log
{


// Open the output file
OutputFile::OutputFile(const Config::Ptr& aConfigPtr) :
    mpFile(NULL)
{
    assert(aConfigPtr);

    /// @todo Save parameters to members
    std::string filename = aConfigPtr->get("filename", "log.txt");
    mpFile = fopen(filename.c_str(), "ab");
    if (NULL == mpFile)
    {
        LOGGER_THROW("file \"" << filename << "\" not opened");
    }
    /// @todo Add other parameters : max_size, numer_of_file, append vs create...
    long max_size = aConfigPtr->get("max_size", 1024*1024);
}


// Close the file
OutputFile::~OutputFile()
{
    if (NULL != mpFile)
    {
        fclose(mpFile);
        mpFile = NULL;
    }
}


// Output the Log to the standard console using printf
void OutputFile::output(const Channel::Ptr& aChannelPtr, const Log& aLog) const
{
    const Time& time = aLog.getTime();

    // uses fprintf for atomic thread-safe operation
    fprintf(mpFile, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u.%.3u  %-20s %s  %s\n",
            time.year, time.month, time.day,
            time.hour, time.minute, time.second, time.ms,
            aChannelPtr->getName().c_str(), Log::toString(aLog.getSeverity()), (aLog.getStream()).str().c_str());
    fflush(stdout);
}


} // namespace Log
