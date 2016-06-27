#ifndef DSB_INTERPRETER_STRING_UTILS_H
#define DSB_INTERPRETER_STRING_UTILS_H

#include <string>

namespace openrayman
{
    // Reads ahead in the string and returns true if a match is found.
    bool read_ahead_match(const std::string& source, std::size_t begin_at, const std::string& match)
    {
        if((int)source.length() - (int)begin_at + (int)match.length() < 0)
            return false;
        for(std::size_t n = 0; n < match.length(); n++)
        {
            if(source[n + begin_at] != match[n])
                return false;
        }
        return true;
    }

    // Returns the difference needed to skip whitespace from the point "at".
    std::size_t skip_whitespace(const std::string& source, std::size_t at)
    {
        std::size_t new_at = at;
        while((source[new_at] == ' ' || source[new_at] == '\t') && new_at < source.length())
            new_at++;
        return new_at - at;
    }

    // Returns the string in the range from point "at" to whitespace.
    std::string read_until_whitespace(const std::string& source, std::size_t at)
    {
        std::string result = "";
        while((source[at] != ' ' && source[at] != '\t') && at < source.length())
        {
            result += source[at];
            at++;
        }
        return result;
    }

    // Returns the string from point "at" until the specified character is found.
    std::string read_until_char(const std::string& source, std::size_t at, char until)
    {
        std::string result = "";
        while((source[at] != until) && at < source.length())
        {
            result += source[at];
            at++;
        }
        return result;
    }

    // Returns the string in the range from point "at" to end.
    std::string read_until_end(const std::string& source, std::size_t at)
    {
        std::string result = "";
        while(at < source.length())
        {
            result += source[at];
            at++;
        }
        return result;
    }

    // Reads a quote escaped string that starts at point "at".
    std::string read_quote_escaped_string(const std::string& source, std::size_t at)
    {
        std::string result = "";
        at++;
        bool next_escaped = false;
        while(at < source.length())
        {
            if(source[at] == '"')
                break;
            result += source[at];
            at++;
        }
        return result;
    }
}

#endif
