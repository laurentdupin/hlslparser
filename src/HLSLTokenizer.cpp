//#include "Engine/Log.h"
//#include "Engine/String.h"
#include "Engine.h"

#include "HLSLTokenizer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

namespace M4
{

// The order here must match the order in the Token enum.
static const char* _reservedWords[] =
    {
        "float",
        "float2",
        "float3",
        "float4",
		"float2x2",
        "float3x3",
        "float4x4",
        "float4x3",
        "float4x2",
        "half",
        "half2",
        "half3",
        "half4",
		"half2x2",
        "half3x3",
        "half4x4",
        "half4x3",
        "half4x2",
        "bool",
		"bool2",
		"bool3",
		"bool4",
        "int",
        "int2",
        "int3",
        "int4",
        "uint",
        "uint2",
        "uint3",
        "uint4",
        "texture",
        "sampler",
        "sampler2D",
        "sampler3D",
        "samplerCUBE",
        "sampler2DShadow",
        "sampler2DMS",
        "sampler2DArray",
        "if",
        "else",
        "for",
        "while",
        "break",
        "true",
        "false",
        "void",
        "struct",
        "namespace",
        "cbuffer",
        "tbuffer",
        "register",
        "return",
        "continue",
        "discard",
        "const",
        "static",
        "inline",
        "uniform",
        "in",
        "out",
        "inout",
        "sampler_state",
        "technique",
        "pass",
        "Texture1D",
        "Texture1DArray",
        "Texture2D",
        "Texture2DArray",
        "Texture2DMS",
        "Texture2DMSArray",
        "Texture3D",
        "TextureCube",
        "TextureCubeArray",
        "SamplerState"
    };

static bool GetIsSymbol(char c)
{
    switch (c)
    {
    case ';':
    case ':':
    case '(': case ')':
    case '[': case ']':
    case '{': case '}':
    case '-': case '+':
    case '*': case '/':
    case '?':
    case '!':
    case ',':
    case '=':
    case '.':
    case '<': case '>':
    case '|': case '&': case '^': case '~':
    case '@':
        return true;
    }
    return false;
}

/** Returns true if the character is a valid token separator at the end of a number type token */
static bool GetIsNumberSeparator(char c)
{
    return c == 0 || isspace(c) || GetIsSymbol(c);
}

HLSLTokenizer::HLSLTokenizer(const char* fileName, const char* buffer, size_t length)
{
    m_buffer            = buffer;
    m_bufferEnd         = buffer + length;
    m_fileName          = fileName;
    m_lineNumber        = 1;
    m_tokenLineNumber   = 1;
    m_error             = false;
    Next();
}

void HLSLTokenizer::Next()
{

	while( SkipWhitespace() || SkipComment() || ScanLineDirective() || SkipPragmaDirective() )
    {
    }

    if (m_error)
    {
        m_token = (int)HLSLToken::EndOfStream;
        return;
    }

    m_tokenLineNumber = m_lineNumber;

    if (m_buffer >= m_bufferEnd || *m_buffer == '\0')
    {
        m_token = (int)HLSLToken::EndOfStream;
        return;
    }

    const char* start = m_buffer;

    // +=, -=, *=, /=, ==, <=, >=
    if (m_buffer[0] == '+' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::PlusEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '-' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::MinusEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '*' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::TimesEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '/' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::DivideEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '=' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::EqualEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '!' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::NotEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '<' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::LessEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '>' && m_buffer[1] == '=')
    {
        m_token = (int)HLSLToken::GreaterEqual;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '&' && m_buffer[1] == '&')
    {
        m_token = (int)HLSLToken::AndAnd;
        m_buffer += 2;
        return;
    }
    else if (m_buffer[0] == '|' && m_buffer[1] == '|')
    {
        m_token = (int)HLSLToken::BarBar;
        m_buffer += 2;
        return;
    }

    // ++, --
    if ((m_buffer[0] == '-' || m_buffer[0] == '+') && (m_buffer[1] == m_buffer[0]))
    {
        m_token = (m_buffer[0] == '+') ? (int)HLSLToken::PlusPlus : (int)HLSLToken::MinusMinus;
        m_buffer += 2;
        return;
    }

    // Check for the start of a number.
    if (ScanNumber())
    {
        return;
    }
    
    if (GetIsSymbol(m_buffer[0]))
    {
        m_token = static_cast<unsigned char>(m_buffer[0]);
        ++m_buffer;
        return;
    }

    // Must be an identifier or a reserved word.
    while (m_buffer < m_bufferEnd && m_buffer[0] != 0 && 
        (!GetIsSymbol(m_buffer[0]) || (((m_buffer + 1) < m_bufferEnd) && m_buffer[0] == ':' && m_buffer[1] == ':')) && 
        !isspace(m_buffer[0]))
    {
        if (((m_buffer + 1) < m_bufferEnd) && m_buffer[0] == ':' && m_buffer[1] == ':')
        {
            ++m_buffer;
        }

        ++m_buffer;
    }

    size_t length = m_buffer - start;
    memcpy(m_identifier, start, length);
    m_identifier[length] = 0;
    
    const int numReservedWords = sizeof(_reservedWords) / sizeof(const char*);
    for (int i = 0; i < numReservedWords; ++i)
    {
        if (strcmp(_reservedWords[i], m_identifier) == 0)
        {
            m_token = 256 + i;
            return;
        }
    }

    m_token = (int)HLSLToken::Identifier;

}

bool HLSLTokenizer::SkipWhitespace()
{
    bool result = false;
    while (m_buffer < m_bufferEnd && isspace(m_buffer[0]))
    {
        result = true;
        if (m_buffer[0] == '\n')
        {
            ++m_lineNumber;
        }
        ++m_buffer;
    }
    return result;
}

bool HLSLTokenizer::SkipComment()
{
    bool result = false;
    if (m_buffer[0] == '/')
    {
        if (m_buffer[1] == '/')
        {
            // Single line comment.
            result = true;
            m_buffer += 2;
            while (m_buffer < m_bufferEnd)
            {
                if (*(m_buffer++) == '\n')
                {
                    ++m_lineNumber;
                    break;
                }
            }
        }
        else if (m_buffer[1] == '*')
        {
            // Multi-line comment.
            result = true;
            m_buffer += 2;
            while (m_buffer < m_bufferEnd)
            {
                if (m_buffer[0] == '\n')
                {
                    ++m_lineNumber;
                }
                if (m_buffer[0] == '*' && m_buffer[1] == '/')
                {
                    break;
                }
                ++m_buffer;
            }
            if (m_buffer < m_bufferEnd)
            {
                m_buffer += 2;
            }
        }
    }
    return result;
}

bool HLSLTokenizer::SkipPragmaDirective()
{
	bool result = false;
	if( m_bufferEnd - m_buffer > 7 && *m_buffer == '#' )
	{
		const char* ptr = m_buffer + 1;
		while( isspace( *ptr ) )
			ptr++;

		if( strncmp( ptr, "pragma", 6 ) == 0 && isspace( ptr[ 6 ] ) )
		{
			m_buffer = ptr + 6;
			result = true;
			while( m_buffer < m_bufferEnd )
			{
				if( *( m_buffer++ ) == '\n' )
				{
					++m_lineNumber;
					break;
				}
			}
		}
	}
	return result;
}

bool HLSLTokenizer::ScanNumber()
{

    // Don't treat the + or - as part of the number.
    if (m_buffer[0] == '+' || m_buffer[0] == '-')
    {
        return false;
    }

    // Parse hex literals.
    if (m_bufferEnd - m_buffer > 2 && m_buffer[0] == '0' && m_buffer[1] == 'x')
    {
        char*   hEnd = NULL;
        int     iValue = strtol(m_buffer+2, &hEnd, 16);
        if (GetIsNumberSeparator(hEnd[0]))
        {
            m_buffer = hEnd;
            m_token  = (int)HLSLToken::IntLiteral;
            m_iValue = iValue;
            return true;
        }
    }

    char* fEnd = NULL;
    double fValue = String_ToDouble(m_buffer, &fEnd);

    if (fEnd == m_buffer)
    {
        return false;
    }

    char*  iEnd = NULL;
    int    iValue = String_ToInteger(m_buffer, &iEnd);

    // If the character after the number is an f then the f is treated as part
    // of the number (to handle 1.0f syntax).
	if( ( fEnd[ 0 ] == 'f' || fEnd[ 0 ] == 'h' ) && fEnd < m_bufferEnd )
	{
		++fEnd;
	}

	if( fEnd > iEnd && GetIsNumberSeparator( fEnd[ 0 ] ) )
	{
		m_buffer = fEnd;
		m_token = fEnd[ 0 ] == 'f' ? (int)HLSLToken::FloatLiteral : (int)HLSLToken::HalfLiteral;
        m_fValue = static_cast<float>(fValue);
        return true;
    }
    else if (iEnd > m_buffer && GetIsNumberSeparator(iEnd[0]))
    {
        m_buffer = iEnd;
        m_token  = (int)HLSLToken::IntLiteral;
        m_iValue = iValue;
        return true;
    }

    return false;
}

bool HLSLTokenizer::ScanLineDirective()
{
    
    if (m_bufferEnd - m_buffer > 5 && strncmp(m_buffer, "#line", 5) == 0 && isspace(m_buffer[5]))
    {

        m_buffer += 5;
        
        while (m_buffer < m_bufferEnd && isspace(m_buffer[0]))
        {
            if (m_buffer[0] == '\n')
            {
                Error("Syntax error: expected line number after #line");
                return false;
            }
            ++m_buffer;
        }

        char* iEnd = NULL;
        int lineNumber = String_ToInteger(m_buffer, &iEnd);

        if (!isspace(*iEnd))
        {
            Error("Syntax error: expected line number after #line");
            return false;
        }

        m_buffer = iEnd;
        while (m_buffer < m_bufferEnd && isspace(m_buffer[0]))
        {
            char c = m_buffer[0];
            ++m_buffer;
            if (c == '\n')
            {
                m_lineNumber = lineNumber;
                return true;
            }
        }

        if (m_buffer >= m_bufferEnd)
        {
            m_lineNumber = lineNumber;
            return true;
        }

        if (m_buffer[0] != '"')
        {
            Error("Syntax error: expected '\"' after line number near #line");
            return false;
        }
            
        ++m_buffer;
        
        int i = 0;
        while (i + 1 < s_maxIdentifier && m_buffer < m_bufferEnd && m_buffer[0] != '"')
        {
            if (m_buffer[0] == '\n')
            {
                Error("Syntax error: expected '\"' before end of line near #line");
                return false;
            }

            m_lineDirectiveFileName[i] = *m_buffer;
            ++m_buffer;
            ++i;
        }
        
        m_lineDirectiveFileName[i] = 0;
        
        if (m_buffer >= m_bufferEnd)
        {
            Error("Syntax error: expected '\"' before end of file near #line");
            return false;
        }

        if (i + 1 >= s_maxIdentifier)
        {
            Error("Syntax error: file name too long near #line");
            return false;
        }

        // Skip the closing quote
        ++m_buffer;
        
        while (m_buffer < m_bufferEnd && m_buffer[0] != '\n')
        {
            if (!isspace(m_buffer[0]))
            {
                Error("Syntax error: unexpected input after file name near #line");
                return false;
            }
            ++m_buffer;
        }

        // Skip new line
        ++m_buffer;

        m_lineNumber = lineNumber;
        m_fileName = m_lineDirectiveFileName;

        return true;

    }

    return false;

}

int HLSLTokenizer::GetToken() const
{
    return m_token;
}

float HLSLTokenizer::GetFloat() const
{
    return m_fValue;
}

int HLSLTokenizer::GetInt() const
{
    return m_iValue;
}

const char* HLSLTokenizer::GetIdentifier() const
{
    return m_identifier;
}

int HLSLTokenizer::GetLineNumber() const
{
    return m_tokenLineNumber;
}

const char* HLSLTokenizer::GetFileName() const
{
    return m_fileName;
}

void HLSLTokenizer::Error(const char* format, ...)
{
    // It's not always convenient to stop executing when an error occurs,
    // so just track once we've hit an error and stop reporting them until
    // we successfully bail out of execution.
    if (m_error)
    {
        return;
    }
    m_error = true;


    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);

    Log_Error("%s(%d) : %s\n", m_fileName, m_lineNumber, buffer);
} 

void HLSLTokenizer::GetTokenName(char buffer[s_maxIdentifier]) const
{
    if (m_token == (int)HLSLToken::FloatLiteral || m_token == (int)HLSLToken::HalfLiteral )
    {
        sprintf_s(buffer, s_maxIdentifier, "%f", m_fValue);
    }
    else if (m_token == (int)HLSLToken::IntLiteral)
    {
        sprintf_s(buffer, s_maxIdentifier, "%d", m_iValue);
    }
    else if (m_token == (int)HLSLToken::Identifier)
    {
        strcpy_s(buffer, s_maxIdentifier, m_identifier);
    }
    else
    {
        GetTokenName(m_token, buffer);
    }
}

void HLSLTokenizer::GetTokenName(int token, char buffer[s_maxIdentifier])
{
    if (token < 256)
    {
        buffer[0] = (char)token;
        buffer[1] = 0;
    }
    else if (token < (int)HLSLToken::LessEqual)
    {
        strcpy_s(buffer, s_maxIdentifier, _reservedWords[token - 256]);
    }
    else
    {
        switch (token)
        {
        case (int)HLSLToken::PlusPlus:
            strcpy_s(buffer, s_maxIdentifier, "++");
            break;
        case (int)HLSLToken::MinusMinus:
            strcpy_s(buffer, s_maxIdentifier, "--");
            break;
        case (int)HLSLToken::PlusEqual:
            strcpy_s(buffer, s_maxIdentifier, "+=");
            break;
        case (int)HLSLToken::MinusEqual:
            strcpy_s(buffer, s_maxIdentifier, "-=");
            break;
        case (int)HLSLToken::TimesEqual:
            strcpy_s(buffer, s_maxIdentifier, "*=");
            break;
        case (int)HLSLToken::DivideEqual:
            strcpy_s(buffer, s_maxIdentifier, "/=");
            break;
		case (int)HLSLToken::HalfLiteral:
            strcpy_s(buffer, s_maxIdentifier, "half" );
			break;
        case (int)HLSLToken::FloatLiteral:
            strcpy_s(buffer, s_maxIdentifier, "float");
            break;
        case (int)HLSLToken::IntLiteral:
            strcpy_s(buffer, s_maxIdentifier, "int");
            break;
        case (int)HLSLToken::Identifier:
            strcpy_s(buffer, s_maxIdentifier, "identifier");
            break;
        case (int)HLSLToken::EndOfStream:
            strcpy_s(buffer, s_maxIdentifier, "<eof>");
            break;
        default:
            strcpy_s(buffer, s_maxIdentifier, "unknown");
            break;
        }
    }

}

}
